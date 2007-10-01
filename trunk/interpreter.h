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
  bool waitForValidation() const { return _waitForValidation; }
  void sendInput(const TextLine &value);
  void sendValidation();

  bool error() const { return _error; }
  int errorStep() const { return _errorStep; }

  double lastResult() const { return _lastResult; }

  bool displayDefm() const { return _displayDefm; }

signals:
  void displayLine();
  void askForValidation();

private:
  class ProgramIndex // Search for a better name
  {
  public:
    ProgramIndex() {}
    ProgramIndex(int p, int s) : program(p), step(s) {}

    int program; // 0 -> 9
    int step;
  };

  QQueue<TextLine> _displayLines;
  TextLine _program;
  int _currentProgramIndex; // -1 => use _program, else use Memory::instance()
  int _currentOffset;
  QMutex _displayLineMutex;
  ExpressionSolver _expressionSolver;
  bool _error; // If true then the last execution failed
  int _errorStep; // The last error step
  double _lastResult;
  TextLine _input;
  bool _waitForInput;
  bool _waitForValidation;
  bool _displayDefm;
  QMutex _inputMutex;
  QWaitCondition _inputWaitCondition;
  QStack<ProgramIndex> _callStack;

  void execute() throw (InterpreterException);

  int currentEntity() const;
  int readEntity(); // Returns -1 if it the end of file
  bool eatEntity(int entity); // Returns true if entity is eaten
  int readAlpha() throw (InterpreterException); // Returns an alpha
  int indexOfEntity(int entity, int from) const;
  int entityAt(int index) const;
  const TextLine &program() const;

  // Returns true if (d1 comp d2) is true
  bool computeBoolean(int comp, double d1, double d2);
  void moveOffsetToNextInstruction();

  void storeDisplayLine(const TextLine &textLine);

  QList<TextLine> errorLines(Error error, int step) const;

  TextLine parseString();
  void parseLabel();
  void parseGoto();
  bool parseProg();
  void parseVariableAndStore(double d);
  void parseInput(const TextLine &prefix = TextLine());
  void parseDefm();

  void display(const QList<TextLine> &lines);

  void changeAngleMode(int entity);
};

#endif
