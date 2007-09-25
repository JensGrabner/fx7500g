#include <math.h>

#include "memory.h"

#include "interpreter.h"

Interpreter::Interpreter(const QList<TextLine> &program) :
  _currentOffset(0),
  _error(false),
  _errorStep(0),
  _waitForInput(false)
{
  setProgram(program);
}

void Interpreter::run()
{
  try
  {
    execute();
  } catch (InterpreterException exception)
  {
    _error = true;
    _errorStep = exception.offset();
    switch (exception.error())
    {
    case Error_Syntax: display(syntaxError(_errorStep)); break;
    case Error_Stack: display(stackError(_errorStep)); break;
    case Error_Memory: display(memError(_errorStep)); break;
    case Error_Argument: display(argError(_errorStep)); break;
    case Error_Goto: display(gotoError(_errorStep)); break;
    default:;
    }
  }
}

void Interpreter::execute() throw (InterpreterException)
{
  int entity;
  double lastResult = 0.0;
  bool lastResultExists = false;
  while ((entity = currentEntity()) != -1)
  {
    msleep(10);
    lastResultExists = false;
    switch (entity)
    {
    case LCDChar_DoubleQuote: // A string instruction?
      {
        TextLine textLine = parseString();
        if (currentEntity() == LCDChar_Question)
          textLine << LCDChar_Question;
        storeDisplayLine(textLine);
        emit displayLine();
        if (eatEntity(LCDChar_Question))
          parseInput();
      }
      break;
    case LCDChar_Question: // An input affectation?
      {
        storeDisplayLine(TextLine() << LCDChar_Question);
        emit displayLine();
        readEntity(); // Pass the "?"
        if (!eatEntity(LCDChar_Arrow)) // Pass the "->"
          throw InterpreterException(Error_Syntax, _currentOffset);
        // Wait for user data
        _waitForInput = true;
        _inputMutex.lock();
        _inputWaitCondition.wait(&_inputMutex);

        int offset = 0;
        double d = _expressionSolver.solve(_input, offset);
        lastResultExists = true;
        lastResult = d;

        // Compute the destination
        parseVariableAndStore(d);
      }
      break;
    case LCDOp_Lbl: parseLabel(); break;
    case LCDOp_Goto: parseGoto(); break;
    default:
      if (ExpressionSolver::isExpressionStartEntity(entity))
      {
        double d = _expressionSolver.solve(_program, _currentOffset);
        lastResultExists = true;
        lastResult = d;
        if (eatEntity(LCDChar_Arrow)) // Affectation?
          parseVariableAndStore(d);
        else if (isComparisonOperator(currentEntity()))
        {
          int comp = readEntity();
          double d2 = _expressionSolver.solve(_program, _currentOffset);
          lastResult = d2;

          // "=>" is expected
          if (!eatEntity(LCDChar_DoubleArrow))
            throw InterpreterException(Error_Syntax, _currentOffset);

          // Some non sep char is expected
          if (isSeparator(currentEntity()) || currentEntity() == -1)
            throw InterpreterException(Error_Syntax, _currentOffset);

          // compute boolean
          if (!computeBoolean(comp, d, d2))
            moveOffsetToNextInstruction();
          continue;
        }
      }
    }
    // Separator is mandatory
    if (isSeparator(currentEntity()) || currentEntity() == -1)
      readEntity();
    else
      throw InterpreterException(Error_Syntax, _currentOffset);
  }

  // Display the stack value?
  if (lastResultExists)
  {
    TextLine textLine = formatDouble(lastResult);
    textLine.setRightJustified(true);
    storeDisplayLine(textLine);
    emit displayLine();
  }
}

int Interpreter::readEntity()
{
  if (_currentOffset >= _program.count())
    return -1;

  return _program[_currentOffset++];
}

int Interpreter::currentEntity()
{
  if (_currentOffset >= _program.count())
    return -1;

  return _program[_currentOffset];
}

bool Interpreter::eatEntity(int entity)
{
  if (currentEntity() == entity)
  {
    readEntity();
    return true;
  } else
    return false;
}

int Interpreter::readAlpha() throw(InterpreterException)
{
  if (isAlpha(currentEntity()))
    return readEntity();
  else
    throw InterpreterException(Error_Syntax, _currentOffset);
}

void Interpreter::setProgram(const QList<TextLine> &program)
{
  _program.affect(program);
  _currentOffset = 0;
}

TextLine Interpreter::parseString()
{
  TextLine textLine;
  int entity;
  readEntity(); // Pass the "
  while ((entity = currentEntity()) != -1)
  {
    if (eatEntity(LCDChar_DoubleQuote))
      break;
    else if (entity == LCDChar_CR || entity == LCDChar_RBTriangle)
    {
      storeDisplayLine(textLine);
      emit displayLine();
      textLine.clear();
    } else
      textLine << entity;
    readEntity(); // Pass the current entity
  }
  if (entity == -1)
    throw InterpreterException(Error_Syntax, 0); // TODO: good line & offset
  return textLine;
}

void Interpreter::parseInput()
{
  if (currentEntity() != LCDChar_Arrow)
    throw InterpreterException(Error_Syntax, 0); // TODO: good line & offset

  readEntity(); // Pass the "->"
}

bool Interpreter::computeBoolean(int comp, double d1, double d2)
{
  switch (comp)
  {
  case LCDChar_Equal: return d1 == d2;
  case LCDChar_Different: return d1 != d2;
  case LCDChar_Greater: return d1 > d2;
  case LCDChar_Less: return d1 < d2;
  case LCDChar_GreaterEqual: return d1 >= d2;
  case LCDChar_LessEqual: return d1 <= d2;
  default: return false;
  }
}

void Interpreter::moveOffsetToNextInstruction()
{
  // Get after the next separator
  int entity;
  while ((entity = currentEntity()) != -1)
  {
    if (isSeparator(entity))
      break;
    readEntity(); // Pass the current entity
  }
}

void Interpreter::parseLabel()
{
  readEntity(); // Pass the "label "
  if (!isCipher(currentEntity()))
    throw InterpreterException(Error_Argument, _currentOffset);
  readEntity(); // Pass the cipher
  if (!isSeparator(currentEntity()) && currentEntity() != -1)
    throw InterpreterException(Error_Argument, _currentOffset);
}

void Interpreter::parseGoto()
{
  readEntity(); // Pass the "goto "
  if (!isCipher(currentEntity()))
    throw InterpreterException(Error_Argument, _currentOffset);
  int cipher = readEntity(); // Pass the cipher
  if (!isSeparator(currentEntity()) && currentEntity() != -1)
    throw InterpreterException(Error_Argument, _currentOffset);

  int p = 0;
  while ((p = _program.indexOf(LCDOp_Lbl, p)) >= 0)
  {
    if (p < _program.count() - 1 && _program[p + 1] == cipher &&
        (p == 0 || isSeparator(_program[p - 1])) &&
        (p + 1 >= _program.count() - 1 || isSeparator(_program[p + 2])))
    {
      _currentOffset = p + 2;
      return;
    }
    p++;
  }
  throw InterpreterException(Error_Goto, _currentOffset);
}

TextLine Interpreter::getNextDisplayLine()
{
  QMutexLocker loker(&_displayLineMutex);

  if (_displayLines.count())
    return _displayLines.dequeue();
  else
    return TextLine();
}

void Interpreter::storeDisplayLine(const TextLine &textLine)
{
  QMutexLocker loker(&_displayLineMutex);

  _displayLines.enqueue(textLine);
}

void Interpreter::sendInput(const TextLine &value)
{
  if (!_waitForInput)
    return;

  _input = value;
  _waitForInput = false;
  _inputWaitCondition.wakeAll();
}

QList<TextLine> Interpreter::syntaxError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Syn ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> Interpreter::stackError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Stk ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> Interpreter::memError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Mem ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> Interpreter::argError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Arg ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> Interpreter::gotoError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Go  ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

void Interpreter::display(const QList<TextLine> &lines)
{
  foreach (const TextLine &textLine, lines)
  {
    storeDisplayLine(textLine);
    emit displayLine();
  }
}

void Interpreter::parseVariableAndStore(double d)
{
  // Parse a variable or a array var
  int varPrefix = readAlpha();

  int index = 0;
  if (eatEntity(LCDChar_OpenBracket)) // Array var
  {
    index = (int) _expressionSolver.solve(_program, _currentOffset);
    if (!eatEntity(LCDChar_CloseBracket))
      throw InterpreterException(Error_Syntax, _currentOffset);
  }
  // Next entity is separator or end
  if (!isSeparator(currentEntity()) && currentEntity() != -1)
    throw InterpreterException(Error_Syntax, _currentOffset);

  // Stock it
  if (!Memory::instance().setVariable((LCDChar) varPrefix, index, d))
    throw InterpreterException(Error_Memory, _currentOffset);
}
