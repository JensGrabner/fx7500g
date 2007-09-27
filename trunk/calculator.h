#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "lcd_display.h"
#include "prog_screen.h"
#include "prog_edit_screen.h"
#include "run_screen.h"

class Calculator : QObject
{
  Q_OBJECT
public:
  Calculator();

  // Display the resume screen
  void init();

  void setCalMode(CalMode value);
  void setBaseMode(BaseMode value);
  void setDisplayMode(DisplayMode value);

  LCDDisplay *lcdDisplay() { return _lcdDisplay; }
  void setLCDDisplay(LCDDisplay *value) { _lcdDisplay = value; };

  RunScreen &runScreen() { return _runScreen; }

  void applyKey(int key);

  void buttonClicked(int buttonIndex);

private:
  RunScreen _runScreen;
  ProgScreen _progScreen;
  ProgEditScreen _progEditScreen;
  LCDDisplay *_lcdDisplay;

  QList<LCDString> getResumeScreen() const;

private slots:
  void screenModeChanged(ScreenMode oldMode);
  void sysModeChanged(SysMode oldMode);

  void runChangeChar(int col, int line, LCDChar c);
  void runScreenChanged();

  void progChangeChar(int col, int line, LCDChar c);
  void progEditProgram(int programIndex);
  void progScreenChanged();

  void editorChangeChar(int col, int line, LCDChar c);
  void progEditScreenChanged();
};

#endif
