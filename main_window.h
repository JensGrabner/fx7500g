#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "ui_main_window.h"

#include "calculator.h"
#include "pad.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow();

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private slots:
  void on_sliderSize_sliderMoved(int value);
  void on_pushButtonRUN_clicked();
  void on_pushButtonWRT_clicked();
  void on_pushButtonPCL_clicked();

private:
  QGraphicsScene *_graphicsScene;
  Calculator _calculator;
  LCDDisplay *_lcdDisplay;
  Pad *_pad1, *_pad2, *_pad3;
};

#endif // MAIN_WINDOW_H
