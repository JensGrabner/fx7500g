#include <QGraphicsScene>

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow(0)
{
  setupUi(this);

  graphicsView->setScene(_graphicsScene = new QGraphicsScene);

  _calculator.setScreen(_screen = new Screen);
  _graphicsScene->addItem(_screen);
  _screen->setPlotSize(5);
}

void MainWindow::on_sliderSize_sliderMoved(int value)
{
  _screen->setPlotSize(value);
}

void MainWindow::on_pushButtonRandomLetter_clicked()
{
  //_screen->randomLetter();
//  _screen->drawResumeScreen();
  _calculator.test();
  _calculator.setDisplayMode(Calculator::DisplayMode_Shell);
  _calculator.shell().write(LCDChar_O);
  _calculator.shell().moveRight();
  _calculator.shell().write(LCDOp_Log);
}
