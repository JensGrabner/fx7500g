#ifndef RUN_SCREEN_H
#define RUN_SCREEN_H

#include "editor_screen.h"

class RunScreen : public EditorScreen
{
  Q_OBJECT
public:
  RunScreen() : EditorScreen() {}

  void buttonClicked(int button);

private:
  QList<TextLine> _lastProgram;

  void validate();
  void displayLastProgram(bool cursorOnTop = false); // Empty <_lines> and paste <_lastProgram> inside
};

#endif
