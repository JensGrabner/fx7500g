#ifndef PROGRAM_H
#define PROGRAM_H

#include <QMap>

#include "misc.h"

class Program
{
public:
  bool isEmpty() const { return !_steps.count(); }

  QList<LCDString> &steps() { return _steps; }
  void setSteps(QList<LCDString> value);

  int size() const;

private:
  QList<LCDString> _steps;
};

class Programs
{
public:
  static Programs &instance();

  int count() const { return _programs.count(); }
  Program &at(int index);

  int freeSteps() const;

private:
  static const int _freeStepsMax = 4006;

  static Programs *_instance;
  QMap<int,Program> _programs;
  int _freeSteps;

  Programs();
};

#endif
