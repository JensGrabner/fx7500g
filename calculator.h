#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "screen.h"
#include "shell.h"

class Calculator : QObject
{
  Q_OBJECT
public:
  Calculator();

  enum AngleMode { Deg, Rad, Grad };
  enum DisplayMode { DisplayMode_Resume, DisplayMode_Shell, DisplayMode_Graphical,
                     DisplayMode_WRT, DisplayMode_PCL };

  AngleMode angleMode() const { return _angleMode; }
  void setAngleMode(AngleMode value);

  DisplayMode displayMode() const { return _displayMode; }
  void setDisplayMode(DisplayMode value);

  Screen *screen() { return _screen; }
  void setScreen(Screen *screen) { _screen = screen; };

  Shell &shell() { return _shell; }

  void test();

private:
  DisplayMode _displayMode;
  AngleMode _angleMode;
  Shell _shell;
  Screen *_screen;

private slots:
  void shellChangeChar(int col, int line, LCDChar c);
  void shellPromptLineChanged();
};

#endif
