#ifndef TEXT_PRINTER_H
#define TEXT_PRINTER_H

#include <QMap>

#include "misc.h"

class TextPrinter
{
public:
  static TextPrinter &instance();

  void printChar(QPaintDevice &paintDevice, LCDChar c, int col, int line);
  void printString(QPaintDevice &paintDevice, const LCDString &str, int col, int line, bool wordwrap = true);

private:
  static TextPrinter *_instance;
  QMap<LCDChar,QImage> _charset;

  TextPrinter();
};

#endif
