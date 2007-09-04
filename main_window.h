#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "ui_main_window.h"

#include "calculator.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  MainWindow();

private slots:
  void on_sliderSize_sliderMoved(int value);
  void on_pushButtonRandomLetter_clicked();

private:
  QGraphicsScene *_graphicsScene;
  Calculator _calculator;
  Screen *_screen;
};

#endif // MAIN_WINDOW_H
