#ifndef PAD_H
#define PAD_H

#include <QGraphicsItem>
#include <QPainter>

class Pad : public QGraphicsItem
{
public:
  Pad(const QString &padName, QGraphicsItem *parent = 0);


  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  QRectF boundingRect () const;

private:
  QImage _padImage;
  QPixmap _pixmap;
};

#endif
