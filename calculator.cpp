#include "calculator.h"

Calculator::Calculator() :
  _displayMode(DisplayMode_Resume),
  _angleMode(Deg),
  _screen(0)
{
  connect(&_shell, SIGNAL(cursorBlinked(int, int, LCDChar)),
          this, SLOT(shellCursorBlinked(int, int, LCDChar)));
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
    _screen->drawScreen(_shell.currentScreen());
}

void Calculator::shellCursorBlinked(int col, int line, LCDChar c)
{
  if (_screen && _displayMode != DisplayMode_Resume &&
      _displayMode != DisplayMode_Graphical)
    _screen->drawChar(c, col, line);
}

void Calculator::setDisplayMode(DisplayMode value)
{
  if (value == _displayMode)
    return;

  _displayMode = value;

  if (_displayMode == DisplayMode_Shell ||
      _displayMode == DisplayMode_WRT ||
      _displayMode == DisplayMode_PCL)
    _shell.restartBlink();
}

void Calculator::shellPromptLineChanged()
{
  _screen->drawScreen(_shell.currentScreen());
}
