#include "editor_screen.h"

EditorScreen::EditorScreen()
{
}

void EditorScreen::feedScreen()
{
}

void EditorScreen::write(LCDChar c)
{
/*  if (_cursorOffset >= _promptLine.length())
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

  return true;*/
}

void EditorScreen::write(LCDOperator o)
{
/*  int newOffset;
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

  return true;*/
}

void EditorScreen::applyKey(int key)
{
}

void EditorScreen::moveLeft()
{
}

void EditorScreen::moveRight()
{
}

void EditorScreen::moveUp()
{
}

void EditorScreen::moveDown()
{
}

void EditorScreen::deleteString()
{
}

void EditorScreen::moveCursor(int newOffset)
{
}
