#include "shell.h"

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

Shell::Shell() :
  _cursorOffset(0),
  _cursorMode(Cursor),
  _prompt(true),
  _displayCursorTurn(true)
{
  _blinkTimer.setInterval(500); // Cursor blinks every second
  connect(&_blinkTimer, SIGNAL(timeout()), this, SLOT(doBlinkCursor()));
  _blinkTimer.start();
}

void Shell::setCursorMode(CursorMode mode)
{
  if (mode == _cursorMode)
    return;

  _cursorMode = mode;
  _blinkTimer.start();
}

void Shell::doBlinkCursor()
{
  int line = _cursorOffset / 16;
  int col = _cursorOffset - line * 16;
  line += getPromptLineIndex();

  if (_displayCursorTurn)
  {
    LCDChar cursor = LCDChar_Cursor;
    switch (_cursorMode)
    {
    case Cursor: cursor = LCDChar_Cursor; break;
    case InsertCursor: cursor = LCDChar_InsertCursor; break;
    case InsertCapsLockCursor: cursor = LCDChar_InsertCapsLockCursor; break;
    case ShiftCursor: cursor = LCDChar_ShiftCursor; break;
    case CapsLockCursor: cursor = LCDChar_CapsLockCursor; break;
    default:;
    }
    emit changeChar(col, line, cursor);
  }
  else
  {
    if (_cursorOffset < _promptLine.length())
      emit changeChar(col, line, _promptLine.charAtOffset(_cursorOffset));
    else
      emit changeChar(col, line, LCDChar_Space);
  }
  _displayCursorTurn = !_displayCursorTurn;
}

QList<LCDString> Shell::currentScreen() const
{
  QList<LCDString> screen;

  foreach (const ShellLine &line, _lines)
  {
    LCDString lcdStr;
    if (line.isRightJustified())
    {
      Q_ASSERT_X(line.length() < 17, "Shell::currentScreen()", "A shell response length must be less or equal to 16");

      // Padding with space
      for (int i = 0; i < 16 - line.length(); ++i)
        lcdStr << LCDChar_Space;
    }

    // Append real value
    foreach (const LCDString &subLcdStr, line)
      foreach (LCDChar c, subLcdStr)
        lcdStr << c;
    screen << lcdStr;
  }

  // Prompt?
  if (_prompt)
  {
    LCDString lcdStr;

    foreach (const LCDString &subLcdStr, _promptLine)
      foreach (LCDChar c, subLcdStr)
        lcdStr << c;
    if (lcdStr.count())
      screen << lcdStr;
  }

  return screen;
}

int Shell::getPromptLineIndex() const
{
  int lineIndex = 0;
  foreach (const ShellLine &line, _lines)
  {
    if (line.isRightJustified())
      lineIndex++;
    else
      lineIndex += line.length() / 16 + 1;
  }
  return lineIndex;
}

void Shell::restartBlink()
{
  _displayCursorTurn = true;
  doBlinkCursor();
  _blinkTimer.start();
}

bool Shell::write(LCDChar c)
{
  if (_cursorOffset >= _promptLine.length())
  {
    _promptLine << LCDString(c);
    moveCursor(_cursorOffset + 1);
  } else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(c);
    moveCursor(_cursorOffset + 1);
  }

  restartBlink();

  emit promptLineChanged();

  return true;
}

bool Shell::write(LCDOperator o)
{
  if (_cursorOffset >= _promptLine.length())
  {
    _promptLine << LCDString(o);
    moveCursor(_promptLine.length());
  } else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(o);

    // Cursor move to the right
    if (index >= _promptLine.count())
      moveCursor(_promptLine.length());
    else
      moveCursor(_promptLine.offsetByStringIndex(index + 1));
  }

  restartBlink();

  emit promptLineChanged();

  return true;
}

void Shell::moveLeft()
{
  int index;
  if (_promptLine.length() && _cursorOffset >= _promptLine.length())
    index = _promptLine.count();
  else
    index = _promptLine.stringIndexAtOffset(_cursorOffset);

  if (index > 0)
    moveCursor(_promptLine.offsetByStringIndex(index - 1));

  restartBlink();
}

void Shell::moveRight()
{
  int index = _promptLine.stringIndexAtOffset(_cursorOffset);
  if (index >= 0 && index <= _promptLine.count() - 1)
    moveCursor(_promptLine.offsetByStringIndex(index + 1));

  restartBlink();
}

void Shell::moveUp()
{
  if (_cursorOffset < 16)
    moveCursor(0);
  else
  {
    int upIndex = _promptLine.stringIndexAtOffset(_cursorOffset - 16);
    moveCursor(_promptLine.offsetByStringIndex(upIndex));
  }

  restartBlink();
}

void Shell::moveDown()
{
  if (_cursorOffset + 16 >= _promptLine.length())
    moveCursor(_promptLine.length());
  else
  {
    int downIndex = _promptLine.stringIndexAtOffset(_cursorOffset + 16);
    int downOffset = _promptLine.offsetByStringIndex(downIndex);
    if (downOffset == _cursorOffset + 16)
      moveCursor(_cursorOffset + 16);
    else
      moveCursor(_promptLine.offsetByStringIndex(downIndex + 1));
  }

  restartBlink();
}

void Shell::applyKey(int key)
{
  switch (key)
  {
  case Qt::Key_Left: moveLeft(); break;
  case Qt::Key_Right: moveRight(); break;
  case Qt::Key_Up: moveUp(); break;
  case Qt::Key_Down: moveDown(); break;
  case Qt::Key_Delete: deleteString(); break;
  }
}

void Shell::moveCursor(int newOffset)
{
  // Restore old offset char
  int line = _cursorOffset / 16;
  int col = _cursorOffset - line * 16;
  line += getPromptLineIndex();
  emit changeChar(col, line, _promptLine.charAtOffset(_cursorOffset));

  // Move cursor
  _cursorOffset = newOffset;
}

void Shell::deleteString()
{
  if (_cursorOffset < _promptLine.length())
  {
    _promptLine.removeAt(_promptLine.stringIndexAtOffset(_cursorOffset));
    emit promptLineChanged();
  }

  restartBlink();
}
