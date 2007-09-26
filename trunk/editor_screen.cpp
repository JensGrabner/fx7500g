#include "memory.h"

#include "editor_screen.h"

EditorScreen::EditorScreen() :
  _editZoneTopLineIndex(0),
  _topLineIndex(0),
  _topLineSubIndex(0),
  _cursorLineIndex(0),
  _cursorOffset(0)
{
}

void EditorScreen::init()
{
  TextScreen::init();
  connect(&CalculatorState::instance(), SIGNAL(keyModeChanged(KeyMode)), this, SLOT(keyModeChanged(KeyMode)));
}

void EditorScreen::setProgram(int programIndex)
{
  _lines.clear();
  Program &program = Memory::instance().programAt(programIndex);

  _lines = program.steps();
  _editZoneTopLineIndex = 0;
  _topLineIndex = 0;
  _topLineSubIndex = 0;
  _cursorLineIndex = 0;
  _cursorOffset = 0;

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
    const TextLine &textLine = _lines[lineIndex];
    int startOffset = 0;
    if (lineIndex == _topLineIndex)
      startOffset = _topLineSubIndex * 16;
    int col = 0;
    QList<LCDChar> charLine = textLine.charLine();
    if (textLine.rightJustified() && charLine.count() <= 15)
      col = 16 - charLine.count();
    for (int offset = startOffset; offset < charLine.count(); ++offset)
    {
      if (col > 15)
      {
        line++;
        col = 0;
      }
      if (line > 7)
        break;
      _screen[col][line] = charLine[offset];
      col++;
    }

    // For empty end line in multi-lines cases
    if (charLine.count() && !(charLine.count() % 16) && _topLineSubIndex < textLine.rowCount() - 1)
      line++;

    line++;
  }
}

void EditorScreen::writeEntity(int entity)
{
  bool breaker = entity == (int) LCDChar_RBTriangle;
  int newCursorLineIndex = breaker ? _cursorLineIndex + 1 : _cursorLineIndex;
  int newCursorOffset = breaker ? 0 : _cursorOffset + entityToChars((LCDOperator) entity).count();
  if (!_lines.count())
  {
    _lines << TextLine(entity);
    if (breaker)
      _lines << TextLine();
  } else
  {
    TextLine &textLine = _lines[_cursorLineIndex];

    if (_cursorOffset >= textLine.charLength())
    {
      textLine << entity;

      // Append the next line if new char is not a breaker and we aren't in insert mode
      if (!breaker && !_insertMode && _cursorLineIndex < _lines.count() - 1)
      {
        textLine << _lines[_cursorLineIndex + 1];
        _lines.removeAt(_cursorLineIndex + 1);
      } else if (breaker)
        _lines << TextLine();
    }
    else
    {
      // Get LCDString under the cursor
      int index = textLine.entityAt(_cursorOffset);
      if (index >= 0)
      {
        if (_insertMode)
          textLine.insert(index, entity);
        else
          textLine[index] = entity;
      }
      if (breaker && _cursorOffset < textLine.charLength() - 1)
      {
        // Insert a new line
        TextLine newLine;
        for (int i = index + 1; i < textLine.count(); ++i)
          newLine << textLine[i];
        _lines.insert(_cursorLineIndex + 1, newLine);
        // Remove the last entities in <textLine>
        int newCount = textLine.count() - index - 1;
        for (int i = 0; i < newCount; ++i)
          textLine.removeLast();
      }
    }
  }

  feedScreen();

  emit screenChanged();
  moveCursor(newCursorLineIndex, newCursorOffset);
  restartBlink();
}

void EditorScreen::moveLeft()
{
  // Re-init insert mode
  _insertMode = false;
  _cursorMode = getCursorMode();

  if (_cursorLineIndex == _editZoneTopLineIndex && !_cursorOffset)
  {
    restartBlink();
    return;
  }

  const TextLine &textLine = _lines[_cursorLineIndex];
  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  if (!_cursorOffset)
  {
    newCursorLineIndex--;
    newCursorOffset = _lines[newCursorLineIndex].charLength();
    if (_lines[newCursorLineIndex].isBreakerEndedLine())
      newCursorOffset--;
  } else if (textLine.charLength() && _cursorOffset >= textLine.charLength())
    newCursorOffset = textLine.offsetAt(textLine.count() - 1);
  else
    newCursorOffset = textLine.offsetAt(textLine.entityAt(_cursorOffset) - 1);

  moveCursor(newCursorLineIndex, newCursorOffset);
}

void EditorScreen::moveRight()
{
  // Re-init insert mode
  _insertMode = false;
  _cursorMode = getCursorMode();

  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  const TextLine &textLine = _lines[_cursorLineIndex];

  if (_cursorLineIndex >= _lines.count() - 1 && !textLine.cursorCanMoveRight(_cursorOffset))
  {
    restartBlink();
    return;
  }

  int newCursorLineIndex = _cursorLineIndex;
  int newCursorOffset;
  if (!textLine.cursorCanMoveRight(_cursorOffset))
  {
    newCursorLineIndex++;
    newCursorOffset = 0;
  } else
    newCursorOffset = textLine.offsetAt(textLine.entityAt(_cursorOffset) + 1);

  moveCursor(newCursorLineIndex, newCursorOffset);
}

void EditorScreen::moveUp()
{
  // Re-init insert mode
  _insertMode = false;
  _cursorMode = getCursorMode();

  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  const TextLine &textLine = _lines[_cursorLineIndex];

  if (_cursorOffset < 16)
  {
    // Previous line?
    if (_cursorLineIndex > _editZoneTopLineIndex)
    {
      TextLine &previousLine = _lines[_cursorLineIndex - 1];
      int previousOffset = (previousLine.rowCount() - 1) * 16 + _cursorOffset;
      if (!previousLine.cursorCanMoveRight(previousOffset))
        moveCursor(_cursorLineIndex - 1, previousLine.maximumCursorPosition());
      else
        moveCursor(_cursorLineIndex - 1, previousLine.offsetAt(previousLine.entityAt(previousOffset)));
    } else
      moveCursor(_cursorLineIndex, 0);
  } else
  {
    int upIndex = textLine.entityAt(_cursorOffset - 16);
    moveCursor(_cursorLineIndex, textLine.offsetAt(upIndex));
  }
}

void EditorScreen::moveDown()
{
  // Re-init insert mode
  _insertMode = false;
  _cursorMode = getCursorMode();

  if (!_lines.count())
  {
    restartBlink();
    return;
  }

  const TextLine &textLine = _lines[_cursorLineIndex];

  if (_cursorOffset + 16 < textLine.charLength())
  {
    int downIndex = textLine.entityAt(_cursorOffset + 16);
    int downOffset = textLine.offsetAt(downIndex);
    if (downOffset == _cursorOffset + 16)
      moveCursor(_cursorLineIndex, _cursorOffset + 16);
    else
      moveCursor(_cursorLineIndex, textLine.offsetAt(downIndex + 1));
  } else if (_cursorOffset + 16 == textLine.charLength())
    moveCursor(_cursorLineIndex, textLine.maximumCursorPosition());
  else
  {
    if (_cursorLineIndex == _lines.count() - 1 || _cursorOffset / 16 < textLine.rowCount() - 1)
      moveCursor(_cursorLineIndex, textLine.maximumCursorPosition());
    else
    {
      // To the next line
      TextLine &nextLine = _lines[_cursorLineIndex + 1];
      int nextOffset = _cursorOffset % 16;

      int downIndex = nextLine.entityAt(nextOffset);
      int downOffset = nextLine.offsetAt(downIndex);
      if (downOffset == nextOffset)
        moveCursor(_cursorLineIndex + 1, nextLine.maximumCursorPositionIfTooHigh(nextOffset));
      else
        moveCursor(_cursorLineIndex + 1,
                   nextLine.maximumCursorPositionIfTooHigh(nextLine.offsetAt(downIndex + 1)));
    }
  }
}

void EditorScreen::deleteString()
{
  if (_lines.count())
  {
    TextLine &textLine = _lines[_cursorLineIndex];
    if (textLine.cursorCanMoveRight(_cursorOffset))
    {
      textLine.removeAt(textLine.entityAt(_cursorOffset));
      feedScreen();
      emit screenChanged();
    } else if (_cursorLineIndex < _lines.count() - 1)
    {
      if (textLine.isBreakerEndedLine())
        textLine.removeLast();

      // Stick the next line to the previous one
      const TextLine &nextLine = _lines[_cursorLineIndex + 1];
      textLine << nextLine;

      _lines.removeAt(_cursorLineIndex + 1);
      feedScreen();
      emit screenChanged();
    }
  }
  restartBlink();
}

void EditorScreen::moveCursor(int newLineIndex, int newOffset, bool *scrolled)
{
  int line = 0;
  int col = 0;

  bool done = false;
  while (newLineIndex < _topLineIndex || (newLineIndex == _topLineIndex && newOffset / 16 < _topLineSubIndex))
    if (scrollUp())
      done = true;
    else
      break;

  if (done)
  {
    feedScreen();
    emit screenChanged();
    if (scrolled)
      *scrolled = true;
  }

  for (int index = _topLineIndex; index < newLineIndex; ++index)
  {
    const TextLine &textLine = _lines[index];
    line += textLine.rowCount();
    if (index == _topLineIndex)
      line -= _topLineSubIndex;
  }

  line += newOffset / 16;

  if (newLineIndex == _topLineIndex)
    line -= _topLineSubIndex;

  col = newOffset % 16;

  // Must scroll down?
  done = false;
  for (int i = 0; i < line - 7; i++)
    if (scrollDown())
      done = true;
    else
      break;

  if (done)
  {
    line = 7;
    if (scrolled)
      *scrolled = true;
    feedScreen();
    emit screenChanged();
  }

  TextScreen::moveCursor(col, line);

  _cursorLineIndex = newLineIndex;
  _cursorOffset = newOffset;
}

bool EditorScreen::scrollUp()
{
  if (_topLineSubIndex)
    _topLineSubIndex--;
  else if (_topLineIndex)
    _topLineIndex--;
  else
    return false;

  return true;
}

bool EditorScreen::scrollDown()
{
  if (!_lines.count())
    return false;

  // Compute the number of *real* screen lines between <_topLineIndex> and <lastLine>
  int linesCount = 0;
  for (int index = _topLineIndex; index < _lines.count(); ++index)
    linesCount += _lines[index].rowCount();
  linesCount -= _topLineSubIndex;

  if (linesCount <= 8)
    return false;

  TextLine &topLine = _lines[_topLineIndex];
  if (_topLineSubIndex < topLine.rowCount() - 1)
    _topLineSubIndex++;
  else
  {
    _topLineIndex++;
    _topLineSubIndex = 0;
  }
  return true;
}

void EditorScreen::buttonClicked(int button)
{
  int entity = CalculatorState::instance().printableEntityByButton(button);

  if (entity >= 0) // Printable entity
    writeEntity(entity);
  else if (cursorVisible())
    switch (button)
    {
    case Button_Up: moveUp(); break;
    case Button_Down: moveDown(); break;
    case Button_Left: moveLeft(); break;
    case Button_Right: moveRight(); break;
    case Button_Del: deleteString(); break;
    case Button_Ins: insertClicked(); break;
    default:;
    }
}

void EditorScreen::keyModeChanged(KeyMode)
{
  setCursorMode(getCursorMode());

  restartBlink();
}

void EditorScreen::carriageReturn()
{
  bool insertion = false;
  if (_insertMode)
  {
    insertion = true;
    if (!_lines.count())
      _lines << TextLine();
    else
    {
      TextLine &textLine = _lines[_cursorLineIndex];
      if (_cursorOffset < textLine.charLength())
      {
        TextLine newTextLine;
        int toRemove = 0;
        int strIndex = textLine.entityAt(_cursorOffset);
        for (int i = strIndex; i < textLine.count(); ++i)
        {
          newTextLine << textLine[i];
          toRemove++;
        }
        _lines.insert(_cursorLineIndex + 1, newTextLine);

        while (toRemove)
        {
          textLine.removeLast();
          toRemove--;
        }
      } else if (_cursorLineIndex < _lines.count() - 1)
        _lines.insert(_cursorLineIndex + 1, TextLine());
      else
        _lines << TextLine();
    }
  } else if (_cursorLineIndex >= _lines.count() - 1)
  {
    _lines << TextLine();
    insertion = true;
  }

  if (insertion)
    feedScreen();

  bool scrolled;
  moveCursor(_cursorLineIndex + 1, 0, &scrolled); // Go to the next line
  if (scrolled || insertion)
    emit screenChanged();
  restartBlink();
}

void EditorScreen::insertClicked()
{
  if (_insertMode) // We deactivate insert mode
  {
    _insertMode = false;
    _cursorMode = getCursorMode();
  } else
  {
    // No insert mode when we are at the end of the text
    if (_lines.count() && (_cursorLineIndex < _lines.count() - 1 || _cursorOffset < _lines[_cursorLineIndex].charLength()))
    {
      _insertMode = true;
      _cursorMode = getCursorMode();
    }
  }

  // Annihilate all key mode BUT ShiftAlpha
  if (CalculatorState::instance().keyMode() != KeyMode_ShiftAlpha)
    CalculatorState::instance().setKeyMode(KeyMode_Normal);

  restartBlink();
}

void EditorScreen::initTopLineIndex()
{
  _editZoneTopLineIndex = 0;
  _topLineIndex = 0;
  _topLineSubIndex = 0;

  feedScreen();
  emit screenChanged();
}

void EditorScreen::clearLines()
{
  _lines.clear();
  initTopLineIndex();
  moveCursor(0, 0);
}
