#include <QImage>
#include <QPainter>

#include "text_printer.h"

TextPrinter *TextPrinter::_instance = 0;

TextPrinter &TextPrinter::instance()
{
  if (!_instance)
    _instance = new TextPrinter;

  return *_instance;
}

TextPrinter::TextPrinter()
{
  // Load the charset and store it
  QImage pattern(":/images/casio-charset.png");

  int charWidth = pattern.width() / 6;
  int charHeight = pattern.width() / 8;
  int indexLCD = 0;

  for (int line = 0; line < charHeight; ++line)
    for (int column = 0; column < charWidth; ++column)
    {
      if (indexLCD > (int) LCDChar_End)
        break;

      _charset.insert((LCDChar) indexLCD, pattern.copy(column * 6 + 1, line * 8, 5, 7));
      indexLCD++;
    }
}

void TextPrinter::printChar(QPaintDevice &paintDevice, LCDChar c, int col, int line)
{
  QPainter painter(&paintDevice);

  QImage image = _charset[c];
  if (image.isNull())
    return;

  painter.drawImage(col * 6 + 1, line * 8, image);
}

void TextPrinter::printString(QPaintDevice &paintDevice, const LCDString &str, int col, int line, bool wordwrap)
{
  if (!str.count()) // Don't draw empty strings!
    return;

  QPainter painter(&paintDevice);

  int c = col;
  int l = line;
  foreach (LCDChar ch, str)
  {
    QImage image = _charset[ch];
    if (!image.isNull())
      painter.drawImage(c * 6 + 1, l * 8, image);
    c++;

    // To the next position (wordwrap?)
    if (c > 15 && wordwrap)
    {
      l++;
      c = 0;
    }

    if (c > 15 || l > 7)
      break;
  }
}
