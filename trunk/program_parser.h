#ifndef PROGRAM_PARSER_H
#define PROGRAM_PARSER_H

#include "misc.h"

class Token
{
public:
  enum TokenType {
    Type_Number,       // Integer or double
    Type_String,       // "Test"
    Type_Entity,       // LCDChar or LCDOperator
    Type_OpenArrayVar, // ex: A[
    Type_EOF           // End of file
  };

  Token(TokenType tokenType, const TextLine &rawParsed = TextLine());
  Token(int line = 0, int offset = 0);

  TokenType tokenType() const { return _tokenType; }
  const TextLine &rawParsed() const { return _rawParsed; }
  void setToken(TokenType tokenType, const TextLine &rawParsed);

  const TextLine &valueStr() const { return _valueStr; }
  double valueDbl() const { return _valueDbl; }

  int entity() const { return _entity; }

  int line() const { return _line; }
  int offset() const { return _offset; }

  void setLine(int value) { _line = value; }
  void setOffset(int value) { _offset = value; }

private:
  int _line; // Line in the program where token has been readed
  int _offset; // Offset in the line where token has been readed
  TokenType _tokenType;
  TextLine _rawParsed; // The real string which has been identified for the parsing
  TextLine _valueStr;
  double _valueDbl;
  int _entity;

  void feedValues();
};

class ProgramParser
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
    Exception(Error err, int l = 0, int o = 0) : error(err), line(l), offset(o) {}
    Error error;
    int line;
    int offset;
  };

  ProgramParser(const QList<TextLine> &program);

private:
  QList<TextLine> _lines;
  int _currentLine;
  int _currentOffset;

  Token readToken();
  int nextEntity(bool *linePassed = 0); // Returns -1 if we were at the end of all
};

#endif
