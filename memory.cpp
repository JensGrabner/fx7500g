#include "memory.h"

int Program::size() const
{
  return _rawSteps.count();
}

void Program::setSteps(const QList<TextLine> &value)
{
  _steps = value;
  _rawSteps.affect(value);
}

void Program::clear()
{
  _steps.clear();
}

int Program::entityAt(int step)
{
  if (step < 0 || step >= _rawSteps.count())
    return -1;
  return _rawSteps[step];
}

int Program::indexOf(int entity, int from)
{
  return _rawSteps.indexOf(entity, from);
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
  _extraVarCount(0),
  _freeSteps(_freeStepsMax)
{
  clearVariables();

  QList<TextLine> steps;
  steps << TextLine("\"Z0=\"?{->}Y");
  steps << TextLine("\"Z1=\"?{->}Z");
  steps << TextLine("{root}(1-Z/Y){->}A");
  steps << TextLine("Y{mul}A{->}R:Z/A{->}S:Y/Z{->}B:20{mul}{log}({root}B+{root}(B-1)){->}T");
  steps << TextLine("\"R1=\"{triangle}");
  steps << TextLine("R{triangle}");
  steps << TextLine("\"R2=\"{triangle}");
  steps << TextLine("S{triangle}");
  steps << TextLine("\"LMIN=\":T");

  _programs[0].setSteps(steps);
}

Program *Memory::programAt(int index)
{
  Q_ASSERT_X(index >= 0 && index < programsCount, "Memory::programAt()", qPrintable(QString("Invalid <index> (%1)!").arg(index)));

  return &_programs[index];
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

bool Memory::setExtraVarCount(int value)
{
  Q_ASSERT_X(value >= 0 && value <= 500, "Memory::setExtraVarCount()", qPrintable(QString("Invalid <value> (%1)").arg(value)));

  // Compute the free memory
  int canBeAllocated = (_freeStepsMax - totalProgramsSize()) / 8;
  if (value >= 0 && value <= canBeAllocated)
  {
    int oldCount = _extraVarCount;
    _extraVarCount = value;
    // Reset the new allocated variables
    for (int i = oldCount; i < _extraVarCount; ++i)
      _variables[26 + i] = 0.0;
  }
  else
    return false;
  return true;
}

int Memory::totalProgramsSize() const
{
  int size = 0;
  for (int i = 0; i < programsCount; ++i)
    size += _programs[i].size();
  return size;
}

double Memory::variable(int index, bool *overflow)
{
  if (index < 26 + _extraVarCount)
  {
    if (overflow)
      *overflow = false;
    return _variables[index];
  } else
  {
    if (overflow)
      *overflow = true;
    return 0.0;
  }
}

double Memory::variable(LCDChar c, int index, bool *overflow)
{
  Q_ASSERT_X(c >= LCDChar_A && c <= LCDChar_Z, "Memory::variable()", "invalid c");
  // Compute the absolute index
  return variable(c - LCDChar_A + index, overflow);
}

bool Memory::setVariable(int index, double value)
{
  if (index >= 26 + _extraVarCount)
    return false;

  _variables[index] = value;
  return true;
}

bool Memory::setVariable(LCDChar c, int index, double value)
{
  Q_ASSERT_X(c >= LCDChar_A && c <= LCDChar_Z, "Memory::setVariable()", "invalid c");

  return setVariable(c - LCDChar_A + index, value);
}

void Memory::clearVariables()
{
  for (int i = 0; i < (int) (sizeof(_variables) / sizeof(double)); ++i)
    _variables[i] = sizeof(_variables) - i;
}
