#include "memory.h"

#include "prog_screen.h"

ProgScreen::ProgScreen() : TextScreen()
{
  setCursorMode(CursorMode_Space);
}

void ProgScreen::init()
{
  TextScreen::init();
  connect(&CalculatorState::instance(), SIGNAL(sysModeChanged(SysMode)), this, SLOT(sysModeChanged(SysMode)));
}

void ProgScreen::moveLeft()
{
  if (_cursorCol <= _startCursorCol)
    moveCursor(_startCursorCol + 9, _cursorLine);
  else
    moveCursor(_cursorCol - 1, _cursorLine);
}

void ProgScreen::moveRight()
{
  if (_cursorCol >= _startCursorCol + 9)
    moveCursor(_startCursorCol, _cursorLine);
  else
    moveCursor(_cursorCol + 1, _cursorLine);
}

void ProgScreen::feedScreen()
{
  clear();

  CalculatorState &calcState = CalculatorState::instance();
  assignToScreen(LCDString(QString("sys mode : %1").arg(calcState.sysModeString())), 0, 0);

  if (calcState.calMode() == CalMode_BASE_N)
  {
    assignToScreen(LCDString("cal mode :BASE-N"), 0, 1);
    assignToScreen(LCDString(QString("            %1").arg(calcState.baseModeString())), 0, 2);
  } else
  {
    assignToScreen(LCDString(QString("cal mode : %1").arg(calcState.calModeString())), 0, 1);
    assignToScreen(LCDString(QString("   angle : %1").arg(calcState.angleModeString())), 0, 2);
    assignToScreen(LCDString(QString(" display : %1").arg(calcState.displayModeString())), 0, 3);
  }

  Memory &memory = Memory::instance();

  QString strBytesFree = QString("%1 Bytes Free").arg(memory.freeSteps());
  assignToScreen(LCDString(QString(16 - strBytesFree.length(), ' ') + strBytesFree), 0, 5);

  QString str = " Prog 0123456789";

  for (int i = 0; i < memory.programsCount; ++i)
    if (!memory.programAt(i).isEmpty())
      str[6 + i] = '_';
  assignToScreen(LCDString(str), 0, 7);
}

void ProgScreen::sysModeChanged(SysMode oldMode)
{
  feedScreen();

  if (oldMode != SysMode_WRT && oldMode != SysMode_PCL)
  {
    _cursorCol = _startCursorCol;
    _cursorLine = _startCursorLine;
    restartBlink();
  }
}

void ProgScreen::buttonClicked(int button)
{
  CalculatorState &calcState = CalculatorState::instance();

  switch (calcState.sysMode())
  {
  case SysMode_WRT:
    switch (button)
    {
    case Button_Exe: emit editProgram(currentProgramIndex()); break;
    default:;
    }
    break;
  case SysMode_PCL: // Only pad 2 is necessary for PCL mode
    switch (button)
    {
    case Button_Ac:
      Memory::instance().clearProgram(currentProgramIndex());
      feedScreen();
      emit screenChanged();
      break;
    case Button_Del:
      if (calcState.keyMode() == KeyMode_Shift ||
          calcState.keyMode() == KeyMode_ShiftMode ||
          calcState.keyMode() == KeyMode_ShiftHyp)
      {
        Memory::instance().clearAllPrograms();
        feedScreen();
        emit screenChanged();
      }
      break;
    default:;
    }
    break;
  default:;
  }

  switch (button)
  {
  case Button_Left: moveLeft(); break;
  case Button_Right: moveRight(); break;
  default:;
  }

  restartBlink();
}

int ProgScreen::currentProgramIndex() const
{
  return _cursorCol - _startCursorCol;
}
