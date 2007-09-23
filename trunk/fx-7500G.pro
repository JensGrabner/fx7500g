TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += debug

# Input
HEADERS += main_window.h \
  lcd_display.h \
  calculator.h \
  text_printer.h \
  misc.h \
  text_screen.h \
  prog_screen.h \
  editor_screen.h \
  run_screen.h \
  prog_edit_screen.h \
  memory.h \
  pad.h \
  expression_computer.h \
  program_parser.h

SOURCES += main.cpp \
  main_window.cpp \
  lcd_display.cpp \
  calculator.cpp \
  text_printer.cpp \
  misc.cpp \
  text_screen.cpp \
  prog_screen.cpp \
  editor_screen.cpp \
  run_screen.cpp \
  prog_edit_screen.cpp \
  memory.cpp \
  pad.cpp \
  expression_computer.cpp \
  program_parser.cpp

FORMS += main_window.ui

RESOURCES = fx-7500G.qrc