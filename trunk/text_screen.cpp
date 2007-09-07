#include "text_screen.h"

ShellLine::ShellLine(const LCDString &lcdStr, bool rightJustified) :
  _isRightJustified(rightJustified)
{
  (*this) << lcdStr;
}

LCDChar ShellLine::charAtOffset(int offset) const
{
  if (offset >= length())
    return LCDChar_Space;

  int l = offset;

  foreach (const LCDString &lcdStr, *this)
    if (l - lcdStr.count() >= 0)
      l -= lcdStr.count();
    else
      return lcdStr[l];

  return LCDChar_Space;
}

int ShellLine::stringIndexAtOffset(int offset) const
{
  Q_ASSERT_X(offset >= 0, "ShellLine::stringIndexAtOffset()", "<offset> is negative!");

  if (offset >= length())
    return -1;

  int l = offset;
  int index = 0;
  foreach (const LCDString &lcdStr, *this)
    if (l - lcdStr.count() >= 0)
    {
      l -= lcdStr.count();
      index++;
    } else
      return index;

  return -1;
}

int ShellLine::offsetByStringIndex(int strIndex) const
{
  if (strIndex >= count())
    return length();

  int offset = 0;
  for (int i = 1; i <= strIndex; ++i)
    offset += at(i - 1).count();
  return offset;
}

int ShellLine::length() const
{
  int length = 0;
  foreach (const LCDString &lcdStr, *this)
    length += lcdStr.count();

  return length;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

TextScreen::TextScreen() :
  _calcState(0),
  _cursorLine(0),
  _cursorCol(0),
  _cursorVisible(true),
  _cursorMode(CursorMode_Normal),
  _displayCursorTurn(true)
{
  clear();

  _blinkTimer.setInterval(500); // Cursor blinks every second
  connect(&_blinkTimer, SIGNAL(timeout()), this, SLOT(doBlinkCursor()));
  _blinkTimer.start();
}

void TextScreen::init(const CalculatorState *calcState)
{
  _calcState = calcState;
  feedScreen();
}

void TextScreen::doBlinkCursor()
{
  if (!_cursorVisible)
    return;

  LCDChar charToDisplay;
  if (_displayCursorTurn)
  {
    switch (_cursorMode)
    {
    case CursorMode_Insert:         charToDisplay = LCDChar_InsertCursor; break;
    case CursorMode_InsertCapsLock: charToDisplay = LCDChar_InsertCapsLockCursor; break;
    case CursorMode_Shift:          charToDisplay = LCDChar_ShiftCursor; break;
    case CursorMode_CapsLock:       charToDisplay = LCDChar_CapsLockCursor; break;
    case CursorMode_Space:          charToDisplay = LCDChar_Space; break;
    default:                        charToDisplay = LCDChar_Cursor;
    }
  } else
    charToDisplay = _screen[_cursorCol][_cursorLine];

  emit changeChar(_cursorCol, _cursorLine, charToDisplay);

  _displayCursorTurn = !_displayCursorTurn;
}

void TextScreen::setCursorVisible(bool value)
{
  if (_cursorVisible == value)
    return;

  _cursorVisible = value;

  if (!value) // Hide the cursor (restore the true LCDChar)
    emit changeChar(_cursorCol, _cursorLine, _screen[_cursorCol][_cursorLine]);
  else // Show the cursor
    restartBlink();
}

void TextScreen::restartBlink()
{
  _displayCursorTurn = true;
  doBlinkCursor();
  _blinkTimer.start();
}

QList<LCDString> TextScreen::currentScreen() const
{
  QList<LCDString> screen;

  for (int line = 0; line < 8; ++line)
  {
    LCDString lcdStr;
    for (int col = 0; col < 16; ++col)
      lcdStr << _screen[col][line];

    screen << lcdStr;
  }

  return screen;
}

void TextScreen::clear()
{
  for (int line = 0; line < 8; ++line)
    for (int col = 0; col < 16; ++col)
      _screen[col][line] = LCDChar_Space;
}

void TextScreen::setCursorMode(CursorMode mode)
{
  if (mode == _cursorMode)
    return;

  _cursorMode = mode;
  restartBlink();
}

void TextScreen::moveCursor(int col, int line)
{
  // Restore old offset char
  emit changeChar(_cursorCol, _cursorLine, _screen[_cursorCol][_cursorLine]);

  // Move cursor
  _cursorCol = col;
  _cursorLine = line;

  restartBlink();
}

void TextScreen::assignToScreen(const LCDString &str, int col, int line)
{
  Q_ASSERT_X(col >= 0 && col < 16 && line >= 0 && line < 8, "assignToScreen()", QString("Invalid <col> (%1) or <line> (%2)").arg(col, line));

  int offset = 0;
  foreach (LCDChar c, str)
  {
    if (col + offset > 15)
      break;
    _screen[col + offset++][line] = c;
  }
}
