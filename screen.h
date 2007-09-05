#ifndef LCD_SCREEN_H
#define LCD_SCREEN_H

#include <QGraphicsItem>
#include <QPainter>

#include "misc.h"

class Screen : public QGraphicsItem
{
public:
  Screen(QGraphicsItem *parent = 0);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  QRectF boundingRect () const;

  static const int widthInPlots = 96;
  static const int heightInPlots = 64;

  int plotSize() const { return _plotSize; }
  void setPlotSize(int value);

  void drawChar(LCDChar c, int column, int line);

  void clearNormalScreen();

  void drawScreen(const QList<LCDString> &screen);

private:
  int _plotSize;
  QImage _normalScreen;
  QImage _graphicScreen;
  QPixmap _pixmap;
  QColor _backgroundColor; // No tracable zone
  QColor _pixelOnColor;    // "On" pixel color
  QColor _pixelOffColor;   // "Off" pixel color

  static const int _plotSizeForGrid = 4;
  static const int _leftBorder = 20;
  static const int _rightBorder = 20;
  static const int _topBorder = 0;
  static const int _bottomBorder = 10;

  int tracingAreaWidth() const;
  int tracingAreaHeight() const;

  QRect getPlotRect(int x, int y) const;
  QRect getCharRectInPixmap(int column, int line) const;
  QRect getCharRect(int column, int line) const;
  QRect imageRectToPixmapRect(const QRect &rect) const; // Returns the complete pixmap rect in function of an image rect

  void drawNormalScreenInPixmap() { drawImageInPixmap(_normalScreen); }
  void drawGraphicScreenInPixmap() { drawImageInPixmap(_graphicScreen); }
  void drawImageInPixmap(const QImage &image);

  // <rect> is in the image, not in the pixmap
  void copyImageInPixmap(const QRect &rect);
};

#endif
