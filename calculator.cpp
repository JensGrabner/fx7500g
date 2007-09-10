#include "calculator.h"

Calculator::Calculator() :
  _screenMode(ScreenMode_Resume),
  _lcdDisplay(0)
{
  // Init run screen
  _runScreen.init(&_calcState);
  connect(&_runScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(shellChangeChar(int, int, LCDChar)));
  connect(&_runScreen, SIGNAL(promptLineChanged()),
          this, SLOT(shellPromptLineChanged()));

  // Init prog screen
  _progScreen.init(&_calcState);
  connect(&_progScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(progChangeChar(int, int, LCDChar)));
  connect(&_progScreen, SIGNAL(editProgram(int)),
          this, SLOT(progEditProgram(int)));

  // Init editor screen
  _editorScreen.init(&_calcState);
  connect(&_editorScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(editorChangeChar(int, int, LCDChar)));
  connect(&_editorScreen, SIGNAL(screenChanged()),
          this, SLOT(editorScreenChanged()));
}

void Calculator::setAngleMode(AngleMode value)
{
  _calcState.setAngleMode(value);
}

void Calculator::shellChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _screenMode == ScreenMode_Normal &&
      _calcState.sysMode() == SysMode_RUN)
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::progChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _screenMode == ScreenMode_Normal &&
      (_calcState.sysMode() == SysMode_WRT || _calcState.sysMode() == SysMode_PCL))
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::setDisplayMode(DisplayMode value)
{
  _calcState.setDisplayMode(value);
}

void Calculator::shellPromptLineChanged()
{
  _lcdDisplay->drawScreen(_runScreen.currentScreen());
}

QList<LCDString> Calculator::getResumeScreen() const
{
  QList<LCDString> screen;

  screen << LCDString(" **** MODE ****");
  screen << LCDString("");
  screen << LCDString(QString("sys mode : %1").arg(_calcState.sysModeString()));
  if (_calcState.calMode() == CalMode_BASE_N)
  {
    screen << LCDString("cal mode :BASE-N");
    screen << LCDString(QString("            %1").arg(_calcState.baseModeString()));
    screen << LCDString("");
  } else
  {
    screen << LCDString(QString("cal mode : %1").arg(_calcState.calModeString()));
    screen << LCDString(QString("   angle : %1").arg(_calcState.angleModeString()));
    screen << LCDString(QString(" display : %1").arg(_calcState.displayModeString()));
  }
  screen << LCDString("");
  screen << LCDString("   Step    0");

  return screen;
}

void Calculator::init()
{
  Q_ASSERT_X(_lcdDisplay, "Calculator::init()", "<_lcdDisplay> is 0!");

  setScreenMode(ScreenMode_Resume);
  _lcdDisplay->drawScreen(getResumeScreen());
}

void Calculator::setScreenMode(ScreenMode value)
{
  if (value == _screenMode)
    return;

  _screenMode = value;

  switch (_screenMode)
  {
  case ScreenMode_Normal:
    switch (_calcState.sysMode())
    {
    case SysMode_RUN:
      _lcdDisplay->drawScreen(_runScreen.currentScreen());
      _runScreen.restartBlink();
      break;
    case SysMode_WRT:
    case SysMode_PCL:
      _lcdDisplay->drawScreen(_progScreen.currentScreen());
      _progScreen.restartBlink();
      break;
    default:;
    }
    break;
  case ScreenMode_Editor:
    switch (_calcState.sysMode())
    {
    case SysMode_WRT:
      _lcdDisplay->drawScreen(_editorScreen.currentScreen());
      _editorScreen.restartBlink();
      break;
    default:;
    }
  default:;
  }
}

void Calculator::setCalMode(CalMode value)
{
  _calcState.setCalMode(value);
}

void Calculator::setBaseMode(BaseMode value)
{
  _calcState.setBaseMode(value);
}

void Calculator::setSysMode(SysMode value)
{
  _calcState.setSysMode(value);

  switch (_calcState.sysMode())
  {
  case SysMode_RUN:
    _lcdDisplay->drawScreen(_runScreen.currentScreen());
    _runScreen.restartBlink();
    break;
  case SysMode_WRT:
  case SysMode_PCL:
    _lcdDisplay->drawScreen(_progScreen.currentScreen());
    _progScreen.restartBlink();
    break;
  default:;
  }
}

void Calculator::applyKey(int key)
{
  switch (_calcState.sysMode())
  {
  case SysMode_RUN:
    _runScreen.applyKey(key);
    break;
  case SysMode_WRT:
  case SysMode_PCL:
    if (_screenMode == ScreenMode_Normal)
      _progScreen.applyKey(key);
    else if (_screenMode == ScreenMode_Editor)
      _editorScreen.applyKey(key);
    break;
  default:;
  }
}

void Calculator::editorChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _screenMode == ScreenMode_Editor && _calcState.sysMode() == SysMode_WRT)
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::progEditProgram(int programIndex)
{
  _editorScreen.setProgram(programIndex);
  setScreenMode(ScreenMode_Editor);
}

void Calculator::editorScreenChanged()
{
  _lcdDisplay->drawScreen(_editorScreen.currentScreen());
}

void Calculator::buttonClicked(int padIndex, int buttonIndex)
{
  qDebug("Pad: %d, buttonIndex: %d", padIndex, buttonIndex);
}
