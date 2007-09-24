#include "memory.h"

#include "token.h"

Token::Token(TokenType tokenType, int offset) :
  _tokenType(tokenType),
  _value(0.0),
  _entity(0),
  _offset(offset)
{
}

Token::Token(int entity, int offset) :
  _tokenType(Type_Entity),
  _value(0.0),
  _entity(entity),
  _offset(offset)
{
}

double Token::value() const
{
  if (isVariable())
    return Memory::instance().variable((int) ((LCDChar) _entity - LCDChar_A));
  else
    return _value;
}
