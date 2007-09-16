#include <math.h>

#include <QApplication>

#include "main_window.h"
#include "expression_computer.h"

int main(int argc, char *argv[])
{
  LCDLine line("(1/(2x(1+2)))^10");
//  LCDLine line;
  QList<int> expression;
  foreach (int entity, line)
    expression << entity;
  ExpressionComputer::Error error;
  ExpressionComputer::compute(expression, error);

  switch (error)
  {
  case ExpressionComputer::Error_Syntax: qDebug("Syntax error"); break;
  case ExpressionComputer::Error_NumberStack: qDebug("Number stack error"); break;
  case ExpressionComputer::Error_CommandStack: qDebug("Command stack error"); break;
  default:;
  }
  return 0;

  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
