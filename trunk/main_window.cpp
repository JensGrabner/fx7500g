#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPushButton>

#include "main_window.h"

MainWindow::MainWindow() : QMainWindow(0)
{
  setupUi(this);

  graphicsView->setScene(_graphicsScene = new QGraphicsScene);

  _calculator.setLCDDisplay(_lcdDisplay = new LCDDisplay);
  _graphicsScene->addItem(_lcdDisplay);
  _lcdDisplay->setPlotSize(5);

  _lcdDisplay->setPos(0, 0);

  _graphicsScene->addItem(_pad1 = new Pad(1, &_calculator));
  _pad1->setPos(0, 399);

  _graphicsScene->addItem(_pad2 = new Pad(2, &_calculator));
  _pad2->setPos(461, 397);

  _graphicsScene->addItem(_pad3 = new Pad(3, &_calculator));
  _pad3->setPos(0, 329);

  graphicsView->installEventFilter(this);

  _calculator.init();
}

void MainWindow::on_sliderSize_sliderMoved(int value)
{
  _lcdDisplay->setPlotSize(value);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (/*obj != graphicsView || */event->type() != QEvent::KeyPress)
    return QMainWindow::eventFilter(obj, event);

  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
  _calculator.applyKey(keyEvent->key());
  return true;
}
