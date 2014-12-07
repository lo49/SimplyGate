
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = immat

HEADERS += \
    square.h

SOURCES += \
    main.cpp \
    square.cpp



LIBS += `pkg-config opencv --libs`
LIBS += -I/usr/local/include/tesseract -L/usr/lib -llept -ltesseract
