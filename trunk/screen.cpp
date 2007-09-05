#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>

#include "screen.h"
#include "text_printer.h"

Screen::Screen(QGraphicsItem *parent) : QGraphicsItem(parent),
  _plotSize(1),
  _backgroundColor(QColor(200, 200, 200)),
  _pixelOnColor(QColor(50, 50, 50)),
  _pixelOffColor(QColor(150, 150, 150))
{
  qsrand(time(0));
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  _normalScreen = QImage(widthInPlots, heightInPlots, QImage::Format_Mono);
  _graphicScreen = QImage(widthInPlots, heightInPlots, QImage::Format_Mono);

  QPainter painter(&_normalScreen);
  painter.fillRect(0, 0, widthInPlots, heightInPlots, QColor(0, 0, 0));

/*  int indexLCD = 0;
  for (int line = 0; line < 8; ++line)
    for (int column = 0; column < 16; ++column)
    {
      if (indexLCD > LCDChar_End)
        break;

      TextPrinter::instance().printChar(_normalScreen, (LCDChar) indexLCD, column, line);
      indexLCD++;
    }*/

  drawNormalScreenInPixmap();
}

void Screen::drawImageInPixmap(const QImage &image)
{
  _pixmap = QPixmap(tracingAreaWidth() + _leftBorder + _rightBorder,
                    tracingAreaHeight() + _topBorder + _bottomBorder);
  QPainter painter(&_pixmap);

  // Draw general background
  painter.fillRect(0, 0, _pixmap.width(), _pixmap.height(), _backgroundColor);

  // Draw tracing background
  painter.fillRect(_leftBorder, _topBorder, tracingAreaWidth(), tracingAreaHeight(), _pixelOffColor);

  // Draw background grid
  if (_plotSize >= _plotSizeForGrid)
  {
    painter.setPen(_backgroundColor);
    for (int line = 1; line < heightInPlots; ++line)
        painter.drawLine(_leftBorder, _topBorder + line * _plotSize - 1,
                         _leftBorder + tracingAreaWidth() - 1, _topBorder + line * _plotSize - 1);
    for (int column = 1; column < widthInPlots; ++column)
        painter.drawLine(_leftBorder + column * _plotSize - 1, _topBorder,
                         _leftBorder + column * _plotSize - 1, _topBorder + tracingAreaHeight() - 1);
  }

  // Draw image
  for (int line = 0; line < heightInPlots; ++line)
    for (int column = 0; column < widthInPlots; ++column)
      if (image.pixel(column, line) != QColor(0, 0, 0).rgb())
        painter.fillRect(getPlotRect(column, line), _pixelOnColor);
}

void Screen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
  painter->drawPixmap(option->exposedRect, _pixmap, option->exposedRect);
}

void Screen::setPlotSize(int value)
{
  if (value != _plotSize && value >= 1)
  {
    prepareGeometryChange();
    _plotSize = value;
    drawNormalScreenInPixmap();
    update();
  }
}

QRectF Screen::boundingRect () const
{
  return QRectF(0, 0, tracingAreaWidth() + _leftBorder + _rightBorder,
                tracingAreaHeight() + _topBorder + _bottomBorder);
}

int Screen::tracingAreaWidth() const
{
  if (_plotSize < _plotSizeForGrid)
    return widthInPlots * _plotSize;
  else
    return widthInPlots * _plotSize - 1;
}

int Screen::tracingAreaHeight() const
{
  if (_plotSize < _plotSizeForGrid)
    return heightInPlots * _plotSize;
  else
    return heightInPlots * _plotSize - 1;
}

QRect Screen::getPlotRect(int x, int y) const
{
  if (_plotSize < _plotSizeForGrid)
    return QRect(_leftBorder + x * _plotSize, _topBorder + y * _plotSize,
                 _plotSize, _plotSize);

  return QRect(_leftBorder + x * _plotSize, _topBorder + y * _plotSize,
               _plotSize - 1, _plotSize - 1);
}

QRect Screen::getCharRectInPixmap(int column, int line) const
{
  Q_ASSERT_X(column >= 0 && column < 16 && line >= 0 && line < 8, "Screen::getCharRectInPixmap()",
             qPrintable(QString("Invalid <column> (%1) or <line> (%2)").arg(column).arg(line)));
  return QRect(_leftBorder + _plotSize * (column * 6 + 1), _topBorder + _plotSize * line * 6,
               _plotSize * 6 - 1, _plotSize * 8 - 1);
}

QRect Screen::getCharRect(int column, int line) const
{
  Q_ASSERT_X(column >= 0 && column < 16 && line >= 0 && line < 8, "Screen::getCharRect()",
             qPrintable(QString("Invalid <column> (%1) or <line> (%2)").arg(column).arg(line)));
  return QRect(column * 6 + 1, line * 8, 5, 7);
}

void Screen::drawChar(LCDChar c, int column, int line)
{
  Q_ASSERT_X(column >= 0 && column < 16 && line >= 0 && line < 8, "Screen::drawChar()",
             qPrintable(QString("Invalid <column> (%1) or <line> (%2)").arg(column).arg(line)));
  TextPrinter::instance().printChar(_normalScreen, c, column, line);
  copyImageInPixmap(getCharRect(column, line));
  update(imageRectToPixmapRect(getCharRect(column, line)));
}

void Screen::copyImageInPixmap(const QRect &rect)
{
  QPainter painter(&_pixmap);

  QRect pixmapRect = imageRectToPixmapRect(rect);

  // Draw tracing background
  painter.fillRect(pixmapRect, _pixelOffColor);

  // Draw background grid
  if (_plotSize >= _plotSizeForGrid)
  {
    painter.setPen(_backgroundColor);
    for (int line = rect.top() + 1; line <= rect.bottom(); ++line)
        painter.drawLine(pixmapRect.left(), _topBorder + line * _plotSize - 1,
                         pixmapRect.right(), _topBorder + line * _plotSize - 1);
    for (int column = rect.left() + 1; column <= rect.right(); ++column)
        painter.drawLine(_leftBorder + column * _plotSize - 1, pixmapRect.top(),
                         _leftBorder + column * _plotSize - 1, pixmapRect.bottom());
  }

  // Draw image
  for (int line = rect.top(); line <= rect.bottom(); ++line)
    for (int column = rect.left(); column <= rect.right(); ++column)
      if (_normalScreen.pixel(column, line) != QColor(0, 0, 0).rgb())
        painter.fillRect(getPlotRect(column, line), _pixelOnColor);
}

QRect Screen::imageRectToPixmapRect(const QRect &rect) const
{
  QRect r;

  r.setLeft(_leftBorder + rect.left() * _plotSize);
  r.setTop(_topBorder + rect.top() * _plotSize);
  r.setWidth(rect.width() * _plotSize);
  r.setHeight(rect.height() * _plotSize);
  if (_plotSize >= _plotSizeForGrid) // If a grid exists, we don't need the last pixels
  {
    r.setWidth(r.width() - 1);
    r.setHeight(r.height() - 1);
  }

  return r;
}

void Screen::clearNormalScreen()
{
  _normalScreen.fill(0);
  drawNormalScreenInPixmap();
  update();
}

void Screen::drawScreen(const QList<LCDString> &screen)
{
  clearNormalScreen();

  int line = 0;
  foreach (const LCDString &lcdStr, screen)
  {
    TextPrinter::instance().printString(_normalScreen, lcdStr, 0, line);
    line++;
  }
  drawNormalScreenInPixmap();
  update();
}
