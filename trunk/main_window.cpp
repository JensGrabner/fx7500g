#include <QGraphicsScene>
#include <QKeyEvent>

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow(0)
{
  setupUi(this);

  graphicsView->setScene(_graphicsScene = new QGraphicsScene);

  _calculator.setLCDDisplay(_lcdDisplay = new LCDDisplay);
  _graphicsScene->addItem(_lcdDisplay);
  _lcdDisplay->setPlotSize(5);

  _lcdDisplay->setPos(70, 0);

  _graphicsScene->addItem(_pad1 = new Pad("pad1"));
  _pad1->setPos(0, 340);

  _graphicsScene->addItem(_pad2 = new Pad("pad2"));
  _pad2->setPos(490, 340);

  graphicsView->installEventFilter(this);

  _calculator.init();
}

void MainWindow::on_sliderSize_sliderMoved(int value)
{
  _lcdDisplay->setPlotSize(value);
}

void MainWindow::on_pushButtonRUN_clicked()
{
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  _calculator.setSysMode(SysMode_RUN);
//  _calculator.runScreen().write(LCDOp_Log);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

void MainWindow::on_pushButtonWRT_clicked()
{
  _calculator.setSysMode(SysMode_WRT);
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

void MainWindow::on_pushButtonPCL_clicked()
{
  _calculator.setSysMode(SysMode_PCL);
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (/*obj != graphicsView || */event->type() != QEvent::KeyPress)
    return QMainWindow::eventFilter(obj, event);

  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
  _calculator.applyKey(keyEvent->key());
  return true;
}
