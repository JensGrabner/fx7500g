#include "expression_computer.h"

#include "run_screen.h"

void RunScreen::buttonClicked(int button)
{
  int entity = _calcState->printableEntityByButton(button);

  if (entity >= 0 && !cursorVisible())
  {
    _lines << TextLine();
    setCursorVisible(true);
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
    case KeyMode_ShiftHyp:
      carriageReturn(); break;
    default:
      validate(); break;
    }
    break;
  case Button_Left: if (!cursorVisible()) displayLastProgram(); break;
  case Button_Right: if (!cursorVisible()) displayLastProgram(true); break;
  default:;
  }
}

void RunScreen::validate()
{
  QList<int> result;

  // Save last program bloc
  _lastProgram.clear();
  for (int lineIndex = _editZoneTopLineIndex; lineIndex < _lines.count(); ++lineIndex)
    _lastProgram << _lines[lineIndex];

  // Compute the program
  if (_lines.count())
  {
    TextLine &textLine = _lines[_lines.count() - 1];
    ExpressionComputer::Error error;
    result = ExpressionComputer::compute(textLine, error);
  }

  // Display it
  TextLine line("", true);
  line << result;
  _lines << line;
  moveCursor(_lines.count() - 1, 0);

  _editZoneTopLineIndex = _lines.count();
  setCursorVisible(false);

  feedScreen();
  emit screenChanged();
}

void RunScreen::displayLastProgram(bool cursorOnTop)
{
  _lines = _lastProgram;
  _editZoneTopLineIndex = 0;
  feedScreen();
  emit screenChanged();
  setCursorVisible(true);
  if (cursorOnTop || !_lines.count())
    moveCursor(0, 0);
  else
    moveCursor(_lines.count() - 1, _lines[_lines.count() - 1].charLength());
  restartBlink();
}
