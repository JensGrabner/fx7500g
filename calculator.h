#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "screen.h"
#include "shell.h"

class Calculator : QObject
{
  Q_OBJECT
public:
  Calculator();

  // Display the resume screen
  void init();

  enum ScreenMode { ScreenMode_Normal, ScreenMode_Resume, ScreenMode_Graphical };
  enum AngleMode { Deg, Rad, Grad };
  enum SysMode { SysMode_RUN, SysMode_WRT, SysMode_PCL };
  enum CalMode { CalMode_COMP, CalMode_BASE_N, CalMode_SD1, CalMode_LR1, CalMode_SD2, CalMode_LR2 };
  enum BaseMode { BaseMode_Dec, BaseMode_Hex, BaseMode_Bin, BaseMode_Oct };
  enum DisplayMode { DisplayMode_Norm, DisplayMode_Fix, DisplayMode_Sci };

  ScreenMode screenMode() const { return _screenMode; }
  void setScreenMode(ScreenMode value);

  AngleMode angleMode() const { return _angleMode; }
  void setAngleMode(AngleMode value);

  SysMode sysMode() const { return _sysMode; }
  void setSysMode(SysMode value);

  CalMode calMode() const { return _calMode; }
  void setCalMode(CalMode value);

  BaseMode baseMode() const { return _baseMode; }
  void setBaseMode(BaseMode value);

  DisplayMode displayMode() const { return _displayMode; }
  void setDisplayMode(DisplayMode value);

  Screen *screen() { return _screen; }
  void setScreen(Screen *screen) { _screen = screen; };

  Shell &shell() { return _shell; }

  void test();

private:
  ScreenMode _screenMode;
  SysMode _sysMode;
  AngleMode _angleMode;
  CalMode _calMode;
  BaseMode _baseMode;
  DisplayMode _displayMode;
  Shell _shell;
  Screen *_screen;

  QList<LCDString> getResumeScreen() const;
  QString sysModeString() const;
  QString calModeString() const;
  QString baseModeString() const;
  QString angleModeString() const;
  QString displayModeString() const;

private slots:
  void shellChangeChar(int col, int line, LCDChar c);
  void shellPromptLineChanged();
};

#endif
