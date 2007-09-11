#include "program.h"

#include "editor_screen.h"

EditorScreen::EditorScreen() :
  _topLineIndex(0),
  _topLineSubIndex(0),
  _cursorLineIndex(0),
  _cursorOffset(0)
{
}

void EditorScreen::init(const CalculatorState *calcState)
{
  TextScreen::init(calcState);
  connect(calcState, SIGNAL(keyModeChanged(KeyMode)), this, SLOT(keyModeChanged(KeyMode)));
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
    const ShellLine &shellLine = _lines[lineIndex];
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

void EditorScreen::writeEntity(int entity)
{
  LCDString lcdStr(entity);
  bool breaker = entity == (int) LCDChar_RBTriangle;
  int newCursorLineIndex = breaker ? _cursorLineIndex + 1 : _cursorLineIndex;
  int newCursorOffset = breaker ? 0 : _cursorOffset + lcdStr.count();
  if (!_lines.count())
  {
    _lines << ShellLine(lcdStr);
    if (breaker)
      _lines << ShellLine();
  } else
  {
    ShellLine &shellLine = _lines[_cursorLineIndex];

    if (_cursorOffset >= shellLine.length())
    {
      shellLine << lcdStr;

      // Append the next line if new char is not a breaker
      if (!breaker && _cursorLineIndex < _lines.count() - 1)
      {
        shellLine << _lines[_cursorLineIndex + 1];
        _lines.removeAt(_cursorLineIndex + 1);
      } else if (breaker)
        _lines << ShellLine();
    }
    else
    {
      // Get LCDString under the cursor
      int index = shellLine.stringIndexAtOffset(_cursorOffset);
      if (index >= 0)
        shellLine[index] = lcdStr;
      if (breaker && _cursorOffset < shellLine.length() - 1)
      {
        // Insert a new line
        ShellLine newLine;
        for (int i = index + 1; i < shellLine.count(); ++i)
          newLine << shellLine[i];
        _lines.insert(_cursorLineIndex + 1, newLine);
        // Remove the last string in <shellLine>
        int newCount = shellLine.count() - index - 1;
        for (int i = 0; i < newCount; ++i)
          shellLine.removeLast();
      }
    }
  }

  feedScreen();

  emit screenChanged();
  moveCursor(newCursorLineIndex, newCursorOffset);
  restartBlink();
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
  case Qt::Key_A: write(LCDChar_A); break;
  case Qt::Key_B: write(LCDChar_B); break;
  case Qt::Key_C: write(LCDOp_Log); break;
  case Qt::Key_D: write(LCDOp_YonMinusOne); break;
  case Qt::Key_Z: write(LCDChar_RBTriangle); break;
  }
}

void EditorScreen::moveLeft()
{
  if (!_cursorLineIndex && !_cursorOffset)
  {
    restartBlink();
    return;
  }

  const ShellLine &shellLine = _lines[_cursorLineIndex];
  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  if (!_cursorOffset)
  {
    newCursorLineIndex--;
    newCursorOffset = _lines[newCursorLineIndex].length();
    if (_lines[newCursorLineIndex].isBreakerEndedLine())
      newCursorOffset--;
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

  const ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorLineIndex >= _lines.count() - 1 && !shellLine.cursorCanMoveRight(_cursorOffset))
  {
    restartBlink();
    return;
  }

  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  if (!shellLine.cursorCanMoveRight(_cursorOffset))
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

  const ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorOffset < 16)
  {
    // Previous line?
    if (_cursorLineIndex)
    {
      ShellLine &previousLine = _lines[_cursorLineIndex - 1];
      int previousOffset = (previousLine.rowCount() - 1) * 16 + _cursorOffset;
      if (!previousLine.cursorCanMoveRight(previousOffset))
        moveCursor(_cursorLineIndex - 1, previousLine.maximumCursorPosition());
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

  const ShellLine &shellLine = _lines[_cursorLineIndex];

  if (_cursorOffset + 16 < shellLine.length())
  {
    int downIndex = shellLine.stringIndexAtOffset(_cursorOffset + 16);
    int downOffset = shellLine.offsetByStringIndex(downIndex);
    if (downOffset == _cursorOffset + 16)
      moveCursor(_cursorLineIndex, _cursorOffset + 16);
    else
      moveCursor(_cursorLineIndex, shellLine.offsetByStringIndex(downIndex + 1));
  } else if (_cursorOffset + 16 == shellLine.length())
    moveCursor(_cursorLineIndex, shellLine.maximumCursorPosition());
  else
  {
    if (_cursorLineIndex == _lines.count() - 1 || _cursorOffset / 16 < shellLine.rowCount() - 1)
      moveCursor(_cursorLineIndex, shellLine.maximumCursorPosition());
    else
    {
      // To the next line
      ShellLine &nextLine = _lines[_cursorLineIndex + 1];
      int nextOffset = _cursorOffset % 16;

      int downIndex = nextLine.stringIndexAtOffset(nextOffset);
      int downOffset = nextLine.offsetByStringIndex(downIndex);
      if (downOffset == nextOffset)
        moveCursor(_cursorLineIndex + 1, nextLine.maximumCursorPositionIfTooHigh(nextOffset));
      else
        moveCursor(_cursorLineIndex + 1,
                   nextLine.maximumCursorPositionIfTooHigh(nextLine.offsetByStringIndex(downIndex + 1)));
    }
  }
}

void EditorScreen::deleteString()
{
  if (_lines.count())
  {
    ShellLine &shellLine = _lines[_cursorLineIndex];
    if (shellLine.cursorCanMoveRight(_cursorOffset))
    {
      shellLine.removeAt(shellLine.stringIndexAtOffset(_cursorOffset));
      feedScreen();
      emit screenChanged();
    } else if (_cursorLineIndex < _lines.count() - 1)
    {
      if (shellLine.isBreakerEndedLine())
        shellLine.removeLast();

      // Stick the next line to the previous one
      const ShellLine &nextLine = _lines[_cursorLineIndex + 1];
      shellLine << nextLine;

      _lines.removeAt(_cursorLineIndex + 1);
      feedScreen();
      emit screenChanged();
    }
  }
  restartBlink();
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

void EditorScreen::buttonClicked(int button)
{
  int entity = _calcState->printableEntityByButton(button);

  if (entity >= 0) // Printable entity
    writeEntity(entity);
}

void EditorScreen::keyModeChanged(KeyMode oldMode)
{
  switch (_calcState->keyMode())
  {
  case KeyMode_Normal: setCursorMode(CursorMode_Normal); break;
  case KeyMode_Shift: setCursorMode(CursorMode_Shift); break;
  case KeyMode_Alpha: setCursorMode(CursorMode_CapsLock); break;
  case KeyMode_Mode: setCursorMode(CursorMode_Normal); break;
  case KeyMode_ShiftMode: setCursorMode(CursorMode_Shift); break;
  case KeyMode_ShiftAlpha: setCursorMode(CursorMode_CapsLock); break;
  case KeyMode_Hyp: setCursorMode(CursorMode_Normal); break;
  case KeyMode_ShiftHyp: setCursorMode(CursorMode_Shift); break;
  default:;
  }

  restartBlink();
}
