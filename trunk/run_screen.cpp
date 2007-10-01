#include "run_screen.h"

RunScreen::RunScreen() :
  EditorScreen(),
  _waitingMode(false),
  _errorMode(false),
  _lastResult(0.0)
{
  connect(&_interpreter, SIGNAL(displayLine()), this, SLOT(interpreterDisplayLine()), Qt::QueuedConnection);
  connect(&_interpreter, SIGNAL(finished()), this, SLOT(interpreterFinished()));
  connect(&_interpreter, SIGNAL(askForValidation()), this, SLOT(interpreterAskForValidation()));

  _timerDisplay.setInterval(10);
  connect(&_timerDisplay, SIGNAL(timeout()), this, SLOT(timerDisplayTimeout()));
}

void RunScreen::buttonClicked(int button)
{
  int entity = CalculatorState::instance().printableEntityByButton(button);

  if (_interpreter.isRunning() && !_interpreter.waitForInput() && !_interpreter.waitForValidation())
    return;

  if (_interpreter.waitForValidation())
  {
    if (button == Button_Exe)
      validate();
    return;
  }

  if (entity >= 0 && _waitingMode && !_errorMode)
  {
    setWaitingMode(false);

    TextLine textLine;
    // Use <_lastResult> with some operator
    if (isOperator(entity))
    {
      textLine << formatDouble(_lastResult);
    }
    _lines << textLine;

    moveCursor(_editZoneTopLineIndex, textLine.count());
  }

  if (entity >= 0) // Printable entity
  {
    // Remove the resume mode
    if (CalculatorState::instance().screenMode() != ScreenMode_Normal)
    {
      CalculatorState::instance().setScreenMode(ScreenMode_Normal);

      // If there was something in lines, pass a new line
      if (_lines.count())
      {
        _lines << TextLine();
        _editZoneTopLineIndex = _lines.count() - 1;
        moveCursor(_editZoneTopLineIndex, 0);
        feedScreen();
        emit screenChanged();
      }

      restartBlink();
    }
  }

  if (!_errorMode)
    EditorScreen::buttonClicked(button);

  switch (button)
  {
  case Button_Exe:
    if (_errorMode)
      break;
    switch (CalculatorState::instance().keyMode())
    {
    case KeyMode_Shift:
    case KeyMode_ShiftMode:
    case KeyMode_ShiftHyp: carriageReturn(); break;
    default: validate(); break;
    }
    break;
  case Button_Left: if (_waitingMode) displayLastProgram(); break;
  case Button_Right: if (_waitingMode) displayLastProgram(true); break;
  case Button_Ac: resetScreen(); break;
  default:;
  }
}

void RunScreen::validate()
{
  setCursorVisible(false);
  if (_interpreter.waitForInput())
    _interpreter.sendInput(_lines[_lines.count() - 1]);
  else if (_interpreter.waitForValidation())
  {
    // Remove "- Disp -"
    _lines.removeLast();
    feedScreen();
    emit screenChanged();
    _interpreter.sendValidation();
  } else
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
  TextLine textLine = _interpreter.getNextDisplayLine();

  _lines << textLine;

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
  } else
    _lastResult = _interpreter.lastResult();

  // Move the cursor for the waiting mode
  moveCursor(_lines.count() - 1, 0);

  _editZoneTopLineIndex = _lines.count();
  setWaitingMode(true);

  feedScreen();
  emit screenChanged();
  if (_interpreter.displayDefm())
    emit displayDefm();
}

void RunScreen::resetScreen()
{
  clearLines();
  _waitingMode = false;
  _errorMode = false;
  setCursorVisible(true);
}

void RunScreen::interpreterAskForValidation()
{
  TextLine textLine("- Disp -");
  textLine.setRightJustified(true);
  _lines << textLine;
  feedScreen();
  emit screenChanged();
  moveCursor(_lines.count() - 1, 0); // To scroll
}
