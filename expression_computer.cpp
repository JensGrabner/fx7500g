#include "expression_computer.h"

ExpressionToken::ExpressionToken(TokenType tokenType) : _tokenType(tokenType), _value(0.0), _command(0)
{
  switch (tokenType)
  {
  case TokenType_OpenParen: _command = LCDChar_OpenParen; break;
  case TokenType_CloseParen: _command = LCDChar_CloseParen; break;
  default:;
  }
}

QString ExpressionToken::toString() const
{
  switch (_tokenType)
  {
  case TokenType_EOL: return "EOL"; break;
  case TokenType_Number: return QString("%1").arg(_value); break;
  case TokenType_Operator:
    switch (_command)
    {
    case LCDChar_Add: return "+"; break;
    case LCDChar_Substract: return "-"; break;
    case LCDChar_Multiply: return "*"; break;
    case LCDChar_Divide: return "/"; break;
    default: return "";
    }
    break;
  case TokenType_OpenParen: return "("; break;
  case TokenType_CloseParen: return ")"; break;
  default: return "";
  }
}

QString ExpressionToken::toString(int command)
{
  switch (command)
  {
  case LCDChar_Add: return "+"; break;
  case LCDChar_Substract: return "-"; break;
  case LCDChar_Multiply: return "*"; break;
  case LCDChar_Divide: return "/"; break;
  case LCDChar_OpenParen: return "("; break;
  case LCDChar_CloseParen: return ")"; break;
  default: return "";
  }
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////

QList<int> ExpressionComputer::compute(const QList<int> &expression, Error &error)
{
  ExpressionComputer computer;
  QList<int> returned;
  try
  {
    returned = computer.computeExpression(expression);
  } catch (Error err)
  {
    error = err;
  }
  return returned;
}

QList<int> ExpressionComputer::computeExpression(const QList<int> &expression) throw (Error)
{
  QList<int> result;
  _numberStack.clear();
  _commandStack.clear();
  _offset = 0;
  _expression = expression;

  ExpressionToken token;
  while ((token = readToken()).tokenType() != ExpressionToken::TokenType_EOL)
  {
    switch (token.tokenType())
    {
    case ExpressionToken::TokenType_Number: // Ex: 1.6546
      pushNumber(token.value());
      break;
    case ExpressionToken::TokenType_Operator: // +, -, *, /
      if (!_commandStack.isEmpty())
      {
        if (isOperator(_commandStack.top()) || isPreFunc(_commandStack.top()))
        {
          if (comparePriorities(token.command(), _commandStack.top()) <= 0)
            performOperation(_commandStack.pop());
        }
      }

      pushCommand(token.command());
      break;
    case ExpressionToken::TokenType_PreFunc: // log, ln, cos, sin, ...
      pushCommand(token.command());
      break;
    case ExpressionToken::TokenType_PostFunc: // !, ², ...
      if (!_commandStack.isEmpty())
      {
        if (isOperator(_commandStack.top()) ||
            isPreFunc(_commandStack.top()) ||
            isPostFunc(_commandStack.top()))
        {
          if (comparePriorities(token.command(), _commandStack.top()) <= 0)
            performOperation(_commandStack.pop());
        }
      }

      performOperation(token.command());
      break;
    case ExpressionToken::TokenType_OpenParen: // (
      pushCommand(token.command());
      break;
    case ExpressionToken::TokenType_CloseParen: // )
      // Consume all operators
      performStackOperations();

      if (!_commandStack.isEmpty() && _commandStack.top() == LCDChar_OpenParen)
        _commandStack.pop();
      else // No corresponding open parenthesis => syntax error
        throw Error_Syntax;
      break;
    default:;
    }
  }

  // Consume all resting operators
  performStackOperations(true);

  return formatDouble(_numberStack.top());
}

QList<int> ExpressionComputer::formatDouble(double d) const
{
  QList<int> result;

  QString sortie;
  // Format the double
  if (fabs(d) < 0.01 || fabs(d) >= 10000000000.0)
    sortie = QString::number(d, 'E', 9);
  else
    sortie = QString::number(d, 'G', 10);

  // Casio add a '.' at the end of a double if decimal part is 0
  if (sortie.indexOf('.') < 0)
    sortie.append('.');

  // Remove all excessive 0
  int p;
  while ((p = sortie.indexOf("0E")) >= 0)
    sortie.remove(p, 1);
  if (sortie.indexOf('E') < 0)
    while ((p = sortie.indexOf('0')) == sortie.length() - 1)
      sortie.remove(p, 1);

  // Store into the QList<int>
  foreach (const QChar &c, sortie)
  {
    const char ch = c.toLatin1();

    if (ch >= '0' && ch <= '9')
      result << LCDChar_0 + ch - '0';
    else if (ch == '.')
      result << LCDChar_Dot;
    else if (ch == '-')
      result << LCDChar_MinusPrefix;
    else if (ch == '+')
      result << LCDChar_Add;
    else if (ch == 'E')
      result << LCDChar_Exponent;
  }

  return result;
}

QChar ExpressionComputer::toChar(int entity) const
{
  if (entity >= LCDChar_0 && entity <= LCDChar_9)
    return QChar('0' + entity - LCDChar_0);
  else if (entity == LCDChar_Dot)
    return QChar('.');
  else
    return QChar();
}

bool ExpressionComputer::isOperator(int entity) const
{
  return entity == LCDChar_Multiply ||
         entity == LCDChar_Divide ||
         entity == LCDChar_Add ||
         entity == LCDChar_Substract ||
         entity == LCDOp_Xy ||
         entity == LCDOp_xSquareRoot;
}

bool ExpressionComputer::isPreFunc(int entity) const
{
  return entity == LCDChar_SquareRoot ||
         entity == LCDOp_CubeSquareRoot ||
         entity == LCDOp_Log ||
         entity == LCDChar_Ten ||
         entity == LCDOp_Ln ||
         entity == LCDChar_Euler ||
         entity == LCDOp_Sin ||
         entity == LCDOp_Cos ||
         entity == LCDOp_Tan ||
         entity == LCDOp_Sinh ||
         entity == LCDOp_Cosh ||
         entity == LCDOp_Tanh ||
         entity == LCDOp_Sin_1 ||
         entity == LCDOp_Cos_1 ||
         entity == LCDOp_Tan_1 ||
         entity == LCDOp_Sinh_1 ||
         entity == LCDOp_Cosh_1 ||
         entity == LCDOp_Tanh_1 ||
         entity == LCDChar_MinusPrefix ||
         entity == LCDOp_Abs ||
         entity == LCDOp_Int ||
         entity == LCDOp_Frac ||
         entity == LCDChar_h ||
         entity == LCDChar_d ||
         entity == LCDChar_b ||
         entity == LCDChar_o ||
         entity == LCDOp_Neg ||
         entity == LCDOp_Not;
}

bool ExpressionComputer::isPostFunc(int entity) const
{
  return entity == LCDChar_Square ||
         entity == LCDChar_MinusOneUp ||
         entity == LCDChar_Exclamation ||
         entity == LCDChar_LittleO ||
         entity == LCDChar_LittleR ||
         entity == LCDChar_LittleG ||
         entity == LCDChar_Degree;
}

void ExpressionComputer::performStackOperations(bool manageOpenParen)
{
  while (!_commandStack.isEmpty())
  {
    if (isOperator(_commandStack.top()) || isPreFunc(_commandStack.top()) ||
        isPostFunc(_commandStack.top()))
      performOperation(_commandStack.pop());
    else if (_commandStack.top() == LCDChar_OpenParen && manageOpenParen)
      _commandStack.pop();
    else
      break;
  }
}

void ExpressionComputer::performOperation(int entity)
{
  switch (entity)
  {
  case LCDChar_Multiply:
  case LCDChar_Divide:
  case LCDChar_Add:
  case LCDChar_Substract:
  case LCDOp_Xy:
  case LCDOp_xSquareRoot:
    {
      double d2 = _numberStack.pop();
      double d1 = _numberStack.pop();

      switch (entity)
      {
      case LCDChar_Multiply: _numberStack.push(d1 * d2); break;
      case LCDChar_Divide: _numberStack.push(d1 / d2); break;
      case LCDChar_Add: _numberStack.push(d1 + d2); break;
      case LCDChar_Substract: _numberStack.push(d1 - d2); break;
      case LCDOp_Xy: _numberStack.push(pow(d1, d2)); break;
      case LCDOp_xSquareRoot: _numberStack.push(pow(d2, 1.0 / d1)); break; // NEED a real xSquareRoot function
      default:;
      }
      break;
    }
  // Prefixed functions
  case LCDChar_SquareRoot: _numberStack.push(sqrt(_numberStack.pop())); break;
  case LCDOp_CubeSquareRoot: _numberStack.push(cbrt(_numberStack.pop())); break;
  case LCDOp_Log: _numberStack.push(log10(_numberStack.pop())); break;
  case LCDChar_Ten: _numberStack.push(exp10(_numberStack.pop())); break;
  case LCDOp_Ln: _numberStack.push(log(_numberStack.pop())); break;
  case LCDChar_Euler: _numberStack.push(exp(_numberStack.pop())); break;
  case LCDOp_Sin: _numberStack.push(sin(deg2rad(_numberStack.pop()))); break;
  case LCDOp_Cos: _numberStack.push(cos(deg2rad(_numberStack.pop()))); break;
  case LCDOp_Tan: _numberStack.push(tan(deg2rad(_numberStack.pop()))); break;
  case LCDOp_Sinh: _numberStack.push(sinh(_numberStack.pop())); break;
  case LCDOp_Cosh: _numberStack.push(cosh(_numberStack.pop())); break;
  case LCDOp_Tanh: _numberStack.push(tanh(_numberStack.pop())); break;
  case LCDOp_Sin_1: _numberStack.push(rad2deg(asin(_numberStack.pop()))); break;
  case LCDOp_Cos_1: _numberStack.push(rad2deg(acos(_numberStack.pop()))); break;
  case LCDOp_Tan_1: _numberStack.push(rad2deg(atan(_numberStack.pop()))); break;
  case LCDOp_Sinh_1: _numberStack.push(asinh(_numberStack.pop())); break;
  case LCDOp_Cosh_1: _numberStack.push(acosh(_numberStack.pop())); break;
  case LCDOp_Tanh_1: _numberStack.push(atanh(_numberStack.pop())); break;
  case LCDChar_MinusPrefix: _numberStack.push(-_numberStack.pop()); break;
  case LCDOp_Abs: _numberStack.push(fabs(_numberStack.pop())); break;
  case LCDOp_Int: _numberStack.push((int) _numberStack.pop()); break;
  case LCDOp_Frac: { double n = _numberStack.pop(); _numberStack.push(n - (double) ((int) n)); } break;
  case LCDChar_h: break;
  case LCDChar_d: break;
  case LCDChar_b: break;
  case LCDChar_o: break;
  case LCDOp_Neg: break;
  case LCDOp_Not: break;
  // Postfixed functions
  case LCDChar_Square: { double n = _numberStack.pop(); _numberStack.push(n * n); } break;
  case LCDChar_MinusOneUp: _numberStack.push(1.0 / _numberStack.pop()); break;
  case LCDChar_Exclamation: _numberStack.push(factorial(_numberStack.pop())); break;
  case LCDChar_LittleO: break;
  case LCDChar_LittleR: break;
  case LCDChar_LittleG: break;
  case LCDChar_Degree: break;
  default:;
  }
}

ExpressionToken ExpressionComputer::readToken() throw (Error)
{
  if (_offset >= _expression.count())
    return ExpressionToken();

  if (isOperator(_expression[_offset]))
  {
    ExpressionToken token(ExpressionToken::TokenType_Operator);
    token.setCommand(_expression[_offset]);
    _offset++;
    return token;
  } else if (isPreFunc(_expression[_offset]))
  {
    ExpressionToken token(ExpressionToken::TokenType_PreFunc);
    token.setCommand(_expression[_offset]);
    _offset++;
    return token;
  } else if (isPostFunc(_expression[_offset]))
  {
    ExpressionToken token(ExpressionToken::TokenType_PostFunc);
    token.setCommand(_expression[_offset]);
    _offset++;
    return token;
  } else
    switch (_expression[_offset])
    {
    case LCDChar_OpenParen:
      {
        ExpressionToken token(ExpressionToken::TokenType_OpenParen);
        _offset++;
        return token;
      }
    case LCDChar_CloseParen:
      {
        ExpressionToken token(ExpressionToken::TokenType_CloseParen);
        _offset++;
        return token;
      }
    default:
      if (isCipher(_expression[_offset]) || _expression[_offset] == LCDChar_Dot)
      {
        QString numberStr;
        numberStr.append(toChar(_expression[_offset]));
        _offset++;
        while (_offset < _expression.count() && (isCipher(_expression[_offset]) || _expression[_offset] == LCDChar_Dot))
        {
          // Continue the number
          if (isCipher(_expression[_offset]))
            numberStr.append(toChar(_expression[_offset]));
          else if (numberStr.indexOf('.') < 0)
            numberStr.append('.');
          else
            throw Error_Syntax;
          _offset++;
        }
        return ExpressionToken(numberStr.toDouble());
      } else
        throw Error_Syntax;
    }
  return ExpressionToken();
}

void ExpressionComputer::pushNumber(double value) throw (Error)
{
  if (_numberStack.count() == _numberStackLimit)
    throw Error_Stack;
  else
    _numberStack.push(value);
}

void ExpressionComputer::pushCommand(int command) throw (Error)
{
  if (_commandStack.count() == _commandStackLimit)
    throw Error_Stack;
  else
    _commandStack.push(command);
}

void ExpressionComputer::displayCommandStack() const
{
  foreach (int command, _commandStack)
    qDebug(qPrintable(ExpressionToken::toString(command)));
}

double ExpressionComputer::factorial(double value) const
{
  long long int f = (long long int) value;

  if (!f)
    return 1.0;
  else if (f == 1)
    return 1.0;

  double result = 1.0;
  for (int i = 2; i <= f; ++i)
    result *= (double) i;

  return result;
}
