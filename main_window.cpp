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
  //on_pushButtonRandomLetter_clicked();
}

void MainWindow::on_sliderSize_sliderMoved(int value)
{
  _screen->setPlotSize(value);
}

void MainWindow::on_pushButtonRandomLetter_clicked()
{
  _calculator.setScreenMode(Calculator::ScreenMode_Normal);
  _calculator.test();
  _calculator.shell().write(LCDOp_Log);
  _calculator.shell().write(LCDOp_Log);
  _calculator.shell().write(LCDOp_Log);
  _calculator.shell().write(LCDOp_Log);
  _calculator.shell().write(LCDChar_A);
  _calculator.shell().write(LCDOp_Ln);
  _calculator.shell().write(LCDOp_YonMinusOne);
  _calculator.shell().write(LCDOp_Cos_1);
  _calculator.shell().write(LCDOp_Sin_1);
  _calculator.shell().write(LCDOp_Tan_1);
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
