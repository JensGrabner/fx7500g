#ifndef EDITOR_SCREEN
#define EDITOR_SCREEN

#include "text_screen.h"

class EditorScreen : public TextScreen
{
  Q_OBJECT
public:
  EditorScreen();

  void write(LCDChar c);
  void write(LCDOperator o);

  void applyKey(int key);

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  void deleteString(); // Delete the string under cursor .. WANTED A BETTER NAME

signals:
  void promptLineChanged();

protected:
  void feedScreen(); // <_screen> of Shell ancestor is filled with <_lines> and <_promptLine>

private:
  QList<ShellLine> _lines;
  int _topLineIndex; // Absolute index of the top line (the most top visible line at screen) in <_lines>
  int _cursorLineIndex; // Absolute index of the line where cursor is
  int _cursorOffset; // Offset in the line

  void moveCursor(int newOffset);
};

#endif
