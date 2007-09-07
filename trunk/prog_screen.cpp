#include "prog_screen.h"

ProgScreen::ProgScreen() : TextScreen()
{
  setCursorMode(CursorMode_Space);
}

void ProgScreen::init(const CalculatorState *calcState)
{
  TextScreen::init(calcState);
  connect(calcState, SIGNAL(sysModeChanged(SysMode)), this, SLOT(sysModeChanged(SysMode)));
}

void ProgScreen::applyKey(int key)
{
  switch (key)
  {
  case Qt::Key_Left: moveLeft(); break;
  case Qt::Key_Right: moveRight(); break;
  }
}

void ProgScreen::moveLeft()
{
  if (_cursorCol > _startCursorCol)
    moveCursor(_cursorCol - 1, _cursorLine);
}

void ProgScreen::moveRight()
{
  if (_cursorCol < _startCursorCol + 9)
    moveCursor(_cursorCol + 1, _cursorLine);
}

void ProgScreen::feedScreen()
{
  clear();

  assignToScreen(LCDString(QString("sys mode : %1").arg(_calcState->sysModeString())), 0, 0);

  if (_calcState->calMode() == CalMode_BASE_N)
  {
    assignToScreen(LCDString("cal mode :BASE-N"), 0, 1);
    assignToScreen(LCDString(QString("            %1").arg(_calcState->baseModeString())), 0, 2);
  } else
  {
    assignToScreen(LCDString(QString("cal mode : %1").arg(_calcState->calModeString())), 0, 1);
    assignToScreen(LCDString(QString("   angle : %1").arg(_calcState->angleModeString())), 0, 2);
    assignToScreen(LCDString(QString(" display : %1").arg(_calcState->displayModeString())), 0, 3);
  }

  QString strBytesFree = QString("%1 Bytes Free").arg(14);
  assignToScreen(LCDString(QString(16 - strBytesFree.length(), ' ') + strBytesFree), 0, 5);

  assignToScreen(LCDString(" Prog 0123456789"), 0, 7);
}

void ProgScreen::sysModeChanged(SysMode oldMode)
{
  feedScreen();

  if (oldMode != SysMode_WRT && oldMode != SysMode_PCL)
  {
    _cursorCol = _startCursorCol;
    _cursorLine = _startCursorLine;
  }
}
