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
    return LCDChar_Plus;
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
  clear();
  (*this) << c;
}

LCDString::LCDString(LCDOperator op)
{
  clear();
  switch (op)
  {
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
  case LCDOp_Scl: assignString("Scl"); break;
  default:;
  }
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

CalculatorState::CalculatorState() :
  _sysMode(SysMode_RUN),
  _angleMode(Deg),
  _calMode(CalMode_COMP),
  _baseMode(BaseMode_Dec),
  _displayMode(DisplayMode_Norm)
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
