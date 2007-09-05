#include <QGraphicsScene>
#include <QKeyEvent>

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow(0)
{
  setupUi(this);

  graphicsView->setScene(_graphicsScene = new QGraphicsScene);

  _calculator.setScreen(_screen = new Screen);
  _graphicsScene->addItem(_screen);
  _screen->setPlotSize(5);

  graphicsView->installEventFilter(this);

  _calculator.init();
}

void MainWindow::on_sliderSize_sliderMoved(int value)
{
  _screen->setPlotSize(value);
}

void MainWindow::on_pushButtonRUN_clicked()
{
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  _calculator.setSysMode(Calculator::SysMode_RUN);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

void MainWindow::on_pushButtonWRT_clicked()
{
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  _calculator.setSysMode(Calculator::SysMode_WRT);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

void MainWindow::on_pushButtonPCL_clicked()
{
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  _calculator.setSysMode(Calculator::SysMode_PCL);
  graphicsView->setFocus(Qt::OtherFocusReason);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (/*obj != graphicsView || */event->type() != QEvent::KeyPress)
    return QMainWindow::eventFilter(obj, event);

  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
  _calculator.shell().applyKey(keyEvent->key());
  return true;
}
