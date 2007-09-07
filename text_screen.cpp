#include "text_screen.h"

TextScreen::TextScreen() :
  _cursorLine(0),
  _cursorCol(0),
  _cursorVisible(true),
  _cursorMode(Cursor),
  _displayCursorTurn(true)
{
  clear();

  _blinkTimer.setInterval(500); // Cursor blinks every second
  connect(&_blinkTimer, SIGNAL(timeout()), this, SLOT(doBlinkCursor()));
  _blinkTimer.start();
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
    case InsertCursor:         charToDisplay = LCDChar_InsertCursor; break;
    case InsertCapsLockCursor: charToDisplay = LCDChar_InsertCapsLockCursor; break;
    case ShiftCursor:          charToDisplay = LCDChar_ShiftCursor; break;
    case CapsLockCursor:       charToDisplay = LCDChar_CapsLockCursor; break;
    default:                   charToDisplay = LCDChar_Cursor;
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

  // Cursor
  _cursorLine = 0;
  _cursorCol = 0;
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
