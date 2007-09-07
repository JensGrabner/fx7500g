#include "run_screen.h"

RunScreen::RunScreen() : TextScreen(),
  _cursorOffset(0),
  _prompt(true)
{
}

int RunScreen::getPromptLineIndex() const
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

bool RunScreen::write(LCDChar c)
{
  if (_cursorOffset >= _promptLine.length())
    _promptLine << LCDString(c);
  else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(c);
  }

  feedScreen();

  emit promptLineChanged();

  moveCursor(_cursorOffset + 1);

  return true;
}

bool RunScreen::write(LCDOperator o)
{
  int newOffset;
  if (_cursorOffset >= _promptLine.length())
  {
    _promptLine << LCDString(o);
    newOffset = _promptLine.length();
    moveCursor(_promptLine.length());
  } else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(o);

    // Cursor move to the right
    if (index >= _promptLine.count())
      newOffset = _promptLine.length();
    else
      newOffset = _promptLine.offsetByStringIndex(index + 1);
  }

  feedScreen();

  emit promptLineChanged();

  moveCursor(newOffset);

  return true;
}

void RunScreen::moveLeft()
{
  int index;
  if (_promptLine.length() && _cursorOffset >= _promptLine.length())
    index = _promptLine.count();
  else
    index = _promptLine.stringIndexAtOffset(_cursorOffset);

  if (index > 0)
    moveCursor(_promptLine.offsetByStringIndex(index - 1));
  else
    restartBlink();
}

void RunScreen::moveRight()
{
  int index = _promptLine.stringIndexAtOffset(_cursorOffset);
  if (index >= 0 && index <= _promptLine.count() - 1)
    moveCursor(_promptLine.offsetByStringIndex(index + 1));
  else
    restartBlink();
}

void RunScreen::moveUp()
{
  if (_cursorOffset < 16)
    moveCursor(0);
  else
  {
    int upIndex = _promptLine.stringIndexAtOffset(_cursorOffset - 16);
    moveCursor(_promptLine.offsetByStringIndex(upIndex));
  }
}

void RunScreen::moveDown()
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
}

void RunScreen::applyKey(int key)
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

void RunScreen::moveCursor(int newOffset)
{
  // Restore old offset char
  int line = newOffset / 16;
  int col = newOffset - line * 16;
  line += getPromptLineIndex();

  TextScreen::moveCursor(col, line);
  _cursorOffset = newOffset;
}

void RunScreen::deleteString()
{
  if (_cursorOffset < _promptLine.length())
  {
    _promptLine.removeAt(_promptLine.stringIndexAtOffset(_cursorOffset));
    feedScreen();
    emit promptLineChanged();
  }

  restartBlink();
}

void RunScreen::feedScreen()
{
  clear();

  int line = 0;
  foreach (const ShellLine &shellLine, _lines)
  {
    LCDString lcdStr;
    if (shellLine.isRightJustified())
    {
      Q_ASSERT_X(line.length() < 17, "Shell::currentScreen()", "A shell response length must be less or equal to 16");

      // Padding with space
      for (int i = 0; i < 16 - shellLine.length(); ++i)
        lcdStr << LCDChar_Space;
    }

    // Append real value
    foreach (const LCDString &subLcdStr, shellLine)
      foreach (LCDChar c, subLcdStr)
        lcdStr << c;

    // Copy it into <_screen>
    int col = 0;
    foreach (LCDChar c, lcdStr)
    {
      if (col >= 16)
      {
        col = 0;
        line++;
      }
      _screen[col++][line] = c;
    }
    line++;
  }

  // Prompt?
  if (_prompt)
  {
    int col = 0;
    foreach (const LCDString &subLcdStr, _promptLine)
      foreach (LCDChar c, subLcdStr)
      {
        if (col >= 16)
        {
          col = 0;
          line++;
        }
        _screen[col++][line] = c;
      }
  }
}
