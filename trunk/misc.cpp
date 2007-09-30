#include <math.h>

#include <QString>

#include "misc.h"

bool isAlpha(int entity)
{
  return entity >= LCDChar_A && entity <= LCDChar_Z;
}

bool isCipher(int entity)
{
  return entity >= LCDChar_0 && entity <= LCDChar_9;
}

bool isSeparator(int entity)
{
  return entity == LCDChar_Colon ||
         entity == LCDChar_RBTriangle ||
         entity == LCDChar_CR;
}

QChar toNumChar(int entity)
{
  if (isCipher(entity))
    return QChar('0' + entity - LCDChar_0);
  else if (entity == LCDChar_Dot)
    return QChar('.');
  else
    return QChar();
}

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
  else if (ch == '-')
    return LCDChar_Substract;
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

int idToEntity(const QString &id)
{
  if (!id.compare("root", Qt::CaseInsensitive))
    return LCDChar_SquareRoot;
  else if (!id.compare("mul", Qt::CaseInsensitive))
    return LCDChar_Multiply;
  else if (!id.compare("square", Qt::CaseInsensitive))
    return LCDChar_Square;
  else if (!id.compare("minusoneup", Qt::CaseInsensitive))
    return LCDChar_MinusOneUp;
  else if (!id.compare("degree", Qt::CaseInsensitive))
    return LCDChar_Degree;
  else if (!id.compare("ten", Qt::CaseInsensitive))
    return LCDChar_Ten;
  else if (!id.compare("euler", Qt::CaseInsensitive))
    return LCDChar_Euler;
  else if (!id.compare("arrow", Qt::CaseInsensitive) ||
           !id.compare("->"))
    return LCDChar_Arrow;
  else if (!id.compare("MinusPrefix", Qt::CaseInsensitive))
    return LCDChar_MinusPrefix;
  else if (!id.compare("triangle", Qt::CaseInsensitive) ||
           !id.compare("rbtriangle", Qt::CaseInsensitive))
    return LCDChar_RBTriangle;
  else if (!id.compare("doublearrow", Qt::CaseInsensitive) ||
           !id.compare("=>", Qt::CaseInsensitive))
    return LCDChar_DoubleArrow;
  else if (!id.compare("different", Qt::CaseInsensitive) ||
           !id.compare("/=", Qt::CaseInsensitive))
    return LCDChar_Different;
  else if (!id.compare("greaterequal", Qt::CaseInsensitive) ||
           !id.compare(">=", Qt::CaseInsensitive))
    return LCDChar_GreaterEqual;
  else if (!id.compare("lessequal", Qt::CaseInsensitive) ||
           !id.compare("<=", Qt::CaseInsensitive))
    return LCDChar_LessEqual;
  else if (!id.compare("pi", Qt::CaseInsensitive))
    return LCDChar_Pi;
  else if (!id.compare("degsuffix", Qt::CaseInsensitive))
    return LCDChar_DegSuffix;
  else if (!id.compare("radsuffix", Qt::CaseInsensitive))
    return LCDChar_RadSuffix;
  else if (!id.compare("gradsuffix", Qt::CaseInsensitive))
    return LCDChar_GradSuffix;
  else if (!id.compare("int", Qt::CaseInsensitive))
    return LCDOp_Int;
  else if (!id.compare("frac", Qt::CaseInsensitive))
    return LCDOp_Frac;
  else if (!id.compare("log", Qt::CaseInsensitive))
    return LCDOp_Log;
  else if (!id.compare("ln", Qt::CaseInsensitive))
    return LCDOp_Ln;
  else if (!id.compare("sin", Qt::CaseInsensitive))
    return LCDOp_Sin;
  else if (!id.compare("cos", Qt::CaseInsensitive))
    return LCDOp_Cos;
  else if (!id.compare("tan", Qt::CaseInsensitive))
    return LCDOp_Tan;
  else if (!id.compare("sinh", Qt::CaseInsensitive))
    return LCDOp_Sinh;
  else if (!id.compare("cosh", Qt::CaseInsensitive))
    return LCDOp_Cosh;
  else if (!id.compare("tanh", Qt::CaseInsensitive))
    return LCDOp_Tanh;
  else if (!id.compare("sin_1", Qt::CaseInsensitive))
    return LCDOp_Sin;
  else if (!id.compare("cos_1", Qt::CaseInsensitive))
    return LCDOp_Cos;
  else if (!id.compare("tan_1", Qt::CaseInsensitive))
    return LCDOp_Tan;
  else if (!id.compare("sinh_1", Qt::CaseInsensitive))
    return LCDOp_Sinh;
  else if (!id.compare("cosh_1", Qt::CaseInsensitive))
    return LCDOp_Cosh;
  else if (!id.compare("tanh_1", Qt::CaseInsensitive))
    return LCDOp_Tanh;
  else if (!id.compare("not", Qt::CaseInsensitive))
    return LCDOp_Not;
  else if (!id.compare("xor", Qt::CaseInsensitive))
    return LCDOp_Xor;
  else if (!id.compare("xy", Qt::CaseInsensitive))
    return LCDOp_Xy;
  else if (!id.compare("xroot", Qt::CaseInsensitive))
    return LCDOp_xSquareRoot;
  else if (!id.compare("Neg", Qt::CaseInsensitive))
    return LCDOp_Neg;
  else if (!id.compare("And", Qt::CaseInsensitive))
    return LCDOp_And;
  else if (!id.compare("Or", Qt::CaseInsensitive))
    return LCDOp_Or;
  else if (!id.compare("Abs", Qt::CaseInsensitive))
    return LCDOp_Abs;
  else if (!id.compare("cuberoot", Qt::CaseInsensitive))
    return LCDOp_CubeSquareRoot;
  else if (!id.compare("Ans", Qt::CaseInsensitive))
    return LCDOp_Ans;
  else if (!id.compare("Cls", Qt::CaseInsensitive))
    return LCDOp_Cls;
  else if (!id.compare("Prog", Qt::CaseInsensitive))
    return LCDOp_Prog;
  else if (!id.compare("Graph", Qt::CaseInsensitive))
    return LCDOp_Graph;
  else if (!id.compare("Range", Qt::CaseInsensitive))
    return LCDOp_Range;
  else if (!id.compare("Plot", Qt::CaseInsensitive))
    return LCDOp_Plot;
  else if (!id.compare("Factor", Qt::CaseInsensitive))
    return LCDOp_Factor;
  else if (!id.compare("Deg", Qt::CaseInsensitive))
    return LCDOp_Deg;
  else if (!id.compare("Rad", Qt::CaseInsensitive))
    return LCDOp_Rad;
  else if (!id.compare("Grad", Qt::CaseInsensitive) ||
           !id.compare("Gra", Qt::CaseInsensitive))
    return LCDOp_Gra;
  else if (!id.compare("Fix", Qt::CaseInsensitive))
    return LCDOp_Fix;
  else if (!id.compare("Sci", Qt::CaseInsensitive))
    return LCDOp_Sci;
  else if (!id.compare("Norm", Qt::CaseInsensitive))
    return LCDOp_Norm;
  else if (!id.compare("Defm", Qt::CaseInsensitive))
    return LCDOp_Defm;
  else if (!id.compare("Rnd", Qt::CaseInsensitive))
    return LCDOp_Rnd;
  else if (!id.compare("RanSharp", Qt::CaseInsensitive))
    return LCDOp_RanSharp;
  else if (!id.compare("Line", Qt::CaseInsensitive))
    return LCDOp_Line;
  else if (!id.compare("Goto", Qt::CaseInsensitive))
    return LCDOp_Goto;
  else if (!id.compare("Lbl", Qt::CaseInsensitive))
    return LCDOp_Lbl;
  else if (!id.compare("Dsz", Qt::CaseInsensitive))
    return LCDOp_Dsz;
  else if (!id.compare("Isz", Qt::CaseInsensitive))
    return LCDOp_Isz;
  else if (!id.compare("Yon", Qt::CaseInsensitive))
    return LCDOp_Yon;
  else if (!id.compare("YonMinusOne", Qt::CaseInsensitive))
    return LCDOp_YonMinusOne;
  else if (!id.compare("Xon", Qt::CaseInsensitive))
    return LCDOp_Xon;
  else if (!id.compare("XonMinusOne", Qt::CaseInsensitive))
    return LCDOp_XonMinusOne;
  else if (!id.compare("Pol", Qt::CaseInsensitive))
    return LCDOp_Pol;
  else if (!id.compare("Rec", Qt::CaseInsensitive))
    return LCDOp_Rec;
  else if (!id.compare("Mcl", Qt::CaseInsensitive))
    return LCDOp_Mcl;
  else if (!id.compare("Scl", Qt::CaseInsensitive))
    return LCDOp_Scl;
  else
    return -1;
}

QList<LCDChar> stringToChars(const QString &str)
{
  QList<LCDChar> list;

  bool found;
  LCDChar lcdChar;
  foreach (const QChar &c, str)
  {
    lcdChar = charToLCDChar(c, &found);
    if (found)
      list << lcdChar;
  }

  return list;
}

bool isLCDChar(int entity)
{
  return entity >= 0 && entity < 256;
}

bool isLCDOperator(int entity)
{
  return entity >= 256;
}

bool isOperator(int entity)
{
  return entity == LCDChar_Multiply ||
         entity == LCDChar_Divide ||
         entity == LCDChar_Add ||
         entity == LCDChar_Substract ||
         entity == LCDOp_Xy ||
         entity == LCDOp_xSquareRoot;
}

bool isPreFunc(int entity)
{
  return entity == LCDChar_SquareRoot ||
         entity == LCDOp_CubeSquareRoot ||
         entity == LCDOp_Log ||
         entity == LCDChar_Ten ||
         entity == LCDOp_Ln ||
         entity == LCDChar_Euler ||
         entity == LCDOp_Sin ||
         entity == LCDOp_Cos ||
         entity == LCDOp_Tan ||
         entity == LCDOp_Sinh ||
         entity == LCDOp_Cosh ||
         entity == LCDOp_Tanh ||
         entity == LCDOp_Sin_1 ||
         entity == LCDOp_Cos_1 ||
         entity == LCDOp_Tan_1 ||
         entity == LCDOp_Sinh_1 ||
         entity == LCDOp_Cosh_1 ||
         entity == LCDOp_Tanh_1 ||
         entity == LCDChar_MinusPrefix ||
         entity == LCDOp_Abs ||
         entity == LCDOp_Int ||
         entity == LCDOp_Frac ||
         entity == LCDChar_h ||
         entity == LCDChar_d ||
         entity == LCDChar_b ||
         entity == LCDChar_o ||
         entity == LCDOp_Neg ||
         entity == LCDOp_Not;
}

bool isPostFunc(int entity)
{
  return entity == LCDChar_Square ||
         entity == LCDChar_MinusOneUp ||
         entity == LCDChar_Exclamation ||
         entity == LCDChar_DegSuffix ||
         entity == LCDChar_RadSuffix ||
         entity == LCDChar_GradSuffix ||
         entity == LCDChar_Degree;
}

bool isComparisonOperator(int entity)
{
  return entity == LCDChar_Equal ||
         entity == LCDChar_Different ||
         entity == LCDChar_Greater ||
         entity == LCDChar_Less ||
         entity == LCDChar_GreaterEqual ||
         entity == LCDChar_LessEqual;
}

double deg2rad(double deg)
{
  return (deg * M_PI) / 180.0;
}

double deg2grad(double deg)
{
  return (deg * 100.0) / 90.0;
}

double rad2deg(double rad)
{
  return (rad * 180.0) / M_PI;
}

double rad2grad(double rad)
{
  return (rad * 200.0) / M_PI;
}

double grad2deg(double grad)
{
  return (grad * 90.0) / 100.0;
}

double grad2rad(double grad)
{
  return (grad * M_PI) / 200.0;
}

double factorial(double value)
{
  long long int f = (long long int) value;

  if (!f)
    return 1.0;
  else if (f == 1)
    return 1.0;

  double result = 1.0;
  for (int i = 2; i <= f; ++i)
    result *= (double) i;

  return result;
}

QList<LCDChar> operatorToChars(LCDOperator op)
{
  QList<LCDChar> list;
  switch (op)
  {
  case LCDOp_Int: return stringToChars("Int ");
  case LCDOp_Frac: return stringToChars("Frac ");
  case LCDOp_Log: return stringToChars("log ");
  case LCDOp_Ln: return stringToChars("ln ");
  case LCDOp_Sin: return stringToChars("sin ");
  case LCDOp_Cos: return stringToChars("cos ");
  case LCDOp_Tan: return stringToChars("tan ");
  case LCDOp_Sinh: return stringToChars("sinh ");
  case LCDOp_Cosh: return stringToChars("cosh ");
  case LCDOp_Tanh: return stringToChars("tanh ");
  case LCDOp_Sin_1: list = stringToChars("sin"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Cos_1: list = stringToChars("cos"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Tan_1: list = stringToChars("tan"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Sinh_1: list = stringToChars("sinh"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Cosh_1: list = stringToChars("cosh"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Tanh_1: list = stringToChars("tanh"); list << LCDChar_MinusOneUp << LCDChar_Space; break;
  case LCDOp_Not: return stringToChars("Not");
  case LCDOp_Xor: return stringToChars("xor");
  case LCDOp_Xy: list << LCDChar_x << LCDChar_ExpY; break;
  case LCDOp_xSquareRoot: list << LCDChar_SquareRootX << LCDChar_SquareRoot; break;
  case LCDOp_Neg: return stringToChars("Neg "); break;
  case LCDOp_And: return stringToChars("and"); break;
  case LCDOp_Or: return stringToChars("or"); break;
  case LCDOp_Abs: return stringToChars("Abs "); break;
  case LCDOp_CubeSquareRoot: list << LCDChar_Cube << LCDChar_SquareRoot; break;
  case LCDOp_Ans: return stringToChars("Ans"); break;
  case LCDOp_Cls: return stringToChars("Cls"); break;
  case LCDOp_Prog: return stringToChars("Prog "); break;
  case LCDOp_Graph: return stringToChars("Graph Y="); break;
  case LCDOp_Range: return stringToChars("Range "); break;
  case LCDOp_Plot: return stringToChars("Plot "); break;
  case LCDOp_Factor: return stringToChars("Factor "); break;
  case LCDOp_Deg: return stringToChars("Deg"); break;
  case LCDOp_Rad: return stringToChars("Rad"); break;
  case LCDOp_Gra: return stringToChars("Gra"); break;
  case LCDOp_Fix: return stringToChars("Fix "); break;
  case LCDOp_Sci: return stringToChars("Sci "); break;
  case LCDOp_Norm: return stringToChars("Norm"); break;
  case LCDOp_Defm: return stringToChars("Defm "); break;
  case LCDOp_Rnd: return stringToChars("Rnd"); break;
  case LCDOp_RanSharp: return stringToChars("Ran#"); break;
  case LCDOp_Line: return stringToChars("Line"); break;
  case LCDOp_Goto: return stringToChars("Goto "); break;
  case LCDOp_Lbl: return stringToChars("Lbl "); break;
  case LCDOp_Dsz: return stringToChars("Dsz "); break;
  case LCDOp_Isz: return stringToChars("Isz "); break;
  case LCDOp_Yon: list << LCDChar_y << LCDChar_Teta << LCDChar_n; break;
  case LCDOp_YonMinusOne: list << LCDChar_y << LCDChar_Teta << LCDChar_n << LCDChar_MinusOne; break;
  case LCDOp_Xon: list << LCDChar_x << LCDChar_Teta << LCDChar_n; break;
  case LCDOp_XonMinusOne: list << LCDChar_x << LCDChar_Teta << LCDChar_n << LCDChar_MinusOne; break;
  case LCDOp_Pol: return stringToChars("Pol("); break;
  case LCDOp_Rec: return stringToChars("Rec("); break;
  case LCDOp_Mcl: return stringToChars("Mcl"); break;
  case LCDOp_Scl: return stringToChars("Scl"); break;
  default:;
  }
  return list;
}

QList<LCDChar> entityToChars(int entity)
{
  QList<LCDChar> list;
  if (entity < 256)
    list << (LCDChar) entity;
  else
    list = operatorToChars((LCDOperator) entity);
  return list;
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
    LCDChar lcdChar = charToLCDChar(c, &found);

    if (found)
      (*this) << lcdChar;
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
  case LCDOp_Not: assignString("Not"); break;
  case LCDOp_Xor: assignString("xor"); break;
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

CalculatorState *CalculatorState::_instance = 0;

CalculatorState &CalculatorState::instance()
{
  if (!_instance)
    _instance = new CalculatorState;

  return *_instance;
}

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

void CalculatorState::setSysMode(SysMode value, bool forceAffectation)
{
  if (_sysMode == value && !forceAffectation)
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
    case KeyMode_Alpha: noSpecialButton = true; break;
    case KeyMode_Mode: setKeyMode(KeyMode_Hyp); break;
    case KeyMode_ShiftMode: setKeyMode(KeyMode_ShiftHyp); break;
    case KeyMode_ShiftAlpha: noSpecialButton = true; break;
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
          return LCDChar_DegSuffix;
        else
          return LCDOp_Deg;
      case Button_5:
        if (_keyMode == KeyMode_ShiftMode)
          return LCDChar_RadSuffix;
        else
          return LCDOp_Rad;
      case Button_6:
        if (_keyMode == KeyMode_ShiftMode)
          return LCDChar_GradSuffix;
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

int getEntityPriority(int entity)
{
  switch (entity)
  {
  // Here come Pol and Rect (priority 1)

  case LCDChar_Square:
  case LCDChar_MinusOneUp:
  case LCDChar_Exclamation:
  case LCDChar_DegSuffix:
  case LCDChar_RadSuffix:
  case LCDChar_GradSuffix:
  case LCDChar_Degree:
    return 2;

  case LCDOp_Xy:
  case LCDOp_xSquareRoot:
    return 3;

  // Here come the first abregged multiplication forms (priority 4)

  case LCDChar_SquareRoot:
  case LCDOp_CubeSquareRoot:
  case LCDOp_Log:
  case LCDChar_Ten:
  case LCDOp_Ln:
  case LCDChar_Euler:
  case LCDOp_Sin:
  case LCDOp_Cos:
  case LCDOp_Tan:
  case LCDOp_Sinh:
  case LCDOp_Cosh:
  case LCDOp_Tanh:
  case LCDOp_Sin_1:
  case LCDOp_Cos_1:
  case LCDOp_Tan_1:
  case LCDOp_Sinh_1:
  case LCDOp_Cosh_1:
  case LCDOp_Tanh_1:
  case LCDChar_MinusPrefix:
  case LCDOp_Abs:
  case LCDOp_Int:
  case LCDOp_Frac:
  case LCDChar_h:
  case LCDChar_d:
  case LCDChar_b:
  case LCDChar_o:
  case LCDOp_Neg:
  case LCDOp_Not:
    return 5;

  // Here come the second abregged multiplication forms (priority 6)

  case LCDChar_Multiply:
  case LCDChar_Divide:
    return 7;

  case LCDChar_Add:
  case LCDChar_Substract:
    return 8;

  case LCDOp_And:
    return 9;

  case LCDOp_Or:
  case LCDOp_Xor:
    return 10;

  default: return -1;
  }
}

void CalculatorState::setScreenMode(ScreenMode value)
{
  if (value == _screenMode)
    return;

  ScreenMode oldValue = _screenMode;

  _screenMode = value;

  emit screenModeChanged(oldValue);
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////

int comparePriorities(int entity1, int entity2)
{
  int p1 = getEntityPriority(entity1);
  int p2 = getEntityPriority(entity2);
  if (p1 < p2)
    return 1;
  else if (p1 > p2)
    return -1;
  else
    return 0;
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////

TextLine::TextLine(const QString &str, bool rightJustified) :
  _rightJustified(rightJustified)
{
  assignString(str);
}

TextLine::TextLine(int entity, bool rightJustified) :
  _rightJustified(rightJustified)
{
  append(entity);
}

TextLine::TextLine(QList<int> list, bool rightJustified) :
  _rightJustified(rightJustified)
{
  (*this) << list;
}

void TextLine::assignString(const QString &str)
{
  clear();

  int offset = 0;
  while (offset < str.length())
  {
    if (str[offset] == '{') // Id?
    {
      offset++;
      QString id;
      while (offset < str.length() && str[offset] != '}')
        id.append(str[offset++]);
      if (offset < str.length()) // Id found
      {
        int entity = idToEntity(id);
        offset++;
        if (entity >= 0)
          append(entity);
      } else
        break;
    } else
    {
      bool found;
      LCDChar lcdChar = charToLCDChar(str[offset++], &found);
      if (found)
        append(lcdChar);
    }
  }

/*  bool found;
  LCDChar lcdChar;
  foreach (const QChar &c, str)
  {
    lcdChar = charToLCDChar(c, &found);
    if (found)
      append(lcdChar);
  }*/
}

LCDChar TextLine::charAt(int offset) const
{
  int currentLength = 0;
  foreach (int entity, *this)
  {
    QList<LCDChar> list;
    if (isLCDChar(entity))
      list << (LCDChar) entity;
    else
      list = operatorToChars((LCDOperator) entity);

    if (offset >= currentLength && offset < currentLength + list.count())
      return list[offset - currentLength];
    currentLength += list.count();
  }
  return LCDChar_0;
}

int TextLine::charLength() const
{
  int length = 0;
  foreach (int entity, *this)
    if (isLCDChar(entity))
      length++;
    else
      length += operatorToChars((LCDOperator) entity).count();
  return length;
}

QList<LCDChar> TextLine::charLine() const
{
  QList<LCDChar> list;
  foreach (int entity, *this)
    if (isLCDChar(entity))
      list << LCDChar(entity);
    else
      list << operatorToChars((LCDOperator) entity);
  return list;
}

int TextLine::rowCount() const
{
  return charLength() / 16 + 1;
}

int TextLine::entityAt(int offset) const
{
  int currentLength = 0;
  for (int i = 0; i < count(); ++i)
  {
    int entity = at(i);
    QList<LCDChar> list;
    if (isLCDChar(entity))
      list << (LCDChar) entity;
    else
      list = operatorToChars((LCDOperator) entity);

    if (offset >= currentLength && offset < currentLength + list.count())
      return i;
    currentLength += list.count();
  }
  return count();
}

bool TextLine::isBreakerEndedLine() const
{
  return count() && at(count() - 1) == LCDChar_RBTriangle;
}

int TextLine::offsetAt(int entityIndex) const
{
  if (entityIndex >= count())
    return charLength();

  int currentLength = 0;
  for (int i = 0; i < entityIndex; ++i)
  {
    int entity = at(i);
    QList<LCDChar> list;
    if (isLCDChar(entity))
      list << (LCDChar) entity;
    else
      list = operatorToChars((LCDOperator) entity);
    currentLength += list.count();
  }
  return currentLength;
}

bool TextLine::cursorCanMoveRight(int offset) const
{
  return (isBreakerEndedLine() && offset < charLength() - 1) ||
         (!isBreakerEndedLine() && offset < charLength());
}

int TextLine::maximumCursorPosition() const
{
  return isBreakerEndedLine() ? charLength() - 1 : charLength();
}

int TextLine::maximumCursorPositionIfTooHigh(int cursorOffset) const
{
  return cursorOffset > maximumCursorPosition() ? maximumCursorPosition() : cursorOffset;
}

TextLine formatDouble(double d)
{
  TextLine result;

  // Format the double
  QString sortie;
  if ((fabs(d) < 0.01 || fabs(d) >= 10000000000.0) && d != 0.0)
    sortie = QString::number(d, 'E', 9);
  else
    sortie = QString::number(d, 'G', 10);

  // Casio add a '.' at the end of a double if decimal part is 0
  if (sortie.indexOf('.') < 0)
    sortie.append('.');

  // Remove all excessive 0
  int p;
  while ((p = sortie.indexOf("0E")) >= 0)
    sortie.remove(p, 1);
  if (sortie.indexOf('E') < 0)
    while ((p = sortie.indexOf('0')) == sortie.length() - 1)
      sortie.remove(p, 1);

  // Store into the QList<int>
  foreach (const QChar &c, sortie)
  {
    const char ch = c.toLatin1();

    if (ch >= '0' && ch <= '9')
      result << LCDChar_0 + ch - '0';
    else if (ch == '.')
      result << LCDChar_Dot;
    else if (ch == '-')
      result << LCDChar_MinusPrefix;
    else if (ch == '+')
      result << LCDChar_Add;
    else if (ch == 'E')
      result << LCDChar_Exponent;
  }

  return result;
}

void TextLine::affect(QList<TextLine> lines)
{
  clear();
  for (int i = 0; i < lines.count(); ++i)
  {
    if (count() && !isBreakerEndedLine())
      append(LCDChar_CR);
    (*this) << lines[i];
  }
}
