#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>

#include "misc.h"

class Program
{
public:
  bool isEmpty() const { return !_steps.count(); }

  QList<TextLine> &steps() { return _steps; }
  void setSteps(QList<TextLine> value);

  int size() const;

  void clear();

private:
  QList<TextLine> _steps;
};

class Memory
{
public:
  static const int programsCount = 10;

  static Memory &instance();

  Program &programAt(int index);

  void clearVariables();
  void clearProgram(int programIndex = -1);
  void clearAllPrograms();

  int freeSteps() const;

  double variable(int index, bool *overflow = 0); // Return 0 is index > 26 + _extraVarCount
  int extraVarCount() const { return _extraVarCount; }
  bool setExtraVarCount(int value); // Returns false is value is invalid

private:
  static const int _freeStepsMax = 4006;

  static Memory *_instance;
  QMap<int,Program> _programs;
  int _extraVarCount;
  double _variables[526]; // Memory
  int _freeSteps;

  Memory();

  int totalProgramsSize() const;
};

#endif
