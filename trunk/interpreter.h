#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QStack>

#include "misc.h"
#include "expression_solver.h"

class Interpreter : public QObject
{
  Q_OBJECT

public:
  Interpreter(const QList<TextLine> &program = QList<TextLine>());

  void setProgram(const QList<TextLine> &program);

  void execute(const TextLine &textLine = TextLine());

  bool waitForDataMode() const { return _waitForDataMode; }

signals:
  void displayLine(const TextLine &textLine);

private:
  TextLine _program;
  int _currentOffset;
  bool _waitForDataMode; // If true, interpreter is waiting for input data

  ExpressionSolver _expressionSolver;

  int currentEntity();
  int readEntity(); // Returns -1 if it the end of file
  bool eatEntity(int entity); // Returns true if entity is eaten

  TextLine parseString();
  void parseInput();
  // Returns true if (d1 comp d2) is true
  bool computeBoolean(int comp, double d1, double d2);
  void moveOffsetToNextInstruction();
};

#endif
