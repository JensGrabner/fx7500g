TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += main_window.h \
  screen.h \
  calculator.h \
  text_printer.h \
  misc.h \
  shell.h

SOURCES += main.cpp \
  main_window.cpp \
  screen.cpp \
  calculator.cpp \
  text_printer.cpp \
  misc.cpp \
  shell.cpp

FORMS += main_window.ui

RESOURCES = fx-7500G.qrc