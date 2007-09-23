#include "program_parser.h"

Token::Token(TokenType tokenType, const TextLine &rawParsed) :
  _line(0), _offset(0)
{
  setToken(tokenType, rawParsed);
}

Token::Token(int line, int offset) :
  _line(line),
  _offset(offset)
{
}

void Token::setToken(TokenType tokenType, const TextLine &rawParsed)
{
  _tokenType = tokenType;
  _rawParsed = rawParsed;
  feedValues();
}

void Token::feedValues()
{
  // TODO : parse <rawParsed> and feed the values
}

/////////////////////////////////
/////////////////////////////////
/////////////////////////////////

ProgramParser::ProgramParser(const QList<TextLine> &program) :
  _lines(program),
  _currentLine(0), _currentOffset(0)
{
}

Token ProgramParser::readToken()
{
  if (_currentLine > _lines.count() - 1)
    return Token(Token::Type_EOF);

  const TextLine &textLine = _lines[_currentLine];

  Token token(_currentLine, _currentOffset);
  int entity = textLine[_currentOffset];

  if (isAlpha(entity))
  {
    bool linePassed;
    int nEntity = nextEntity(&linePassed);

    if (!linePassed && nEntity == LCDChar_OpenBracket)
    {
      token.setToken(Token::Type_OpenArrayVar, TextLine() << entity << nEntity);
      nextEntity(); // Pass the [
    } else // A regular entity
      token.setToken(Token::Type_Entity, TextLine() << entity);
  } else if (isCipher(entity) || entity == LCDChar_Dot)
  {
    TextLine numberLine;
    numberLine << entity;
    bool linePassed;
    while ((entity = nextEntity(&linePassed)) != -1)
    {
      if (!linePassed && (isCipher(entity) || entity == LCDChar_Dot))
      {
        // Continue the number
        if (isCipher(entity) || numberLine.indexOf(LCDChar_Dot) < 0)
          numberLine << entity;
        else
          throw Exception(Error_Syntax);
      } else
        break;
    }
    token.setToken(Token::Type_Number, numberLine);
  } else if (entity == LCDChar_DoubleQuote) // A string
  {
    TextLine rawLine;
    rawLine << LCDChar_DoubleQuote;
    bool linePassed;
    while ((entity = nextEntity(&linePassed)) != -1)
    {
      if (entity == LCDChar_DoubleQuote)
      {
        token.setToken(Token::Type_String, rawLine << LCDChar_DoubleQuote);
        break;
      }
      else if (linePassed)
        rawLine << LCDChar_CR;
      else
        rawLine << entity;
    }
    if (entity == -1) // Failed to close the string
      throw Exception(Error_Syntax);
  } else
  { // A regular entity
    token.setToken(Token::Type_Entity, TextLine() << entity);
    nextEntity();
  }

  return token;
}

int ProgramParser::nextEntity(bool *linePassed)
{
  linePassed = false;
  if (_currentLine >= _lines.count())
    return -1;

  const TextLine &textLine = _lines[_currentLine];
  if (_currentOffset >= textLine.count() - 1)
  {
    if (linePassed)
      *linePassed = true;
    _currentLine++;
  } else
    _currentOffset++;

  if (_currentLine >= _lines.count())
    return -1;

  return _lines[_currentLine][_currentOffset];
}
