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
    return -1;

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

  _promptLine << LCDString("Salut") << LCDString("Les") << LCDString("Amis");
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
    emit cursorBlinked(col, line, cursor);
  }
  else
  {
    if (_cursorOffset < _promptLine.length())
      emit cursorBlinked(col, line, _promptLine.charAtOffset(_cursorOffset));
    else
      emit cursorBlinked(col, line, LCDChar_Space);
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
    _cursorOffset++;
  } else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(c);
  }

  emit promptLineChanged();

  return true;
}

bool Shell::write(LCDOperator o)
{
  if (_cursorOffset >= _promptLine.length())
  {
    _promptLine << LCDString(o);
    _cursorOffset++;
  } else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(o);
  }

  emit promptLineChanged();

  return true;
}

bool Shell::moveLeft()
{
  int index = _promptLine.stringIndexAtOffset(_cursorOffset);
  if (index > 0)
    _cursorOffset = _promptLine.offsetByStringIndex(index - 1);
}

bool Shell::moveRight()
{
  int index = _promptLine.stringIndexAtOffset(_cursorOffset);
  if (index < _promptLine.count() - 1)
    _cursorOffset = _promptLine.offsetByStringIndex(index + 1);
}

bool Shell::moveUp()
{
}

bool Shell::moveDown()
{
}
