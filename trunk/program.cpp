#include "program.h"

int Program::size() const
{
  return _steps.count();
}

void Program::setSteps(QList<TextLine> value)
{
  _steps = value;
}

void Program::clear()
{
  _steps.clear();
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

Programs *Programs::_instance = 0;

Programs &Programs::instance()
{
  if (!_instance)
    _instance = new Programs;

  return *_instance;
}

Programs::Programs() :
  _freeSteps(_freeStepsMax)
{
  // Fill with empty programs
  for (int i = 0; i < 10; ++i)
  {
    Program program;
    QList<TextLine> steps;
    if (i == 0)
    {
      steps << TextLine("SALUT LES COPAIN");
      TextLine textLine;
      textLine << (int) LCDOp_Log << (int) LCDChar_LessEqual;
      steps << textLine;
      for (int i = 0; i < 5; ++i)
      {
        TextLine textLine1;
        textLine1 << (int) LCDOp_Log << (int) LCDChar_LessEqual;
        steps << textLine1;
        TextLine textLine2;
        textLine2 << (int) LCDOp_Ln << (int) LCDOp_Ln << (int) LCDOp_Ln << (int) LCDChar_LessEqual;
        steps << textLine2;
      }
    }
    program.setSteps(steps);

    _programs.insert(i, program);
  }
}

Program &Programs::at(int index)
{
  Q_ASSERT_X(index >= 0 && index < 10, "Programs::at()", qPrintable(QString("Invalid <index> (%1)!").arg(index)));

  return _programs[index];
}

int Programs::freeSteps() const
{
  int steps = _freeStepsMax;

  for (int i = 0; i < 10; ++i)
    steps -= _programs[i].size();

  return steps;
}

void Programs::clear(int programIndex)
{
  Q_ASSERT_X(programIndex >= 0 && programIndex <= 9, "Programs::clear()", qPrintable(QString("Invalid <programIndex> (%&)").arg(programIndex)));

  _programs[programIndex].clear();
}

void Programs::clearAll()
{
  for (int i = 0; i < 10; ++i)
    _programs[i].clear();
}
