#include "program.h"

int Program::size() const
{
  return _steps.count();
}

void Program::setSteps(QList<LCDString> value)
{
  _steps = value;
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
    QList<LCDString> steps;
    if (i == 3)
    {
      steps << LCDString("s") << LCDString("a") << LCDString("l") << LCDString("u") << LCDString("t");
    } else if (i == 5)
    {
      steps << LCDString("les") << LCDString(" copains");
    }
    program.setSteps(steps);

    _programs.insert(i, program);
  }
}

Program &Programs::at(int index)
{
  Q_ASSERT_X(index >= 0 && index < 10, "Programs::at()", QString("Invalid <index> (%1)!").arg(index));

  return _programs[index];
}

int Programs::freeSteps() const
{
  int steps = _freeStepsMax;

  for (int i = 0; i < 10; ++i)
    steps -= _programs[i].size();

  return steps;
}
