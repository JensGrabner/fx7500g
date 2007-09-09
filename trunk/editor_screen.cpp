#include "program.h"

#include "editor_screen.h"

EditorScreen::EditorScreen() :
  _topLineIndex(0),
  _topLineSubIndex(0),
  _cursorLineIndex(0),
  _cursorOffset(0)
{
}

void EditorScreen::setProgram(int programIndex)
{
  _lines.clear();
  Program &program = Programs::instance().at(programIndex);

  QList<LCDLine> &lines = program.steps();

  foreach (const LCDLine &lcdLine, lines)
  {
    ShellLine shellLine;
    for (int offset = 0; offset < lcdLine.count(); ++offset)
      shellLine << LCDString(lcdLine[offset]);

    _lines << shellLine;
  }

  feedScreen();
}

void EditorScreen::feedScreen()
{
  clear();

  int line = 0;
  for (int lineIndex = _topLineIndex; lineIndex < _lines.count(); ++lineIndex)
  {
    if (line > 7)
      break;
    ShellLine &shellLine = _lines[lineIndex];
    int startOffset = 0;
    if (lineIndex == _topLineIndex)
      startOffset = _topLineSubIndex * 16;
    int col = 0;
    for (int offset = startOffset; offset < shellLine.length(); ++offset)
    {
      if (col > 15)
      {
        line++;
        col = 0;
      }
      if (line > 7)
        break;
      _screen[col][line] = shellLine.charAtOffset(offset);
      col++;
    }

    // For empty end line in multi-lines cases
    if (shellLine.length() && !(shellLine.length() % 16) && _topLineSubIndex < shellLine.rowCount() - 1)
      line++;

    line++;
  }
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
  switch (key)
  {
  case Qt::Key_Left: moveLeft(); break;
  case Qt::Key_Right: moveRight(); break;
  case Qt::Key_Up: moveUp(); break;
  case Qt::Key_Down: moveDown(); break;
  case Qt::Key_Delete: deleteString(); break;
  }
}

void EditorScreen::moveLeft()
{
  if (!_cursorLineIndex && !_cursorOffset)
  {
    restartBlink();
    return;
  }

  ShellLine &shellLine = _lines[_cursorLineIndex];
  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  int index;
  if (!_cursorOffset)
  {
    newCursorLineIndex--;
    newCursorOffset = _lines[newCursorLineIndex].length();
  } else if (shellLine.length() && _cursorOffset >= shellLine.length())
    newCursorOffset = shellLine.offsetByStringIndex(shellLine.count() - 1);
  else
    newCursorOffset = shellLine.offsetByStringIndex(shellLine.stringIndexAtOffset(_cursorOffset) - 1);

  moveCursor(newCursorLineIndex, newCursorOffset);
}

void EditorScreen::moveRight()
{
  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorLineIndex >= _lines.count() - 1 && _cursorOffset >= shellLine.length())
  {
    restartBlink();
    return;
  }

  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  int index;
  if (_cursorOffset >= shellLine.length())
  {
    newCursorLineIndex++;
    newCursorOffset = 0;
  } else
    newCursorOffset = shellLine.offsetByStringIndex(shellLine.stringIndexAtOffset(_cursorOffset) + 1);

  moveCursor(newCursorLineIndex, newCursorOffset);
}

void EditorScreen::moveUp()
{
  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorOffset < 16)
  {
    // Previous line?
    if (_cursorLineIndex)
    {
      ShellLine &previousLine = _lines[_cursorLineIndex - 1];
      int previousOffset = (previousLine.rowCount() - 1) * 16 + (_cursorOffset % 16);
      if (previousOffset >= previousLine.length())
        moveCursor(_cursorLineIndex - 1, previousLine.length());
      else
        moveCursor(_cursorLineIndex - 1, previousLine.offsetByStringIndex(previousLine.stringIndexAtOffset(previousOffset)));
    } else
      moveCursor(_cursorLineIndex, 0);
  } else
  {
    int upIndex = shellLine.stringIndexAtOffset(_cursorOffset - 16);
    moveCursor(_cursorLineIndex, shellLine.offsetByStringIndex(upIndex));
  }
}

void EditorScreen::moveDown()
{
  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorOffset + 16 < shellLine.length())
  {
    int downIndex = shellLine.stringIndexAtOffset(_cursorOffset + 16);
    int downOffset = shellLine.offsetByStringIndex(downIndex);
    if (downOffset == _cursorOffset + 16)
      moveCursor(_cursorLineIndex, _cursorOffset + 16);
    else
      moveCursor(_cursorLineIndex, shellLine.offsetByStringIndex(downIndex + 1));
  } else if (_cursorOffset + 16 == shellLine.length())
    moveCursor(_cursorLineIndex, shellLine.length());
  else
  {
    if (_cursorLineIndex == _lines.count() - 1 || _cursorOffset / 16 < shellLine.rowCount() - 1)
      moveCursor(_cursorLineIndex, shellLine.length());
    else
    {
      // To the next line
      ShellLine &nextLine = _lines[_cursorLineIndex + 1];
      int nextOffset = _cursorOffset % 16;

      int downIndex = nextLine.stringIndexAtOffset(nextOffset);
      int downOffset = nextLine.offsetByStringIndex(downIndex);
      if (downOffset == nextOffset)
        moveCursor(_cursorLineIndex + 1, nextOffset);
      else
        moveCursor(_cursorLineIndex + 1, nextLine.offsetByStringIndex(downIndex + 1));
    }
  }
}

void EditorScreen::deleteString()
{
/*  if (_cursorOffset < _promptLine.length())
  {
    _promptLine.removeAt(_promptLine.stringIndexAtOffset(_cursorOffset));
    feedScreen();
    emit promptLineChanged();
  }

  restartBlink();*/
}

void EditorScreen::moveCursor(int newLineIndex, int newOffset)
{
  int line = 0;
  int col = 0;

  // Must scroll up?
  if (newLineIndex < _topLineIndex || (newLineIndex == _topLineIndex && newOffset / 16 < _topLineSubIndex))
    scrollUp();

  for (int index = _topLineIndex; index < newLineIndex; ++index)
  {
    const ShellLine &shellLine = _lines[index];
    line += shellLine.rowCount();
    if (index == _topLineIndex)
      line -= _topLineSubIndex;
  }

  line += newOffset / 16;

  if (newLineIndex == _topLineIndex)
    line -= _topLineSubIndex;

  col = newOffset % 16;

  // Must scroll down?
  if (line > 7)
  {
    scrollDown();
    line = 7;
  }

  TextScreen::moveCursor(col, line);

  _cursorLineIndex = newLineIndex;
  _cursorOffset = newOffset;
}

void EditorScreen::scrollUp()
{
  bool done = true;

  if (_topLineSubIndex)
    _topLineSubIndex--;
  else if (_topLineIndex)
    _topLineIndex--;
  else
    done = false;

  if (done)
  {
    feedScreen();
    emit screenChanged();
  }
}

void EditorScreen::scrollDown()
{
  if (!_lines.count())
    return;

  ShellLine &lastLine = _lines[_lines.count() - 1];
  // Compute the number of *real* screen lines between <_topLineIndex> and <lastLine>
  int linesCount = 0;
  for (int index = _topLineIndex; index < _lines.count(); ++index)
    linesCount += _lines[index].rowCount();
  linesCount -= _topLineSubIndex;

  if (linesCount <= 8)
    return;

  ShellLine &topLine = _lines[_topLineIndex];
  if (_topLineSubIndex < topLine.rowCount() - 1)
    _topLineSubIndex++;
  else
  {
    _topLineIndex++;
    _topLineSubIndex = 0;
  }

  feedScreen();
  emit screenChanged();
}
