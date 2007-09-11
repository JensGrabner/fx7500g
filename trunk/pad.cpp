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

  _highlight = QImage(_padMask.width(), _padMask.height(), QImage::Format_ARGB32);
}

void Pad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
  painter->drawPixmap(option->exposedRect, _pixmap, option->exposedRect);
  painter->drawImage(0, 0, _highlight);
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

  refreshHighlight(buttonIndex);
}

void Pad::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
  refreshHighlight(255);
}

void Pad::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QPoint point = event->pos().toPoint();

  QColor color = _padMask.pixel(point.x(), point.y());

  int buttonIndex = color.red();

  setFlag(QGraphicsItem::ItemIsMovable, buttonIndex == 255);

  if (buttonIndex < 255)
    _calculator->buttonClicked(buttonIndex);
}

void Pad::refreshHighlight(int buttonIndex)
{
  _highlight.fill(0);

  if (buttonIndex == 255)
  {
    update();
    return;
  }

  QColor color(buttonIndex, buttonIndex, buttonIndex);
  QRgb c = color.rgba();
  QColor setColor(Qt::red);
  setColor.setAlpha(50);
  QRgb setC = setColor.rgba();
  for (int i = 0; i < _highlight.width(); ++i)
    for (int j = 0; j < _highlight.height(); ++j)
      if (_padMask.pixel(i, j) == c)
        _highlight.setPixel(i, j, setC);

  update();
}
