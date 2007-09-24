#ifndef PROGRAM_PARSER_H
#define PROGRAM_PARSER_H

#include "misc.h"

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
