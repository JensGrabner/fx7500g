#include "calculator.h"

Calculator::Calculator() :
  _lcdDisplay(0)
{
  // Init run screen
  _runScreen.init(&_calcState);
  connect(&_runScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(runChangeChar(int, int, LCDChar)));
  connect(&_runScreen, SIGNAL(screenChanged()),
          this, SLOT(runScreenChanged()));

  // Init prog screen
  _progScreen.init(&_calcState);
  connect(&_progScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(progChangeChar(int, int, LCDChar)));
  connect(&_progScreen, SIGNAL(editProgram(int)),
          this, SLOT(progEditProgram(int)));
  connect(&_progScreen, SIGNAL(screenChanged()),
          this, SLOT(progScreenChanged()));

  // Init editor screen
  _progEditScreen.init(&_calcState);
  connect(&_progEditScreen, SIGNAL(changeChar(int, int, LCDChar)),
          this, SLOT(editorChangeChar(int, int, LCDChar)));
  connect(&_progEditScreen, SIGNAL(screenChanged()),
          this, SLOT(editorScreenChanged()));

  // Connect to <_calcState>
  connect(&_calcState, SIGNAL(screenModeChanged(ScreenMode)),
          this, SLOT(screenModeChanged(ScreenMode)));
}

void Calculator::setAngleMode(AngleMode value)
{
  _calcState.setAngleMode(value);
}

void Calculator::runChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _calcState.screenMode() == ScreenMode_Normal &&
      _calcState.sysMode() == SysMode_RUN)
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::progChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _calcState.screenMode() == ScreenMode_Normal &&
      (_calcState.sysMode() == SysMode_WRT || _calcState.sysMode() == SysMode_PCL))
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::setDisplayMode(DisplayMode value)
{
  _calcState.setDisplayMode(value);
}

void Calculator::runScreenChanged()
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

  setSysMode(SysMode_RUN);
  _calcState.setScreenMode(ScreenMode_Resume);
  screenModeChanged(ScreenMode_Resume);

  buttonClicked(Button_Shift); buttonClicked(Button_Plot);
  buttonClicked(Button_4);
  buttonClicked(Button_Colon);
  buttonClicked(Button_4);
  buttonClicked(Button_Colon);
  buttonClicked(Button_Shift); buttonClicked(Button_Prog);
  buttonClicked(Button_4);
/*  buttonClicked(Button_1);
  buttonClicked(Button_2);
  buttonClicked(Button_Arrow);
  buttonClicked(Button_Alpha); buttonClicked(Button_MinusOne);
  buttonClicked(Button_Alpha); buttonClicked(Button_Dot);
  buttonClicked(Button_1);
  buttonClicked(Button_2);
  buttonClicked(Button_3);
  buttonClicked(Button_Alpha); buttonClicked(Button_Exp);
  buttonClicked(Button_Sin);*/
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
  switch (key)
  {
  case Qt::Key_F1: buttonClicked(Button_Shift); break;
  case Qt::Key_F2: buttonClicked(Button_Alpha); break;
  case Qt::Key_F3: buttonClicked(Button_Mode); break;
  case Qt::Key_Left: buttonClicked(Button_Left); break;
  case Qt::Key_Right: buttonClicked(Button_Right); break;
  case Qt::Key_Up: buttonClicked(Button_Up); break;
  case Qt::Key_Down: buttonClicked(Button_Down); break;
  case Qt::Key_Delete: buttonClicked(Button_Del); break;
  case Qt::Key_Return:
  case Qt::Key_Enter: buttonClicked(Button_Exe); break;
  case Qt::Key_0: buttonClicked(Button_0); break;
  case Qt::Key_1: buttonClicked(Button_1); break;
  case Qt::Key_2: buttonClicked(Button_2); break;
  case Qt::Key_3: buttonClicked(Button_3); break;
  case Qt::Key_4: buttonClicked(Button_4); break;
  case Qt::Key_5: buttonClicked(Button_5); break;
  case Qt::Key_6: buttonClicked(Button_6); break;
  case Qt::Key_7: buttonClicked(Button_7); break;
  case Qt::Key_8: buttonClicked(Button_8); break;
  case Qt::Key_9: buttonClicked(Button_9); break;
  case Qt::Key_Period: buttonClicked(Button_Dot); break;
  case Qt::Key_Insert: buttonClicked(Button_Ins); break;
  case Qt::Key_Backspace: buttonClicked(Button_Left); buttonClicked(Button_Del); break;
  case Qt::Key_Plus: buttonClicked(Button_Add); break;
  case Qt::Key_Slash: buttonClicked(Button_Divide); break;
  case Qt::Key_Asterisk: buttonClicked(Button_Multiply); break;
  case Qt::Key_ParenLeft: buttonClicked(Button_OpenParen); break;
  case Qt::Key_ParenRight: buttonClicked(Button_CloseParen); break;
  case Qt::Key_Exclam: buttonClicked(Button_Shift); buttonClicked(Button_MinusOne); break;
  case Qt::Key_BracketLeft: buttonClicked(Button_Alpha); buttonClicked(Button_Dot); break;
  case Qt::Key_BracketRight: buttonClicked(Button_Alpha); buttonClicked(Button_Exp); break;
  case Qt::Key_Colon: buttonClicked(Button_Colon); break;
  case Qt::Key_QuoteDbl: buttonClicked(Button_Alpha); buttonClicked(Button_Colon); break;
  case Qt::Key_Question: buttonClicked(Button_Question); break;
  case Qt::Key_Greater: buttonClicked(Button_Shift); buttonClicked(Button_2); break;
  case Qt::Key_Less: buttonClicked(Button_Shift); buttonClicked(Button_3); break;
  case Qt::Key_Equal: buttonClicked(Button_Shift); buttonClicked(Button_8); break;
  }

  if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key == Qt::Key_Space))
  {
    buttonClicked(Button_Alpha);
    switch (key)
    {
    case Qt::Key_A: buttonClicked(Button_MinusOne); break;
    case Qt::Key_B: buttonClicked(Button_Degree); break;
    case Qt::Key_C: buttonClicked(Button_Ten); break;
    case Qt::Key_D: buttonClicked(Button_Euler); break;
    case Qt::Key_E: buttonClicked(Button_Hyp); break;
    case Qt::Key_F: buttonClicked(Button_Sin); break;
    case Qt::Key_G: buttonClicked(Button_Cos); break;
    case Qt::Key_H: buttonClicked(Button_Tan); break;
    case Qt::Key_I: buttonClicked(Button_Question); break;
    case Qt::Key_J: buttonClicked(Button_Arrow); break;
    case Qt::Key_K: buttonClicked(Button_OpenParen); break;
    case Qt::Key_L: buttonClicked(Button_CloseParen); break;
    case Qt::Key_M: buttonClicked(Button_7); break;
    case Qt::Key_N: buttonClicked(Button_8); break;
    case Qt::Key_O: buttonClicked(Button_9); break;
    case Qt::Key_P: buttonClicked(Button_4); break;
    case Qt::Key_Q: buttonClicked(Button_5); break;
    case Qt::Key_R: buttonClicked(Button_6); break;
    case Qt::Key_S: buttonClicked(Button_Multiply); break;
    case Qt::Key_T: buttonClicked(Button_Divide); break;
    case Qt::Key_U: buttonClicked(Button_1); break;
    case Qt::Key_V: buttonClicked(Button_2); break;
    case Qt::Key_W: buttonClicked(Button_3); break;
    case Qt::Key_X: buttonClicked(Button_Add); break;
    case Qt::Key_Y: buttonClicked(Button_Substract); break;
    case Qt::Key_Z: buttonClicked(Button_0); break;
    case Qt::Key_Space: buttonClicked(Button_Ans); break;
    }
  }
}

void Calculator::editorChangeChar(int col, int line, LCDChar c)
{
  if (_lcdDisplay && _calcState.screenMode() == ScreenMode_Editor &&
      _calcState.sysMode() == SysMode_WRT)
    _lcdDisplay->drawChar(c, col, line);
}

void Calculator::progEditProgram(int programIndex)
{
  _progEditScreen.setProgram(programIndex);
  _calcState.setScreenMode(ScreenMode_Editor);
}

void Calculator::editorScreenChanged()
{
  _lcdDisplay->drawScreen(_progEditScreen.currentScreen());
}

void Calculator::buttonClicked(int buttonIndex)
{
  bool noSpecialButton;

  // Shift, Mode, Alpha stuffs
  _calcState.changeKeyModeByButton(buttonIndex, noSpecialButton);

  // Switch mode?
  if (_calcState.keyMode() == KeyMode_Mode || _calcState.keyMode() == KeyMode_ShiftMode)
  {
    switch (buttonIndex)
    {
    case Button_1:
      setSysMode(SysMode_RUN);
      _calcState.setScreenMode(ScreenMode_Normal);
      _calcState.setKeyMode(KeyMode_Normal);
      break;
    case Button_2:
      setSysMode(SysMode_WRT);
      _calcState.setScreenMode(ScreenMode_Normal);
      _calcState.setKeyMode(KeyMode_Normal);
      break;
    case Button_3:
      setSysMode(SysMode_PCL);
      _calcState.setScreenMode(ScreenMode_Normal);
      _calcState.setKeyMode(KeyMode_Normal);
      break;
    default:;
    }
  }

  // Dispatch button clicks
  switch (_calcState.sysMode())
  {
  case SysMode_RUN:
    _runScreen.buttonClicked(buttonIndex);
    break;
  case SysMode_WRT:
  case SysMode_PCL:
    if (_calcState.screenMode() == ScreenMode_Normal)
      _progScreen.buttonClicked(buttonIndex);
    else if (_calcState.screenMode() == ScreenMode_Editor)
      _progEditScreen.buttonClicked(buttonIndex);
    break;
  default:;
  }

  // Restore key normal mode?
  if (noSpecialButton)
    switch (_calcState.keyMode())
    {
    case KeyMode_Shift: _calcState.setKeyMode(KeyMode_Normal); break;
    case KeyMode_Alpha: _calcState.setKeyMode(KeyMode_Normal); break;
    case KeyMode_Mode: _calcState.setKeyMode(KeyMode_Normal); break;
    case KeyMode_ShiftMode: _calcState.setKeyMode(KeyMode_Normal); break;
    case KeyMode_ShiftAlpha: break;
    case KeyMode_Hyp: _calcState.setKeyMode(KeyMode_Normal); break;
    case KeyMode_ShiftHyp: _calcState.setKeyMode(KeyMode_Normal); break;
    default:;
    }
}

void Calculator::progScreenChanged()
{
  _lcdDisplay->drawScreen(_progScreen.currentScreen());
}

void Calculator::screenModeChanged(ScreenMode)
{
  switch (_calcState.screenMode())
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
  case ScreenMode_Resume:
    _lcdDisplay->drawScreen(getResumeScreen());
    break;
  case ScreenMode_Editor:
    switch (_calcState.sysMode())
    {
    case SysMode_WRT:
      _lcdDisplay->drawScreen(_progEditScreen.currentScreen());
      _progEditScreen.restartBlink();
      break;
    default:;
    }
  default:;
  }
}
