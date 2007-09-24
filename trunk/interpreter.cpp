#include <math.h>

#include "memory.h"

#include "interpreter.h"

Interpreter::Interpreter(const QList<TextLine> &program) :
  _currentOffset(0),
  _waitForDataMode(false)
{
  setProgram(program);
}

void Interpreter::execute(const TextLine &textLine)
{
  if (_waitForDataMode)
  {
    // _currentOffset is valid
    qDebug("Affectation !");
  } else
  {
    int entity;
    while ((entity = currentEntity()) != -1)
    {
      switch (entity)
      {
      case LCDChar_DoubleQuote: // A string instruction?
        {
          TextLine textLine = parseString();
          if (currentEntity() == LCDChar_Question)
            textLine << LCDChar_Question;
          emit displayLine(textLine);
          if (eatEntity(LCDChar_Question))
            parseInput();
          if (_waitForDataMode)
            return;
        }
        break;
      case LCDChar_Question: // An affectation?
        emit displayLine(TextLine() << LCDChar_Question);
        readEntity(); // Pass the "?"
        parseInput();
        return;
      case LCDOp_Lbl: // A label
        readEntity(); // Pass the "label "
        if (!isCipher(currentEntity()))
          throw InterpreterException(Error_Argument, _currentOffset);
        readEntity(); // Pass the cipher
        if (!isSeparator(currentEntity()) && currentEntity() != -1)
          throw InterpreterException(Error_Argument, _currentOffset);
        break;
      default:
        if (ExpressionSolver::isExpressionStartEntity(entity))
        {
          double d = _expressionSolver.solve(_program, _currentOffset);
          if (eatEntity(LCDChar_Arrow)) // Affectation?
          {
            // Parse a variable or a array var
            int varPrefix = currentEntity();
            if (!isAlpha(varPrefix))
              throw InterpreterException(Error_Syntax, _currentOffset);
            readEntity();
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
          } else if (isComparisonOperator(currentEntity()))
          {
            int comp = readEntity();
            double d2 = _expressionSolver.solve(_program, _currentOffset);

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
          TextLine textLine = formatDouble(d);
          textLine.setRightJustified(true);
          emit displayLine(textLine);
        }
      }
      // Separator is mandatory
      if (isSeparator(currentEntity()) || currentEntity() == -1)
        readEntity();
      else
        throw InterpreterException(Error_Syntax, _currentOffset);
    }
  }

  // Display the stack value?
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
      emit displayLine(textLine);
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

  _waitForDataMode = true; // Waiting for IN...
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
