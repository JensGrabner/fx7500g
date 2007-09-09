#ifndef PROG_SCREEN
#define PROG_SCREEN

#include "text_screen.h"

class ProgScreen : public TextScreen
{
  Q_OBJECT
public:
  ProgScreen();

  void init(const CalculatorState *calcState);

  void applyKey(int key);

  void moveLeft();
  void moveRight();

signals:
  void editProgram(int programIndex); // Used to signal that user want edition of a program

protected:
  void feedScreen(); // <_screen> of Shell ancestor is filled with <_lines> and <_promptLine>

private slots:
  void sysModeChanged(SysMode oldMode);

private:
  static const int _startCursorCol = 6;
  static const int _startCursorLine = 7;
};

#endif
