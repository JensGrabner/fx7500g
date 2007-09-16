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
  QList<int> returned = computer.computeExpression(expression);
  error = computer._lastError;
  return returned;
}

QList<int> ExpressionComputer::computeExpression(const QList<int> &expression)
{
  QList<int> result;
  _numberStack.clear();
  _commandStack.clear();
  _offset = 0;
  _expression = expression;

  _lastError = Error_No;

  ExpressionToken token;
  bool syntaxError = false;
  bool numberStackError = false;
  bool commandStackError = false;
  while ((token = readToken(syntaxError)).tokenType() != ExpressionToken::TokenType_EOL)
  {
    if (syntaxError)
    {
      _lastError = Error_Syntax;
      return result;
    }

    switch (token.tokenType())
    {
    case ExpressionToken::TokenType_Number: // Ex: 1.6546
      pushNumber(token.value(), numberStackError);
      break;
    case ExpressionToken::TokenType_Operator: // +, -, *, /
      if (!_commandStack.isEmpty() and isOperator(_commandStack.top()))
      {
        // Already an operator on top
        if (comparePriorities(token.command(), _commandStack.top()) <= 0)
          performOperation(_commandStack.pop());
      }
      pushCommand(token.command(), commandStackError);
      break;
    case ExpressionToken::TokenType_OpenParen: // (
      pushCommand(token.command(), commandStackError);
      break;
    case ExpressionToken::TokenType_CloseParen: // )
      // Consume all operators
      performStackOperations();

      if (!_commandStack.isEmpty() && _commandStack.top() == LCDChar_OpenParen)
        _commandStack.pop();
      else // No corresponding open parenthesis => syntax error
      {
        _lastError = Error_Syntax;
        return result;
      }
      break;
    default:;
    }
  }

  if (syntaxError)
  {
    _lastError = Error_Syntax;
    return result;
  }

  if (numberStackError)
  {
    _lastError = Error_NumberStack;
    return result;
  }

  if (commandStackError)
  {
    _lastError = Error_CommandStack;
    return result;
  }

  // Consume all resting operators
  performStackOperations();

  QString sortie;
  double n = _numberStack.top();
  if (n < 0.01)
    sortie = QString::number(_numberStack.top(), 'E', 0);
  else
    sortie = QString::number(_numberStack.top(), 'G');
  qDebug(qPrintable(sortie));

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
  return (entity == LCDChar_Multiply) ||
         (entity == LCDChar_Divide) ||
         (entity == LCDChar_Add) ||
         (entity == LCDChar_Substract);
}

void ExpressionComputer::performStackOperations(bool manageOpenParen)
{
  while (!_commandStack.isEmpty())
  {
    if (isOperator(_commandStack.top()))
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
    {
      double d2 = _numberStack.pop();
      double d1 = _numberStack.pop();

      if (entity == LCDChar_Multiply)
        _numberStack.push(d1 * d2);
      else if (entity == LCDChar_Divide)
        _numberStack.push(d1 / d2);
      else if (entity == LCDChar_Add)
        _numberStack.push(d1 + d2);
      else if (entity == LCDChar_Substract)
        _numberStack.push(d1 - d2);
      break;
    }
  default:;
  }
}

ExpressionToken ExpressionComputer::readToken(bool &syntaxError)
{
  syntaxError = false;
  if (_offset >= _expression.count())
    return ExpressionToken();

  switch (_expression[_offset])
  {
  case LCDChar_Add:
  case LCDChar_Substract:
  case LCDChar_Multiply:
  case LCDChar_Divide:
    {
      ExpressionToken token(ExpressionToken::TokenType_Operator);
      token.setCommand(_expression[_offset]);
      _offset++;
      return token;
    }
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
        {
          syntaxError = true;
          return ExpressionToken();
        }
        _offset++;
      }
      return ExpressionToken(numberStr.toDouble());
    } else
    {
      syntaxError = true;
      return ExpressionToken();
    }
  }
  return ExpressionToken();
}

void ExpressionComputer::pushNumber(double value, bool &stackError)
{
  stackError = _numberStack.count() == _numberStackLimit;
  if (!stackError)
    _numberStack.push(value);
}

void ExpressionComputer::pushCommand(int command, bool &stackError)
{
  stackError = _commandStack.count() == _commandStackLimit;
  if (!stackError)
    _commandStack.push(command);
}

void ExpressionComputer::displayCommandStack() const
{
  foreach (int command, _commandStack)
    qDebug(qPrintable(ExpressionToken::toString(command)));
}
