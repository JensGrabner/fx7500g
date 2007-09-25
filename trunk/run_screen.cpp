#include "run_screen.h"

RunScreen::RunScreen() :
  EditorScreen(),
  _waitingMode(false),
  _errorMode(false)
{
  connect(&_interpreter, SIGNAL(displayLine()), this, SLOT(interpreterDisplayLine()), Qt::QueuedConnection);
  connect(&_interpreter, SIGNAL(finished()), this, SLOT(interpreterFinished()));

  _timerDisplay.setInterval(10);
  connect(&_timerDisplay, SIGNAL(timeout()), this, SLOT(timerDisplayTimeout()));
}

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
  if (_interpreter.waitForInput())
    _interpreter.sendInput(_lines[_lines.count() - 1]);
  else
  {
    QList<TextLine> result;

    if (!_waitingMode)
    {
      // Save last program bloc
      _lastProgram.clear();
      for (int lineIndex = _editZoneTopLineIndex; lineIndex < _lines.count(); ++lineIndex)
        _lastProgram << _lines[lineIndex];
    }

    if (!_lastProgram.count() ||
        (_lastProgram.count() == 1 && !_lastProgram[0].count()))
    {
      restartBlink();
      return;
    }

    // Compute the program
    if (_lastProgram.count())
    {
      _interpreter.setProgram(_lastProgram);
      _interpreter.start();
    }
  }
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

void RunScreen::interpreterDisplayLine()
{
  _lines << _interpreter.getNextDisplayLine();

  moveCursor(_lines.count() - 1, 0);

  _editZoneTopLineIndex = _lines.count();
  setWaitingMode(true);

  feedScreen();

  emit screenChanged();
}

void RunScreen::getLineAndStep(const QList<TextLine> &program, int offset, int &line, int &step) const
{
  TextLine rawTextLine;
  rawTextLine.affect(program);
  line = 0;
  step = 0;
  int index = 0;
  for (int i = 0; i <= offset && i < rawTextLine.count(); ++i)
    if (rawTextLine[i] == LCDChar_CR || rawTextLine[i] == LCDChar_RBTriangle)
    {
      line++;
      index = i + 1;
    }

  step = program[line].offsetAt(offset - index);
}

void RunScreen::timerDisplayTimeout()
{
}

void RunScreen::interpreterFinished()
{
  if (_interpreter.error())
  {
    _errorMode = true;
    getLineAndStep(_lastProgram, _interpreter.errorStep(), _lastErrorLine, _lastErrorStep);
  }

  // Move the cursor for the waiting mode
  moveCursor(_lines.count() - 1, 0);

  _editZoneTopLineIndex = _lines.count();
  setWaitingMode(true);

  feedScreen();
  emit screenChanged();
}
