// Manage normal mode shell

#ifndef SHELL
#define SHELL

#include <QTimer>

#include "misc.h"

// A ShellLine is a list of strings
class ShellLine : public QList<LCDString>
{
public:
  ShellLine(bool rightJustified = false) : _isRightJustified(rightJustified) {}
  ShellLine(const LCDString &lcdStr, bool rightJustified = false);

  bool isRightJustified() const { return _isRightJustified; }

  int length() const; // Returns the LCDChars length

  LCDChar charAtOffset(int offset) const; // Returns LCD_Space if offset is incorrect
  int stringIndexAtOffset(int offset) const; // Returns -1 if offset is incorrect
  int offsetByStringIndex(int strIndex) const; // Returns -1 if strIndex is incorrect
private:
  bool _isRightJustified;
};

class Shell : public QObject
{
  Q_OBJECT
public:
  enum CursorMode
  {
    Cursor,
    InsertCursor,
    InsertCapsLockCursor,
    ShiftCursor,
    CapsLockCursor
  };

  Shell();

  CursorMode cursorMode() const { return _cursorMode; }
  void setCursorMode(CursorMode mode);

  void restartBlink();

  QList<LCDString> currentScreen() const; // Returns an instantaneous state of the screen

  bool write(LCDChar c); // Returns false if writing is forbidden
  bool write(LCDOperator o);

  bool moveLeft(); // Returns false if movement is forbidden
  bool moveRight();
  bool moveUp();
  bool moveDown();

signals:
  void cursorBlinked(int col, int line, LCDChar c);
  void promptLineChanged();

private:
  int _cursorOffset;
  CursorMode _cursorMode;
  bool _prompt; // If true, the prompt line is present and cursor is blinking
  ShellLine _promptLine;
  QTimer _blinkTimer;
  bool _displayCursorTurn;
  QList<ShellLine> _lines;

  int getPromptLineIndex() const; // Return the line index of the prompt beginning

private slots:
  void doBlinkCursor();
};

#endif
