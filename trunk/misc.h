#ifndef MISC_H
#define MISC_H

#include <QList>
#include <QChar>
#include <QObject>

enum LCDChar {
  LCDChar_0 = 0,
  LCDChar_1,
  LCDChar_2,
  LCDChar_3,
  LCDChar_4,
  LCDChar_5,
  LCDChar_6,
  LCDChar_7,
  LCDChar_8,
  LCDChar_9,
  LCDChar_A,
  LCDChar_B,
  LCDChar_C,
  LCDChar_D,
  LCDChar_E,
  LCDChar_F,
  LCDChar_G,
  LCDChar_H,
  LCDChar_I,
  LCDChar_J,
  LCDChar_K,
  LCDChar_L,
  LCDChar_M,
  LCDChar_N,
  LCDChar_O,
  LCDChar_P,
  LCDChar_Q,
  LCDChar_R,
  LCDChar_S,
  LCDChar_T,
  LCDChar_U,
  LCDChar_V,
  LCDChar_W,
  LCDChar_X,
  LCDChar_Y,
  LCDChar_Z,
  LCDChar_a,
  LCDChar_b,
  LCDChar_c,
  LCDChar_d,
  LCDChar_e,
  LCDChar_f,
  LCDChar_g,
  LCDChar_h,
  LCDChar_i,
  LCDChar_j,
  LCDChar_k,
  LCDChar_l,
  LCDChar_m,
  LCDChar_n,
  LCDChar_o,
  LCDChar_p,
  LCDChar_q,
  LCDChar_r,
  LCDChar_s,
  LCDChar_t,
  LCDChar_u,
  LCDChar_v,
  LCDChar_w,
  LCDChar_x,
  LCDChar_y,
  LCDChar_z,
  LCDChar_SquareRoot,  // Square root
  LCDChar_Square,      // ²
  LCDChar_MinusOneUp,  // -1
  LCDChar_Exclamation, // !
  LCDChar_Degree,      // °
  LCDChar_Ten,         // "10"
  LCDChar_Euler,       // e
  LCDChar_Question,    // ?
  LCDChar_Arrow,       // ->
  LCDChar_OpenParen,   // (
  LCDChar_CloseParen,  // )
  LCDChar_Comma,       // ,
  LCDChar_Semicolon,   // ;
  LCDChar_MinusPrefix, // -
  LCDChar_Colon,       // :
  LCDChar_RBTriangle,  // Right bottom triangle
  LCDChar_DoubleQuote, // "
  LCDChar_ExpY,        // Little y exposant (Xy)
  LCDChar_SquareRootX, // Little x for X-square root
  LCDChar_Cube,        // Little 3 for cube
  LCDChar_DoubleArrow, // =>
  LCDChar_Equal,       // =
  LCDChar_Different,   // /=
  LCDChar_Greater,     // >
  LCDChar_Less,        // <
  LCDChar_GreaterEqual,// >=
  LCDChar_LessEqual,   // <=
  LCDChar_Sharp,       // #
  LCDChar_Pi,
  LCDChar_OpenBracket, // [
  LCDChar_CloseBracket,// ]
  LCDChar_Asterix,     // *
  LCDChar_HexA,
  LCDChar_HexB,
  LCDChar_HexC,
  LCDChar_HexD,
  LCDChar_HexE,
  LCDChar_HexF,
  LCDChar_x_,
  LCDChar_y_,
  LCDChar_x_hat,
  LCDChar_y_hat,
  LCDChar_Tilde,
  LCDChar_Micro,
  LCDChar_Multiply,
  LCDChar_Divide,
  LCDChar_Add,
  LCDChar_Substract,
  LCDChar_Teta,
  LCDChar_LittleA,
  LCDChar_LittleB,
  LCDChar_Dot,
  LCDChar_Cursor,
  LCDChar_InsertCursor,
  LCDChar_InsertCapsLockCursor,
  LCDChar_ShiftCursor,
  LCDChar_CapsLockCursor,
  LCDChar_InsertShiftCursor,
  LCDChar_MinusOne,  // -1
  LCDChar_Exponent,  // E
  LCDChar_LittleO,   // Suffix for degree values
  LCDChar_LittleR,   // Suffix for radian values
  LCDChar_LittleG,   // Suffix for gradian values
  LCDChar_Space,
  LCDChar_End = LCDChar_Space
};

LCDChar charToLCDChar(const QChar &c, bool *found = 0);
QList<LCDChar> stringToChars(const QString &str);

// 'Log', 'Ln', all atomic entities
enum LCDOperator
{
  LCDOp_Int = 256,
  LCDOp_Frac,
  LCDOp_Log,
  LCDOp_Ln,
  LCDOp_Sin,
  LCDOp_Cos,
  LCDOp_Tan,
  LCDOp_Sinh,
  LCDOp_Cosh,
  LCDOp_Tanh,
  LCDOp_Sin_1, // Sin exponent -1
  LCDOp_Cos_1, // Cos exponent -1
  LCDOp_Tan_1, // Cos exponent -1
  LCDOp_Sinh_1, // Sinh exponent -1
  LCDOp_Cosh_1, // Cosh exponent -1
  LCDOp_Tanh_1, // Tanh exponent -1
  LCDOp_Not,
  LCDOp_Xor,
  LCDOp_Xy,
  LCDOp_xSquareRoot,
  LCDOp_Neg,
  LCDOp_And,
  LCDOp_Or,
  LCDOp_Abs,
  LCDOp_CubeSquareRoot,
  LCDOp_Ans,
  LCDOp_Cls,
  LCDOp_Prog,
  LCDOp_Graph,
  LCDOp_Range,
  LCDOp_Plot,
  LCDOp_Factor,
  LCDOp_Deg,
  LCDOp_Rad,
  LCDOp_Gra,
  LCDOp_Fix,
  LCDOp_Sci,
  LCDOp_Norm,
  LCDOp_Defm,
  LCDOp_Rnd,
  LCDOp_RanSharp,
  LCDOp_Line,
  LCDOp_Goto,
  LCDOp_Lbl,
  LCDOp_Dsz,
  LCDOp_Isz,
  LCDOp_Yon,
  LCDOp_YonMinusOne,
  LCDOp_Xon,
  LCDOp_XonMinusOne,
  LCDOp_Pol,
  LCDOp_Rec,
  LCDOp_Mcl,
  LCDOp_Scl
};

QList<LCDChar> operatorToChars(LCDOperator op);
QList<LCDChar> entityToChars(int entity);
bool isLCDChar(int entity);
bool isLCDOperator(int entity);

int getEntityPriority(int entity);
// Return :
// 0 if priorities are equal
// 1 if <entity1> is more prioritary than <entity2>
// -1 if <entity1> is less prioritary than <entity2>
int comparePriorities(int entity1, int entity2);

class LCDString : public QList<LCDChar>
{
public:
  LCDString() : QList<LCDChar>() {}
  LCDString(const QString &str) { assignString(str); }
  LCDString(LCDChar c);
  LCDString(LCDOperator op);
  LCDString(int entity);

private:
  void assignString(const QString &str);
  void assignByOperator(LCDOperator op);
};

enum PadButton
{
  Button_Shift = 1,
  Button_Alpha,
  Button_Mode,
  Button_7,
  Button_8,
  Button_9,
  Button_Del,
  Button_Ac,
  Button_Cls,
  Button_Eng,
  Button_Prog,
  Button_4,
  Button_5,
  Button_6,
  Button_Multiply,
  Button_Divide,
  Button_Graph,
  Button_Range,
  Button_Plot,
  Button_1,
  Button_2,
  Button_3,
  Button_Add,
  Button_Substract,
  Button_Trace,
  Button_Factor,
  Button_Line,
  Button_0,
  Button_Dot,
  Button_Exp,
  Button_Ans,
  Button_Exe,
  Button_SquareRoot,
  Button_Square,
  Button_Log,
  Button_Ln,
  Button_MinusOne,
  Button_Degree,
  Button_Ten,
  Button_Euler,
  Button_Hyp,
  Button_Sin,
  Button_Cos,
  Button_Tan,
  Button_Question,
  Button_Arrow,
  Button_OpenParen,
  Button_CloseParen,
  Button_Minus,
  Button_Colon,
  Button_Xy,
  Button_xSquareRoot,
  Button_MDisp,
  Button_GT,
  Button_Up,
  Button_Down,
  Button_Left,
  Button_Right,
  Button_Ins
};

enum ScreenMode {
  ScreenMode_Normal,      // Normal screen when sysMode is RUN and sysMode is WRT
  ScreenMode_Resume,      // Resume screen
  ScreenMode_Graphical,   // Graphical screen
  ScreenMode_Editor       // Editor screen when sysMode is WRT
};

enum AngleMode { Deg, Rad, Grad };
enum SysMode { SysMode_RUN, SysMode_WRT, SysMode_PCL };
enum CalMode { CalMode_COMP, CalMode_BASE_N, CalMode_SD1, CalMode_LR1, CalMode_SD2, CalMode_LR2 };
enum BaseMode { BaseMode_Dec, BaseMode_Hex, BaseMode_Bin, BaseMode_Oct };
enum DisplayMode { DisplayMode_Norm, DisplayMode_Fix, DisplayMode_Sci };
enum KeyMode { KeyMode_Normal, KeyMode_Shift, KeyMode_Alpha, KeyMode_Mode, KeyMode_ShiftMode, KeyMode_ShiftAlpha,
  KeyMode_Hyp, KeyMode_ShiftHyp };

class CalculatorState : public QObject
{
  Q_OBJECT
public:
  CalculatorState();

  ScreenMode screenMode() const { return _screenMode; }
  void setScreenMode(ScreenMode value);

  SysMode sysMode() const { return _sysMode; }
  void setSysMode(SysMode value);

  AngleMode angleMode() const { return _angleMode; }
  void setAngleMode(AngleMode value) { _angleMode = value; }

  CalMode calMode() const { return _calMode; }
  void setCalMode(CalMode value) { _calMode = value; }

  BaseMode baseMode() const { return _baseMode; }
  void setBaseMode(BaseMode value) { _baseMode = value; }

  DisplayMode displayMode() const { return _displayMode; }
  void setDisplayMode(DisplayMode value) { _displayMode = value; }

  KeyMode keyMode() const { return _keyMode; }
  void setKeyMode(KeyMode value);

  void changeKeyModeByButton(int button, bool &noSpecialButton);

  // Return -1 if not printable
  int printableEntityByButton(int button) const;

  QString sysModeString() const;
  QString calModeString() const;
  QString baseModeString() const;
  QString angleModeString() const;
  QString displayModeString() const;

signals:
  void screenModeChanged(ScreenMode oldMode);
  void sysModeChanged(SysMode oldMode);
  void keyModeChanged(KeyMode oldMode);

private:
  ScreenMode _screenMode;
  SysMode _sysMode;
  AngleMode _angleMode;
  CalMode _calMode;
  BaseMode _baseMode;
  DisplayMode _displayMode;
  KeyMode _keyMode;

  int printableEntityByButtonInPad1(int button) const;
  int printableEntityByButtonInPad2(int button) const;
};

class TextLine : public QList<int>
{
public:
  TextLine(const QString &str = "", bool rightJustified = false);
  TextLine(int entity, bool rightJustified = false);

  bool rightJustified() const { return _rightJustified; }
  void setRightJustified(bool value) { _rightJustified = value; }

  void assignString(const QString &str); // Transforms a QString into a QList of entities

  LCDChar charAt(int offset) const; // Returns LCDChar_0 if offset is not valid
  int charLength() const;
  QList<LCDChar> charLine() const;
  int rowCount() const; // Returns the row line in the screen (16 is the screen width)

  int entityAt(int offset) const; // Returns the entity index. If offset is too big, returns count()
  int offsetAt(int entityIndex) const; // Returns the offset of the first LCDChar of the entity given by its index. If entityIndex is too big, returns charLength()

  bool isBreakerEndedLine() const;
  bool cursorCanMoveRight(int offset) const;
  int maximumCursorPosition() const;
  int maximumCursorPositionIfTooHigh(int cursorOffset) const;

private:
  bool _rightJustified;
};

#endif
