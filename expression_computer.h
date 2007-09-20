#ifndef EXPRESSION_COMPUTER
#define EXPRESSION_COMPUTER

#include <math.h>

#include <QStack>

#include "misc.h"

class ExpressionToken
{
public:
  enum TokenType {
    TokenType_EOL,
    TokenType_Number,       // 1.3
    TokenType_Operator,     // +, x, -, /
    TokenType_PreFunc,      // Cos
    TokenType_PostFunc,     // !
    TokenType_OpenParen,    // (
    TokenType_CloseParen,   // )
    TokenType_Variable,     // A, B, C, etc
    TokenType_OpenArrayVar, // A[ => in this case, <_command> contains the letter involved
    TokenType_CloseBracket  // ]
  };

  ExpressionToken(TokenType tokenType = TokenType_EOL, int step = 0);
  ExpressionToken(double value, int step = 0) : _tokenType(TokenType_Number), _value(value), _command(0), _step(step) {}

  int step() const { return _step; } // Return the step of the token into the expression
  void setStep(int value) { _step = value; }

  bool isEOL() const { return _tokenType == TokenType_EOL; }
  bool isNumber() const { return _tokenType == TokenType_Number; }
  bool isOperator() const { return _tokenType == TokenType_Operator; }
  bool isPreFunc() const { return _tokenType == TokenType_PreFunc; }
  bool isPostFunc() const { return _tokenType == TokenType_PostFunc; }
  bool isOpenParen() const { return _tokenType == TokenType_OpenParen; }
  bool isCloseParen() const { return _tokenType == TokenType_CloseParen; }
  bool isVariable() const { return _tokenType == TokenType_Variable; }
  bool isOpenArrayVar() const { return _tokenType == TokenType_OpenArrayVar; }
  bool isCloseBracket() const { return _tokenType == TokenType_CloseBracket; }

  TokenType tokenType() const { return _tokenType; }

  double value() const;
  void setValue(double value) { _value = value; }

  int command() const { return _command; }
  void setCommand(int value) { _command = value; }

  QString toString() const;
  static QString toString(int command);

private:
  TokenType _tokenType;
  double _value;
  int _command;
  int _step;
};

class ExpressionComputer
{
public:
  enum Error {
    Error_No,
    Error_Syntax,
    Error_Stack,
    Error_Memory
  };

  class Exception {
  public:
    Exception();
    Exception(Error err, int st = 0) : error(err), step(st) {}
    Error error;
    int step;
  };

  ExpressionComputer() {}

  static QList<int> compute(const QList<int> &expression, Error &error, int &errorStep);

private:
  static const int _numberStackLimit = 9;
  static const int _commandStackLimit = 20;
  QStack<double> _numberStack;
  QStack<ExpressionToken> _commandStack;
  int _offset;
  QList<int> _expression;

  QList<int> computeExpression(const QList<int> &expression) throw (Exception);

  ExpressionToken readToken() throw (Exception);

  bool isOperator(int entity) const;
  bool isPreFunc(int entity) const;
  bool isPostFunc(int entity) const;

  void performOperation(int entity);
  void performStackOperations(bool manageOpenParen = false);

  bool isCipher(int entity) const { return entity >= LCDChar_0 && entity <= LCDChar_9; }
  QChar toChar(int entity) const;
  bool isAlpha(int entity) const { return entity >= LCDChar_A && entity <= LCDChar_Z; }

  void pushToken(const ExpressionToken &token) throw (Exception);

  void displayCommandStack() const;

  double deg2rad(double deg) const { return (deg * M_PI) / 180.0; }
  double rad2deg(double rad) const { return (rad * 180.0) / M_PI; }
  double factorial(double value) const;

  QList<int> formatDouble(double d) const;

  void analyzeForSyntaxError(ExpressionToken token, ExpressionToken previousToken) throw (Exception);
};

#endif
