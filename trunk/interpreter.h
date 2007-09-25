#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

#include "misc.h"
#include "expression_solver.h"

class Interpreter : public QThread
{
  Q_OBJECT

public:
  Interpreter(const QList<TextLine> &program = QList<TextLine>());

  void setProgram(const QList<TextLine> &program);

  void run();

  TextLine getNextDisplayLine();

  bool waitForInput() const { return _waitForInput; }
  void sendInput(const TextLine &value);

  bool error() const { return _error; }
  int errorStep() const { return _errorStep; }

signals:
  void displayLine();

private:
  QQueue<TextLine> _displayLines;
  TextLine _program;
  int _currentOffset;
  QMutex _displayLineMutex;
  ExpressionSolver _expressionSolver;
  bool _error; // If true then the last execution failed
  int _errorStep; // The last error step
  TextLine _input;
  bool _waitForInput;
  QMutex _inputMutex;
  QWaitCondition _inputWaitCondition;

  void execute() throw (InterpreterException);

  int currentEntity();
  int readEntity(); // Returns -1 if it the end of file
  bool eatEntity(int entity); // Returns true if entity is eaten
  int readAlpha() throw(InterpreterException); // Returns an alpha

  TextLine parseString();
  void parseInput();
  void parseLabel();
  void parseGoto();
  // Returns true if (d1 comp d2) is true
  bool computeBoolean(int comp, double d1, double d2);
  void moveOffsetToNextInstruction();

  void storeDisplayLine(const TextLine &textLine);

  QList<TextLine> syntaxError(int step) const;
  QList<TextLine> stackError(int step) const;
  QList<TextLine> memError(int step) const;
  QList<TextLine> argError(int step) const;
  QList<TextLine> gotoError(int step) const;

  void parseVariableAndStore(double d);

  void display(const QList<TextLine> &lines);
};

#endif
