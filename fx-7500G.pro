TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += release

# Input
HEADERS += main_window.h \
  lcd_display.h \
  calculator.h \
  text_printer.h \
  misc.h \
  text_screen.h \
  run_screen.h \
  prog_screen.h

SOURCES += main.cpp \
  main_window.cpp \
  lcd_display.cpp \
  calculator.cpp \
  text_printer.cpp \
  misc.cpp \
  text_screen.cpp \
  run_screen.cpp \
  prog_screen.cpp

FORMS += main_window.ui

RESOURCES = fx-7500G.qrc