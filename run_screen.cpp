#include "expression_computer.h"

#include "run_screen.h"

void RunScreen::buttonClicked(int button)
{
  int entity = _calcState->printableEntityByButton(button);

  if (entity >= 0 && _waitingMode && !_errorMode)
  {
    _lines << TextLine();
    setWaitingMode(false);
    moveCursor(_editZoneTopLineIndex, 0);
  }

  if (!_errorMode)
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
    if (_errorMode)
      break;
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
  QList<TextLine> result;

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
    int errorStep;
    QList<int> res = ExpressionComputer::compute(textLine, error, errorStep);
    _errorMode = true;
    _lastErrorLine = 0;
    _lastErrorStep = errorStep;
    switch (error)
    {
    case ExpressionComputer::Error_No: _lines << TextLine(res, true); _errorMode = false; break;
    case ExpressionComputer::Error_Syntax: _lines << syntaxError(errorStep); break;
    case ExpressionComputer::Error_Stack: _lines << stackError(errorStep); break;
    case ExpressionComputer::Error_Memory: _lines << memError(errorStep); break;
    default:;
    }
  }

  // Move the cursor for the waiting mode
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
  if (_errorMode) // Move to the last error
    moveCursor(_lastErrorLine, _lastErrorStep);
  else if (cursorOnTop || !_lines.count())
    moveCursor(0, 0);
  else
    moveCursor(_lines.count() - 1, _lines[_lines.count() - 1].charLength());
  feedScreen();
  emit screenChanged();
  restartBlink();
  _errorMode = false;
}

void RunScreen::setWaitingMode(bool value)
{
  if (value == _waitingMode)
    return;

  _waitingMode = value;

  setCursorVisible(!value);
}

QList<TextLine> RunScreen::syntaxError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Syn ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> RunScreen::stackError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Stk ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}

QList<TextLine> RunScreen::memError(int step) const
{
  QList<TextLine> result;
  result << TextLine("  Mem ERROR");
  result << TextLine(QString("   Step    %1").arg(step));
  return result;
}
