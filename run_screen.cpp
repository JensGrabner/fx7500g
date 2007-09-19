#include "expression_computer.h"

#include "run_screen.h"

void RunScreen::buttonClicked(int button)
{
  int entity = _calcState->printableEntityByButton(button);

  if (entity >= 0 && _waitingMode)
  {
    _lines << TextLine();
    setWaitingMode(false);
    moveCursor(_editZoneTopLineIndex, 0);
  }

  EditorScreen::buttonClicked(button);

  if (entity >= 0) // Printable entity
  {
    // Wake up the cursor if needed
    if (_calcState->screenMode() != ScreenMode_Normal)
    {
      _calcState->setScreenMode(ScreenMode_Normal);
      restartBlink();
    }
  }

  switch (button)
  {
  case Button_Exe:
    switch (_calcState->keyMode())
    {
    case KeyMode_Shift:
    case KeyMode_ShiftMode:
    case KeyMode_ShiftHyp: carriageReturn(); break;
    default: validate(); break;
    }
    break;
  case Button_Left: if (_waitingMode) displayLastProgram(); break;
  case Button_Right: if (_waitingMode) displayLastProgram(true); break;
  default:;
  }
}

void RunScreen::validate()
{
  QList<int> result;

  if (!_waitingMode)
  {
    // Save last program bloc
    _lastProgram.clear();
    for (int lineIndex = _editZoneTopLineIndex; lineIndex < _lines.count(); ++lineIndex)
      _lastProgram << _lines[lineIndex];
  }

  // Compute the program
  if (_lastProgram.count())
  {
    TextLine &textLine = _lastProgram[_lastProgram.count() - 1];
    ExpressionComputer::Error error;
    result = ExpressionComputer::compute(textLine, error);
  }

  // Display it
  TextLine line("", true);
  line << result;
  _lines << line;
  moveCursor(_lines.count() - 1, 0);

  _editZoneTopLineIndex = _lines.count();
  setWaitingMode(true);

  feedScreen();
  emit screenChanged();
}

void RunScreen::displayLastProgram(bool cursorOnTop)
{
  _lines = _lastProgram;
  _editZoneTopLineIndex = 0;
  initTopLineIndex();
  setWaitingMode(false);
  if (cursorOnTop || !_lines.count())
    moveCursor(0, 0);
  else
    moveCursor(_lines.count() - 1, _lines[_lines.count() - 1].charLength());
  feedScreen();
  emit screenChanged();
  restartBlink();
}

void RunScreen::setWaitingMode(bool value)
{
  if (value == _waitingMode)
    return;

  _waitingMode = value;

  setCursorVisible(!value);
}
