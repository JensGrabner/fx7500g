#ifndef EDITOR_SCREEN
#define EDITOR_SCREEN

#include "text_screen.h"

class EditorScreen : public TextScreen
{
  Q_OBJECT
public:
  EditorScreen();

  void init(CalculatorState *calcState);

  void setProgram(int programIndex);

  virtual void buttonClicked(int button);

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  void deleteString(); // Delete the string under cursor .. WANTED A BETTER NAME

signals:
  void promptLineChanged();

protected:
  QList<TextLine> _lines;
  int _editZoneTopLineIndex; // If 0, all zone is editable, this value is absolute
  void feedScreen(); // <_screen> of Shell ancestor is filled with <_lines> and <_promptLine>
  void carriageReturn();
  void moveCursor(int newLineIndex, int newOffset, bool *scrolled = 0); // Move cursor can invoke scrollUp() or scrollDown() if cursor is out of the screen
  void initTopLineIndex();

private:
  int _topLineIndex; // Absolute index of the top line (the most top visible line at screen) in <_lines>
  int _topLineSubIndex; // Index of the subline of the top line
  int _cursorLineIndex; // Absolute index of the line where cursor is
  int _cursorOffset; // Offset in the line

  bool scrollUp(); // Scroll up the screen if it's possible, returns false if not
  bool scrollDown(); // Scroll down the screen if it's possible, returns false if not
  void insertClicked();

  void writeEntity(int entity);

private slots:
  void keyModeChanged(KeyMode oldMode);
};

#endif
