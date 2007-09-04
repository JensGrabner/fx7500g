#ifndef MISC_H
#define MISC_H

#include <QList>
#include <QChar>

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
  LCDChar_MinusOne,    // -1
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
  LCDChar_Plus,
  LCDChar_Minus,
  LCDChar_Teta,
  LCDChar_LittleA,
  LCDChar_LittleB,
  LCDChar_Dot,
  LCDChar_Cursor,
  LCDChar_InsertCursor,
  LCDChar_InsertCapsLockCursor,
  LCDChar_ShiftCursor,
  LCDChar_CapsLockCursor,
  LCDChar_Space,
  LCDChar_End = LCDChar_Space
};

LCDChar charToLCDChar(const QChar &c, bool *found = 0);

// 'Log', 'Ln', all atomic entities
enum LCDOperator
{
  LCDOp_Log,
  LCDOp_Ln,
  LCDOp_Sin,
  LCDOp_Cos,
  LCDOp_Tan,
  LCDOp_Sinh,
  LCDOp_Cosh,
  LCDOp_Tanh,
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
  LCDOp_Scl
};

class LCDString : public QList<LCDChar>
{
public:
  LCDString() : QList<LCDChar>() {}
  LCDString(const QString &str) { assignString(str); }
  LCDString(LCDChar c);
  LCDString(LCDOperator op);

private:
  void assignString(const QString &str);
};

#endif
