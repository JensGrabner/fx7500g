#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>

#include "pad.h"

Pad::Pad(int padIndex, Calculator *calculator, QGraphicsItem *parent) : QGraphicsItem(parent),
  _calculator(calculator),
  _padIndex(padIndex)
{
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setAcceptsHoverEvents(true);

  _padImage = QImage(QString(":/images/pad%1.png").arg(padIndex));
  _padMask = QImage(QString(":/images/pad%1-mask.png").arg(padIndex));

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

void Pad::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
  QPoint point = event->pos().toPoint();

  QColor color = _padMask.pixel(point.x(), point.y());

  int buttonIndex = color.red();
}

void Pad::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QPoint point = event->pos().toPoint();

  QColor color = _padMask.pixel(point.x(), point.y());

  int buttonIndex = color.red();

  setFlag(QGraphicsItem::ItemIsMovable, buttonIndex == 255);

  if (buttonIndex < 255)
    _calculator->buttonClicked(_padIndex, buttonIndex);
}
