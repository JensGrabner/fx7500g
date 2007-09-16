#ifndef EXPRESSION_COMPUTER
#define EXPRESSION_COMPUTER

#include <QStack>

#include "misc.h"

class ExpressionToken
{
public:
  enum TokenType {
    TokenType_EOL,
    TokenType_Number,
    TokenType_Operator,
    TokenType_OpenParen,
    TokenType_CloseParen
  };

  ExpressionToken(TokenType tokenType = TokenType_EOL);
  ExpressionToken(double value) : _tokenType(TokenType_Number), _value(value), _command(0) {}

  bool isNumber() const { return _tokenType == TokenType_Number; }
  bool isOperator() const { return _tokenType == TokenType_Operator; }
  bool isOpenParen() const { return _tokenType == TokenType_OpenParen; }
  bool isCloseParen() const { return _tokenType == TokenType_CloseParen; }
  bool isEOL() const { return _tokenType == TokenType_EOL; }

  TokenType tokenType() const { return _tokenType; }

  double value() const { return _value; }
  void setValue(double value) { _value = value; }

  int command() const { return _command; }
  void setCommand(int value) { _command = value; }

  QString toString() const;
  static QString toString(int command);

private:
  TokenType _tokenType;
  double _value;
  int _command;
};

class ExpressionComputer
{
public:
  enum Error {
    Error_No,
    Error_Syntax,
    Error_NumberStack,
    Error_CommandStack
  };

  ExpressionComputer() {}

  static QList<int> compute(const QList<int> &expression, Error &error);

private:
  static const int _numberStackLimit = 9;
  static const int _commandStackLimit = 20;
  QStack<double> _numberStack;
  QStack<int> _commandStack;
  Error _lastError;
  int _offset;
  QList<int> _expression;

  ExpressionToken readToken(bool &syntaxError);

  QList<int> computeExpression(const QList<int> &expression);

  bool isOperator(int entity) const;

  void performOperation(int entity);
  void performStackOperations(bool manageOpenParen = false);

  bool isCipher(int entity) const { return entity >= LCDChar_0 && entity <= LCDChar_9; }
  QChar toChar(int entity) const;

  void pushNumber(double value, bool &stackError);
  void pushCommand(int command, bool &stackError);

  void displayCommandStack() const;
};

#endif
