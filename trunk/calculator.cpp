#include "calculator.h"

Calculator::Calculator() :
  _screenMode(ScreenMode_Resume),
  _sysMode(SysMode_RUN),
  _angleMode(Deg),
  _calMode(CalMode_COMP),
  _baseMode(BaseMode_Dec),
  _displayMode(DisplayMode_Norm),
  _screen(0)
{
  connect(&_shell, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(shellChangeChar(int, int, LCDChar)));
  connect(&_shell, SIGNAL(promptLineChanged()),
          this, SLOT(shellPromptLineChanged()));
}

void Calculator::setAngleMode(AngleMode value)
{
  _angleMode = value;
}

void Calculator::test()
{
  if (_screen)
  {
    _screen->drawScreen(_shell.currentScreen());
    _shell.restartBlink();
  }
}

void Calculator::shellChangeChar(int col, int line, LCDChar c)
{
  if (_screen && _screenMode == ScreenMode_Normal &&
      _sysMode == SysMode_RUN)
    _screen->drawChar(c, col, line);
}

void Calculator::setDisplayMode(DisplayMode value)
{
  if (value == _displayMode)
    return;

  _displayMode = value;

  if (_screenMode == ScreenMode_Normal)
    _shell.restartBlink();
}

void Calculator::shellPromptLineChanged()
{
  _screen->drawScreen(_shell.currentScreen());
}

QList<LCDString> Calculator::getResumeScreen() const
{
  QList<LCDString> screen;

  screen << LCDString(" **** MODE ****");
  screen << LCDString("");
  screen << LCDString(QString("sys mode : %1").arg(sysModeString()));
  if (_calMode == CalMode_BASE_N)
  {
    screen << LCDString("cal mode :BASE-N");
    screen << LCDString(QString("            %1").arg(baseModeString()));
    screen << LCDString("");
  } else
  {
    screen << LCDString(QString("cal mode : %1").arg(calModeString()));
    screen << LCDString(QString("   angle : %1").arg(angleModeString()));
    screen << LCDString(QString(" display : %1").arg(displayModeString()));
  }
  screen << LCDString("");
  screen << LCDString("   Step    0");

  return screen;
}

void Calculator::init()
{
  Q_ASSERT_X(_screen, "Calculator::init()", "<_screen> is 0!");

  setScreenMode(ScreenMode_Resume);
  _screen->drawScreen(getResumeScreen());
}

void Calculator::setScreenMode(ScreenMode value)
{
  if (value == _screenMode)
    return;

  _screenMode = value;
}

void Calculator::setCalMode(CalMode value)
{
  if (value == _calMode)
    return;

  _calMode = value;
}

QString Calculator::calModeString() const
{
  switch (_calMode)
  {
  case CalMode_COMP: return "COMP";
  case CalMode_BASE_N: return "BASE-N";
  case CalMode_SD1: return "SD 1";
  case CalMode_LR1: return "LR 1";
  case CalMode_SD2: return "SD 2";
  case CalMode_LR2: return "LR 2";
  default: return "";
  }
}

void Calculator::setBaseMode(BaseMode value)
{
  if (value == _baseMode)
    return;

  _baseMode = value;
}

QString Calculator::baseModeString() const
{
  switch (_baseMode)
  {
  case BaseMode_Dec: return "Dec";
  case BaseMode_Hex: return "Hex";
  case BaseMode_Bin: return "Bin";
  case BaseMode_Oct: return "Oct";
  default: return "";
  }
}

QString Calculator::angleModeString() const
{
  switch (_angleMode)
  {
  case Deg: return "Deg";
  case Rad: return "Rad";
  case Grad: return "Gra";
  default: return "";
  }
}

QString Calculator::sysModeString() const
{
  switch (_sysMode)
  {
  case SysMode_RUN: return "RUN";
  case SysMode_WRT: return "WRT";
  case SysMode_PCL: return "PCL";
  default: return "";
  }
}

void Calculator::setSysMode(SysMode value)
{
  if (value == _sysMode)
    return;

  _sysMode = value;
}

QString Calculator::displayModeString() const
{
  switch (_displayMode)
  {
  case DisplayMode_Norm: return "Norm";
  case DisplayMode_Fix: return "Fix";
  case DisplayMode_Sci: return "Sci";
  default: return "";
  }
}
