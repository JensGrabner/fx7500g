// Manage normal mode shell

#ifndef RUN_SCREEN
#define RUN_SCREEN

#include <QTimer>

#include "text_screen.h"

class RunScreen : public TextScreen
{
  Q_OBJECT
public:
  RunScreen();
 
  void write(LCDChar c) { writeEntity(c); }
  void write(LCDOperator o) { writeEntity(o); }

  void applyKey(int key);

  void buttonClicked(int button);

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  void deleteString(); // Delete the string under cursor .. WANTED A BETTER NAME

protected:
  void feedScreen(); // <_screen> of Shell ancestor is filled with <_lines> and <_promptLine>

private:
  int _cursorOffset;
  bool _prompt; // If true, the prompt line is present and cursor is blinking
  ShellLine _promptLine;
  QList<ShellLine> _lines;

  int getPromptLineIndex() const; // Return the line index of the prompt beginning
  void moveCursor(int newOffset);
  void writeEntity(int entity);
};

#endif
