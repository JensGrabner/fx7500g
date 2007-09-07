#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "lcd_display.h"
#include "run_screen.h"
#include "prog_screen.h"
#include "misc.h"

class Calculator : QObject
{
  Q_OBJECT
public:
  Calculator();

  // Display the resume screen
  void init();

  enum ScreenMode { ScreenMode_Normal, ScreenMode_Resume, ScreenMode_Graphical };

  ScreenMode screenMode() const { return _screenMode; }
  void setScreenMode(ScreenMode value);

  CalculatorState &calcState() { return _calcState; }

  void setAngleMode(AngleMode value);
  void setSysMode(SysMode value);
  void setCalMode(CalMode value);
  void setBaseMode(BaseMode value);
  void setDisplayMode(DisplayMode value);

  LCDDisplay *lcdDisplay() { return _lcdDisplay; }
  void setLCDDisplay(LCDDisplay *value) { _lcdDisplay = value; };

  RunScreen &runScreen() { return _runScreen; }

  void applyKey(int key);

private:
  CalculatorState _calcState;
  ScreenMode _screenMode;
  RunScreen _runScreen;
  ProgScreen _progScreen;
  LCDDisplay *_lcdDisplay;
  int _steps; // TEMPORARY => replace by a more complete object

  QList<LCDString> getResumeScreen() const;

private slots:
  void shellChangeChar(int col, int line, LCDChar c);
  void shellPromptLineChanged();

  void progChangeChar(int col, int line, LCDChar c);
};

#endif
