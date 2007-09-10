#ifndef PAD_H
#define PAD_H

#include <QGraphicsItem>
#include <QPainter>

#include "calculator.h"

class Pad : public QGraphicsItem
{
public:
  Pad(int padIndex, Calculator *calculator, QGraphicsItem *parent = 0);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  QRectF boundingRect () const;

protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
  Calculator *_calculator;
  int _padIndex;
  QImage _padImage;
  QImage _padMask;
  QImage _highlight;
  QPixmap _pixmap;

  void refreshHighlight(int buttonIndex);
};

#endif
