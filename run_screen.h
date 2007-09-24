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
  Interpreter _interpreter;

  void validate();
  void displayLastProgram(bool cursorOnTop = false); // Empty <_lines> and paste <_lastProgram> inside
  void setWaitingMode(bool value);
  QList<TextLine> syntaxError(int step) const;
  QList<TextLine> stackError(int step) const;
  QList<TextLine> memError(int step) const;
  QList<TextLine> argError(int step) const;
  QList<TextLine> gotoError(int step) const;

  void getLineAndStep(const QList<TextLine> &program, int offset, int &line, int &step) const;

private slots:
  void interpreterDisplayLine(const TextLine &textLine);
};

#endif
