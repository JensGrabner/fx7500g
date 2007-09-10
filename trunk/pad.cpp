#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>

#include "pad.h"

Pad::Pad(const QString &padName, QGraphicsItem *parent) : QGraphicsItem(parent)
{
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  _padImage = QImage(QString(":/images/%1.png").arg(padName));

  _pixmap = QPixmap(_padImage.width(), _padImage.height());
  QPainter painter(&_pixmap);
  painter.drawImage(0, 0, _padImage);
}

void Pad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
  painter->drawPixmap(option->exposedRect, _pixmap, option->exposedRect);
}

QRectF Pad::boundingRect () const
{
  return QRectF(0, 0, _padImage.width(), _padImage.height());
}
