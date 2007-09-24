#ifndef TOKEN_H
#define TOKEN_H

#include "misc.h"

class Token
{
public:
  enum TokenType {
    Type_Number,       // Integer or double
    Type_Entity,       // LCDChar or LCDOperator
    Type_OpenArrayVar, // ex: A[
    Type_EOF           // End of file
  };

  Token(TokenType tokenType = Type_EOF, int offset = 0);
  Token(int entity, int offset = 0);

  TokenType tokenType() const { return _tokenType; }
  double value() const;
  void setValue(double value) { _value = value; }
  int entity() const { return _entity; }
  void setEntity(int entity) { _entity = entity; }

  int offset() const { return _offset; }
  void setOffset(int value) { _offset = value; }

  bool isOperatorToken() const { return _tokenType == Type_Entity && isOperator(_entity); }
  bool isPreFuncToken() const { return _tokenType == Type_Entity && isPreFunc(_entity); }
  bool isPostFuncToken() const { return _tokenType == Type_Entity && isPostFunc(_entity); }
  bool isVariable() const { return _tokenType == Type_Entity && isAlpha(_entity); }
  bool isEntity(int entity) const { return _tokenType == Type_Entity && _entity == entity; }

private:
  TokenType _tokenType;
  double _value;
  int _entity;
  int _offset; // Offset where the token has been read
};

#endif
