#include <QApplication>

#include "main_window.h"
#include "expression_computer.h"

int main(int argc, char *argv[])
{
//  LCDString exp("(1+2)^(2+3)/(5-16.45)");
  LCDString exp("2^3+4");
  QList<int> expression;
  foreach (LCDChar c, exp)
    expression << c;
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
