#ifndef RUN_SCREEN_H
#define RUN_SCREEN_H

#include "editor_screen.h"

class RunScreen : public EditorScreen
{
  Q_OBJECT
public:
  RunScreen() : EditorScreen(), _waitingMode(false), _errorMode(false) {}

  void buttonClicked(int button);

private:
  QList<TextLine> _lastProgram;
  bool _waitingMode;
  bool _errorMode;
  int _lastErrorLine;
  int _lastErrorStep;

  void validate();
  void displayLastProgram(bool cursorOnTop = false); // Empty <_lines> and paste <_lastProgram> inside
  void setWaitingMode(bool value);
  QList<TextLine> syntaxError(int step) const;
  QList<TextLine> stackError(int step) const;
  QList<TextLine> memError(int step) const;
};

#endif
