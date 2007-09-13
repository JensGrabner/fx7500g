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
    emit screenChanged();
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

void RunScreen::buttonClicked(int button)
{
  int entity = _calcState->printableEntityByButton(button);

  if (entity >= 0) // Printable entity
    writeEntity(entity);
}

void RunScreen::writeEntity(int entity)
{
  int offsetToAdd = LCDString(entity).count();
  if (_cursorOffset >= _promptLine.length())
    _promptLine << LCDString(entity);
  else
  {
    // Get LCDString under the cursor
    int index = _promptLine.stringIndexAtOffset(_cursorOffset);
    if (index >= 0)
      _promptLine[index] = LCDString(entity);
  }

  feedScreen();

  emit screenChanged();

  moveCursor(_cursorOffset + offsetToAdd);
}
