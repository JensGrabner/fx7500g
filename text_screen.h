#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#include <QTimer>

#include "misc.h"

class TextScreen : public QObject
{
  Q_OBJECT
public:
  enum CursorMode
  {
    CursorMode_Normal,
    CursorMode_Insert,
    CursorMode_InsertCapsLock,
    CursorMode_Shift,
    CursorMode_CapsLock,
    CursorMode_Space
  };

  TextScreen();

  virtual void init(const CalculatorState *calcState);

  void clear();

  CursorMode cursorMode() const { return _cursorMode; }
  void setCursorMode(CursorMode mode);

  QList<LCDString> currentScreen() const; // Returns an instantaneous state of the screen

  void restartBlink(); // MOVE IT IN PROTECTED SECTION LATER

signals:
  void changeChar(int col, int line, LCDChar c);

protected:
  LCDChar _screen[16][8];
  const CalculatorState *_calcState;
  int _cursorLine, _cursorCol;

  bool cursorVisible() const { return _cursorVisible; }
  void setCursorVisible(bool value);

  // Move the cursor (and restore old char and reset the blink timer)
  void moveCursor(int col, int line);

  void assignToScreen(const LCDString &str, int col, int line);

  virtual void feedScreen() = 0; // Used by inherited screen to feed _screen

private:
  bool _cursorVisible;
  CursorMode _cursorMode;
  QTimer _blinkTimer;
  bool _displayCursorTurn;

private slots:
  void doBlinkCursor();
};


#endif
