#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>

#include "misc.h"

class Program
{
public:
  bool isEmpty() const { return !_steps.count(); }

  const QList<TextLine> &steps() const { return _steps; } // Used for screens but not for interpretation
  void setSteps(const QList<TextLine> &value);

  // This function used <_rawSteps> and is used for interpretation
  // Returns -1 if step is out of bounds
  int entityAt(int step);
  int indexOf(int entity, int from);
  const TextLine &rawSteps() const { return _rawSteps; }

  int size() const; // In steps
  int count() const { return size(); } // Like size

  void clear();

private:
  QList<TextLine> _steps;
  TextLine _rawSteps;
};

class Memory
{
public:
  static const int programsCount = 10;

  static Memory &instance();

  Program *programAt(int index);
  int programsSize() const;

  void clearVariables();
  void clearProgram(int programIndex = -1);
  void clearAllPrograms();

  int freeSteps() const;

  double variable(LCDChar c, int index = 0, bool *overflow = 0); // Return 0 is index > 26 + _extraVarCount
  double variable(int index, bool *overflow = 0); // Return 0 is index > 26 + _extraVarCount
  bool setVariable(int index, double value); // Return false if index > 26 + _extraVarCount
  bool setVariable(LCDChar c, int index, double value); // Return false if overflow
  int extraVarCount() const { return _extraVarCount; }
  bool setExtraVarCount(int value); // Returns false is value is invalid

private:
  static const int _freeStepsMax = 4006;

  static Memory *_instance;
  Program _programs[10];
  int _extraVarCount;
  double _variables[526]; // Memory
  int _freeSteps;

  Memory();

  int totalProgramsSize() const;
};

#endif
