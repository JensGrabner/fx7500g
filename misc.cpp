#include <QString>

#include "misc.h"

LCDChar charToLCDChar(const QChar &c, bool *found)
{
  const char ch = c.toLatin1();

  // Only char in ascii table 127 chars
  if (found)
    *found = true;
  if (ch >= '0' && ch <= '9')
    return (LCDChar) ((int) LCDChar_0 + ch - '0');
  else if (ch >= 'A' && ch <= 'Z')
    return (LCDChar) ((int) LCDChar_A + ch - 'A');
  else if (ch >= 'a' && ch <= 'z')
    return (LCDChar) ((int) LCDChar_a + ch - 'a');
  else if (ch == '!')
    return LCDChar_Exclamation;
  else if (ch == '?')
    return LCDChar_Question;
  else if (ch == '(')
    return LCDChar_OpenParen;
  else if (ch == ')')
    return LCDChar_CloseParen;
  else if (ch == ',')
    return LCDChar_Comma;
  else if (ch == ';')
    return LCDChar_Semicolon;
  else if (ch == '-')
    return LCDChar_MinusPrefix;
  else if (ch == ':')
    return LCDChar_Colon;
  else if (ch == '"')
    return LCDChar_DoubleQuote;
  else if (ch == '=')
    return LCDChar_Equal;
  else if (ch == '>')
    return LCDChar_Greater;
  else if (ch == '<')
    return LCDChar_Less;
  else if (ch == '#')
    return LCDChar_Sharp;
  else if (ch == '[')
    return LCDChar_OpenBracket;
  else if (ch == ']')
    return LCDChar_CloseBracket;
  else if (ch == '*')
    return LCDChar_Asterix;
  else if (ch == '+')
    return LCDChar_Add;
  else if (ch == '/')
    return LCDChar_Divide;
  else if (ch == '.')
    return LCDChar_Dot;
  else if (ch == ' ')
    return LCDChar_Space;
  else if (ch == '_')
    return LCDChar_Cursor;
  else
  {
    if (found)
      *found = false;
    return LCDChar_0;
  }
}

LCDString::LCDString(LCDChar c)
{
  (*this) << c;
}

LCDString::LCDString(LCDOperator op)
{
  assignByOperator(op);
}

LCDString::LCDString(int entity)
{
  if (entity < 256)
    (*this) << (LCDChar) entity;
  else
    assignByOperator((LCDOperator) entity);
}

void LCDString::assignString(const QString &str)
{
  foreach (const QChar &c, str)
  {
    bool found;
    LCDChar ch = charToLCDChar(c, &found);

    if (found)
      (*this) << ch;
  }
}

void LCDString::assignByOperator(LCDOperator op)
{
  switch (op)
  {
  case LCDOp_Int: assignString("Int "); break;
  case LCDOp_Frac: assignString("Frac "); break;
  case LCDOp_Log: assignString("log "); break;
  case LCDOp_Ln: assignString("ln "); break;
  case LCDOp_Sin: assignString("sin "); break;
  case LCDOp_Cos: assignString("cos "); break;
  case LCDOp_Tan: assignString("tan "); break;
  case LCDOp_Sinh: assignString("sinh "); break;
  case LCDOp_Cosh: assignString("cosh "); break;
  case LCDOp_Tanh: assignString("tanh "); break;
  case LCDOp_Sin_1: assignString("sin"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Cos_1: assignString("cos"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Tan_1: assignString("tan"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Sinh_1: assignString("sinh"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Cosh_1: assignString("cosh"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Tanh_1: assignString("tanh"); (*this) << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Xy: (*this) << LCDChar_x << LCDChar_ExpY; break;
  case LCDOp_xSquareRoot: (*this) << LCDChar_SquareRootX << LCDChar_SquareRoot; break;
  case LCDOp_Neg: assignString("Neg "); break;
  case LCDOp_And: assignString("and"); break;
  case LCDOp_Or: assignString("or"); break;
  case LCDOp_Abs: assignString("Abs "); break;
  case LCDOp_CubeSquareRoot: (*this) << LCDChar_Cube << LCDChar_SquareRoot; break;
  case LCDOp_Ans: assignString("Ans"); break;
  case LCDOp_Cls: assignString("Cls"); break;
  case LCDOp_Prog: assignString("Prog "); break;
  case LCDOp_Graph: assignString("Graph Y="); break;
  case LCDOp_Range: assignString("Range "); break;
  case LCDOp_Plot: assignString("Plot "); break;
  case LCDOp_Factor: assignString("Factor "); break;
  case LCDOp_Deg: assignString("Deg"); break;
  case LCDOp_Rad: assignString("Rad"); break;
  case LCDOp_Gra: assignString("Gra"); break;
  case LCDOp_Fix: assignString("Fix "); break;
  case LCDOp_Sci: assignString("Sci "); break;
  case LCDOp_Norm: assignString("Norm"); break;
  case LCDOp_Defm: assignString("Defm "); break;
  case LCDOp_Rnd: assignString("Rnd"); break;
  case LCDOp_RanSharp: assignString("Ran#"); break;
  case LCDOp_Line: assignString("Line"); break;
  case LCDOp_Goto: assignString("Goto "); break;
  case LCDOp_Lbl: assignString("Lbl "); break;
  case LCDOp_Dsz: assignString("Dsz "); break;
  case LCDOp_Isz: assignString("Isz "); break;
  case LCDOp_Yon: (*this) << LCDChar_y << LCDChar_Teta << LCDChar_n; break;
  case LCDOp_YonMinusOne: (*this) << LCDChar_y << LCDChar_Teta << LCDChar_n << LCDChar_MinusOne; break;
  case LCDOp_Xon: (*this) << LCDChar_x << LCDChar_Teta << LCDChar_n; break;
  case LCDOp_XonMinusOne: (*this) << LCDChar_x << LCDChar_Teta << LCDChar_n << LCDChar_MinusOne; break;
  case LCDOp_Pol: assignString("Pol("); break;
  case LCDOp_Rec: assignString("Rec("); break;
  case LCDOp_Mcl: assignString("Mcl"); break;
  case LCDOp_Scl: assignString("Scl"); break;
  default:;
  }
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

void LCDLine::assignString(const QString &str)
{
  foreach (const QChar &c, str)
  {
    bool found;
    LCDChar ch = charToLCDChar(c, &found);

    if (found)
      (*this) << ch;
  }
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

CalculatorState::CalculatorState() :
  _sysMode(SysMode_RUN),
  _angleMode(Deg),
  _calMode(CalMode_COMP),
  _baseMode(BaseMode_Dec),
  _displayMode(DisplayMode_Norm),
  _keyMode(KeyMode_Normal)
{
}

QString CalculatorState::baseModeString() const
{
  switch (_baseMode)
  {
  case BaseMode_Dec: return "Dec";
  case BaseMode_Hex: return "Hex";
  case BaseMode_Bin: return "Bin";
  case BaseMode_Oct: return "Oct";
  default: return "";
  }
}

QString CalculatorState::angleModeString() const
{
  switch (_angleMode)
  {
  case Deg: return "Deg";
  case Rad: return "Rad";
  case Grad: return "Gra";
  default: return "";
  }
}

QString CalculatorState::sysModeString() const
{
  switch (_sysMode)
  {
  case SysMode_RUN: return "RUN";
  case SysMode_WRT: return "WRT";
  case SysMode_PCL: return "PCL";
  default: return "";
  }
}

QString CalculatorState::displayModeString() const
{
  switch (_displayMode)
  {
  case DisplayMode_Norm: return "Norm";
  case DisplayMode_Fix: return "Fix";
  case DisplayMode_Sci: return "Sci";
  default: return "";
  }
}

QString CalculatorState::calModeString() const
{
  switch (_calMode)
  {
  case CalMode_COMP: return "COMP";
  case CalMode_BASE_N: return "BASE-N";
  case CalMode_SD1: return "SD 1";
  case CalMode_LR1: return "LR 1";
  case CalMode_SD2: return "SD 2";
  case CalMode_LR2: return "LR 2";
  default: return "";
  }
}

void CalculatorState::setSysMode(SysMode value)
{
  if (_sysMode == value)
    return;

  SysMode oldMode = _sysMode;

  _sysMode = value;
  emit sysModeChanged(oldMode);
}

void CalculatorState::setKeyMode(KeyMode value)
{
  if (_keyMode == value)
    return;

  KeyMode oldMode = _keyMode;

  _keyMode = value;
  emit keyModeChanged(oldMode);
}

void CalculatorState::changeKeyModeByButton(int button, bool &noSpecialButton)
{
  noSpecialButton = false;

  switch (button)
  {
  case Button_Shift:
    switch (_keyMode)
    {
    case KeyMode_Normal: setKeyMode(KeyMode_Shift); break;
    case KeyMode_Shift: setKeyMode(KeyMode_Normal); break;
    case KeyMode_Alpha: setKeyMode(KeyMode_Shift); break;
    case KeyMode_Mode: setKeyMode(KeyMode_ShiftMode); break;
    case KeyMode_ShiftMode: setKeyMode(KeyMode_Normal); break;
    case KeyMode_ShiftAlpha: setKeyMode(KeyMode_Shift); break;
    case KeyMode_Hyp: setKeyMode(KeyMode_ShiftHyp); break;
    case KeyMode_ShiftHyp: setKeyMode(KeyMode_Normal); break;
    default:;
    }
    break;
  case Button_Alpha:
    switch (_keyMode)
    {
    case KeyMode_Normal: setKeyMode(KeyMode_Alpha); break;
    case KeyMode_Shift: setKeyMode(KeyMode_ShiftAlpha); break;
    case KeyMode_Alpha: setKeyMode(KeyMode_Normal); break;
    case KeyMode_Mode: setKeyMode(KeyMode_Alpha); break;
    case KeyMode_ShiftMode: setKeyMode(KeyMode_ShiftAlpha); break;
    case KeyMode_ShiftAlpha: setKeyMode(KeyMode_Normal); break;
    case KeyMode_Hyp: setKeyMode(KeyMode_Alpha); break;
    case KeyMode_ShiftHyp: setKeyMode(KeyMode_ShiftAlpha); break;
    default:;
    }
    break;
  case Button_Mode:
    switch (_keyMode)
    {
    case KeyMode_Normal: setKeyMode(KeyMode_Mode); break;
    case KeyMode_Shift: setKeyMode(KeyMode_ShiftMode); break;
    case KeyMode_Alpha: setKeyMode(KeyMode_Mode); break;
    case KeyMode_Mode: setKeyMode(KeyMode_Mode); break;
    case KeyMode_ShiftMode: setKeyMode(KeyMode_ShiftMode); break;
    case KeyMode_ShiftAlpha: setKeyMode(KeyMode_Mode); break;
    case KeyMode_Hyp: setKeyMode(KeyMode_Mode); break;
    case KeyMode_ShiftHyp: setKeyMode(KeyMode_ShiftMode); break;
    default:;
    }
    break;
  case Button_Hyp:
    switch (_keyMode)
    {
    case KeyMode_Normal: setKeyMode(KeyMode_Hyp); break;
    case KeyMode_Shift: setKeyMode(KeyMode_ShiftHyp); break;
    case KeyMode_Alpha: break;
    case KeyMode_Mode: setKeyMode(KeyMode_Hyp); break;
    case KeyMode_ShiftMode: setKeyMode(KeyMode_ShiftHyp); break;
    case KeyMode_ShiftAlpha: break;
    case KeyMode_Hyp: break;
    case KeyMode_ShiftHyp: break;
    default:;
    }
    break;
  default: noSpecialButton = true;
  }
}

int CalculatorState::printableEntityByButton(int button) const
{
  if (button < 33)
    return printableEntityByButtonInPad1(button);
  else
    return printableEntityByButtonInPad2(button);
}

int CalculatorState::printableEntityByButtonInPad1(int button) const
{
    switch (_keyMode)
    {
    case KeyMode_Normal:
    case KeyMode_Hyp:
      switch (button)
      {
      case Button_7: return LCDChar_7;
      case Button_8: return LCDChar_8;
      case Button_9: return LCDChar_9;
      case Button_Cls: return LCDOp_Cls;
      case Button_Prog: return LCDOp_Prog;
      case Button_4: return LCDChar_4;
      case Button_5: return LCDChar_5;
      case Button_6: return LCDChar_6;
      case Button_Multiply: return LCDChar_Multiply;
      case Button_Divide: return LCDChar_Divide;
      case Button_Graph: return LCDOp_Graph;
      case Button_Range: return LCDOp_Range;
      case Button_Plot: return LCDOp_Plot;
      case Button_1: return LCDChar_1;
      case Button_2: return LCDChar_2;
      case Button_3: return LCDChar_3;
      case Button_Add: return LCDChar_Add;
      case Button_Substract: return LCDChar_Substract;
      case Button_Factor: return LCDOp_Factor;
      case Button_Line: return LCDOp_Line;
      case Button_0: return LCDChar_0;
      case Button_Dot: return LCDChar_Dot;
      case Button_Exp: return LCDChar_Exponent;
      case Button_Ans: return LCDOp_Ans;
      default:;
      }
      break;
    case KeyMode_Shift:
    case KeyMode_ShiftHyp:
      switch (button)
      {
      case Button_7: return LCDChar_DoubleArrow;
      case Button_8: return LCDChar_Equal;
      case Button_9: return LCDChar_Different;
      case Button_Del: return LCDOp_Mcl;
      case Button_Prog: return LCDOp_Goto;
      case Button_4: return LCDOp_Isz;
      case Button_5: return LCDChar_GreaterEqual;
      case Button_6: return LCDChar_LessEqual;
      case Button_Graph: return LCDOp_Graph;
      case Button_Range: return LCDOp_Range;
      case Button_Plot: return LCDOp_Lbl;
      case Button_1: return LCDOp_Dsz;
      case Button_2: return LCDChar_Greater;
      case Button_3: return LCDChar_Less;
      case Button_Add: return LCDOp_Pol;
      case Button_Substract: return LCDOp_Rec;
      case Button_Factor: return LCDOp_Factor;
      case Button_Line: return LCDOp_Line;
      case Button_0: return LCDOp_Rnd;
      case Button_Dot: return LCDOp_RanSharp;
      case Button_Exp: return LCDChar_Pi;
      case Button_Ans: return LCDOp_Ans;
      }
      break;
    case KeyMode_Alpha:
    case KeyMode_ShiftAlpha:
      switch (button)
      {
      case Button_7: return LCDChar_M;
      case Button_8: return LCDChar_N;
      case Button_9: return LCDChar_O;
      case Button_Cls: return LCDOp_Cls;
      case Button_Prog: return LCDOp_Prog;
      case Button_4: return LCDChar_P;
      case Button_5: return LCDChar_Q;
      case Button_6: return LCDChar_R;
      case Button_Multiply: return LCDChar_S;
      case Button_Divide: return LCDChar_T;
      case Button_Graph: return LCDOp_Graph;
      case Button_Range: return LCDOp_Range;
      case Button_Plot: return LCDOp_Plot;
      case Button_1: return LCDChar_U;
      case Button_2: return LCDChar_V;
      case Button_3: return LCDChar_W;
      case Button_Add: return LCDChar_X;
      case Button_Substract: return LCDChar_Y;
      case Button_Factor: return LCDOp_Factor;
      case Button_Line: return LCDOp_Line;
      case Button_0: return LCDChar_Z;
      case Button_Dot: return LCDChar_OpenBracket;
      case Button_Exp: return LCDChar_CloseBracket;
      case Button_Ans: return LCDChar_Space;
      }
      break;
    case KeyMode_Mode:
    case KeyMode_ShiftMode:
      switch (button)
      {
      case Button_7: return LCDOp_Fix;
      case Button_8: return LCDOp_Sci;
      case Button_9: return LCDOp_Norm;
      case Button_Cls: return LCDOp_Cls;
      case Button_Prog: return LCDOp_Prog;
      case Button_4:
        if (_keyMode == KeyMode_ShiftMode)
          return LCDChar_LittleO;
        else
          return LCDOp_Deg;
      case Button_5:
        if (_keyMode == KeyMode_ShiftMode)
          return LCDChar_LittleR;
        else
          return LCDOp_Rad;
      case Button_6:
        if (_keyMode == KeyMode_ShiftMode)
          return LCDChar_LittleG;
        else
          return LCDOp_Gra;
      case Button_Graph: return LCDOp_Graph;
      case Button_Range: return LCDOp_Range;
      case Button_Plot: return LCDOp_Plot;
      case Button_Factor: return LCDOp_Factor;
      case Button_Line: return LCDOp_Line;
      case Button_0: return LCDChar_0;
      case Button_Dot: return LCDOp_Defm;
      case Button_Exp: return LCDChar_Exponent;
      case Button_Ans: return LCDOp_Ans;
      }
      break;
    default:;
    }

    return -1;
}

int CalculatorState::printableEntityByButtonInPad2(int button) const
{
    switch (_keyMode)
    {
    case KeyMode_Normal:
    case KeyMode_Mode:
    case KeyMode_Hyp:
      switch (button)
      {
      case Button_SquareRoot: return LCDChar_SquareRoot;
      case Button_Square: return LCDChar_Square;
      case Button_Log: return LCDOp_Log;
      case Button_Ln: return LCDOp_Ln;
      case Button_MinusOne: return LCDChar_MinusOneUp;
      case Button_Degree: return LCDChar_Degree;
      case Button_Ten: return LCDChar_Ten;
      case Button_Euler: return LCDChar_Euler;
      case Button_Sin:
        switch (_keyMode)
        {
        case KeyMode_Hyp: return LCDOp_Sinh;
        default: return LCDOp_Sin;
        }
      case Button_Cos:
        switch (_keyMode)
        {
        case KeyMode_Hyp: return LCDOp_Cosh;
        default: return LCDOp_Cos;
        }
      case Button_Tan:
        switch (_keyMode)
        {
        case KeyMode_Hyp: return LCDOp_Tanh;
        default: return LCDOp_Tan;
        }
      case Button_Question: return LCDChar_Question;
      case Button_Arrow: return LCDChar_Arrow;
      case Button_OpenParen: return LCDChar_OpenParen;
      case Button_CloseParen: return LCDChar_CloseParen;
      case Button_Minus: return LCDChar_MinusPrefix;
      case Button_Colon: return LCDChar_Colon;
      case Button_Xy: return LCDOp_Xy;
      case Button_xSquareRoot: return LCDOp_xSquareRoot;
      default:;
      }
      break;
    case KeyMode_Shift:
    case KeyMode_ShiftMode:
    case KeyMode_ShiftHyp:
      switch (button)
      {
      case Button_SquareRoot: return LCDOp_Int;
      case Button_Square: return LCDOp_Frac;
      case Button_Log: return LCDOp_Log;
      case Button_Ln: return LCDOp_Ln;
      case Button_MinusOne: return LCDChar_Exclamation;
      case Button_Ten: return LCDChar_Ten;
      case Button_Euler: return LCDChar_Euler;
      case Button_Sin:
        switch (_keyMode)
        {
        case KeyMode_ShiftHyp: return LCDOp_Sinh_1;
        default: return LCDOp_Sin_1;
        }
      case Button_Cos:
        switch (_keyMode)
        {
        case KeyMode_ShiftHyp: return LCDOp_Sinh_1;
        default: return LCDOp_Cos_1;
        }
      case Button_Tan:
        switch (_keyMode)
        {
        case KeyMode_ShiftHyp: return LCDOp_Sinh_1;
        default: return LCDOp_Tan_1;
        }
      case Button_Question: return LCDChar_Question;
      case Button_Arrow: return LCDChar_Tilde;
      case Button_OpenParen: return LCDChar_Comma;
      case Button_CloseParen: return LCDChar_Semicolon;
      case Button_Minus: return LCDChar_MinusPrefix;
      case Button_Colon: return LCDChar_RBTriangle;
      case Button_Xy: return LCDOp_Abs;
      case Button_xSquareRoot: return LCDOp_CubeSquareRoot;
      }
      break;
    case KeyMode_Alpha:
    case KeyMode_ShiftAlpha:
      switch (button)
      {
      case Button_SquareRoot: return LCDChar_Micro;
      case Button_Square: return LCDChar_n;
      case Button_Log: return LCDChar_p;
      case Button_Ln: return LCDChar_f;
      case Button_MinusOne: return LCDChar_A;
      case Button_Degree: return LCDChar_B;
      case Button_Ten: return LCDChar_C;
      case Button_Euler: return LCDChar_D;
      case Button_Hyp: return LCDChar_E;
      case Button_Sin: return LCDChar_F;
      case Button_Cos: return LCDChar_G;
      case Button_Tan: return LCDChar_H;
      case Button_Question: return LCDChar_I;
      case Button_Arrow: return LCDChar_J;
      case Button_OpenParen: return LCDChar_K;
      case Button_CloseParen: return LCDChar_L;
      case Button_Minus: return LCDChar_MinusPrefix;
      case Button_Colon: return LCDChar_DoubleQuote;
      case Button_Xy: return LCDChar_k;
      case Button_xSquareRoot: return LCDChar_m;
      }
      break;
    default:;
    }
    return -1;
}
