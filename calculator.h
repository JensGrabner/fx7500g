#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "lcd_display.h"
#include "run_screen.h"
#include "prog_screen.h"
#include "editor_screen.h"

class Calculator : QObject
{
  Q_OBJECT
public:
  Calculator();

  // Display the resume screen
  void init();

  enum ScreenMode {
    ScreenMode_Normal,      // Normal screen when sysMode is RUN and sysMode is WRT
    ScreenMode_Resume,      // Resume screen
    ScreenMode_Graphical,   // Graphical screen
    ScreenMode_Editor       // Editor screen when sysMode is WRT
  };

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

  void buttonClicked(int buttonIndex);

private:
  CalculatorState _calcState;
  ScreenMode _screenMode;
  RunScreen _runScreen;
  ProgScreen _progScreen;
  EditorScreen _editorScreen;
  LCDDisplay *_lcdDisplay;

  QList<LCDString> getResumeScreen() const;

private slots:
  void runChangeChar(int col, int line, LCDChar c);
  void runScreenChanged();

  void progChangeChar(int col, int line, LCDChar c);
  void progEditProgram(int programIndex);
  void progScreenChanged();

  void editorChangeChar(int col, int line, LCDChar c);
  void editorScreenChanged();
};

#endif
