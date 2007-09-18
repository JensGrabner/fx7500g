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

  void clearProgram(int programIndex = -1);
  void clearAllPrograms();

  int freeSteps() const;

private:
  static const int _freeStepsMax = 4006;

  static Memory *_instance;
  QMap<int,Program> _programs;
  int _freeSteps;

  Memory();
};

#endif
