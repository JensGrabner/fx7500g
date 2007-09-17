#ifndef PROGRAM_H
#define PROGRAM_H

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

class Programs
{
public:
  static Programs &instance();

  int count() const { return _programs.count(); }
  Program &at(int index);

  void clear(int programIndex);
  void clearAll();

  int freeSteps() const;

private:
  static const int _freeStepsMax = 4006;

  static Programs *_instance;
  QMap<int,Program> _programs;
  int _freeSteps;

  Programs();
};

#endif
