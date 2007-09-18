#include "memory.h"

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

Memory *Memory::_instance = 0;

Memory &Memory::instance()
{
  if (!_instance)
    _instance = new Memory;

  return *_instance;
}

Memory::Memory() :
  _freeSteps(_freeStepsMax)
{
  // Fill with empty programs
  for (int i = 0; i < programsCount; ++i)
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

Program &Memory::programAt(int index)
{
  Q_ASSERT_X(index >= 0 && index < programsCount, "Memory::at()", qPrintable(QString("Invalid <index> (%1)!").arg(index)));

  return _programs[index];
}

int Memory::freeSteps() const
{
  int steps = _freeStepsMax;

  for (int i = 0; i < programsCount; ++i)
    steps -= _programs[i].size();

  return steps;
}

void Memory::clearProgram(int programIndex)
{
  Q_ASSERT_X(programIndex >= 0 && programIndex < programsCount, "Memory::clear()", qPrintable(QString("Invalid <programIndex> (%&)").arg(programIndex)));

  _programs[programIndex].clear();
}

void Memory::clearAllPrograms()
{
  for (int i = 0; i < programsCount; ++i)
    _programs[i].clear();
}
