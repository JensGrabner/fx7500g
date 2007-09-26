#ifndef RUN_SCREEN_H
#define RUN_SCREEN_H

#include "editor_screen.h"
#include "interpreter.h"

class RunScreen : public EditorScreen
{
  Q_OBJECT
public:
  RunScreen();

  void buttonClicked(int button);

private:
  QList<TextLine> _lastProgram;
  bool _waitingMode;
  bool _errorMode;
  int _lastErrorLine;
  int _lastErrorStep;
  QTimer _timerDisplay;
  Interpreter _interpreter;

  void validate();
  void displayLastProgram(bool cursorOnTop = false); // Empty <_lines> and paste <_lastProgram> inside
  void setWaitingMode(bool value);

  void getLineAndStep(const QList<TextLine> &program, int offset, int &line, int &step) const;

  void resetScreen();

private slots:
  void interpreterDisplayLine();
  void timerDisplayTimeout();
  void interpreterFinished();
};

#endif
