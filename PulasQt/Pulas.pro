#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T14:10:03
#
#-------------------------------------------------

QT      += core gui websockets network printsupport webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pulas
TEMPLATE = app

SOURCES += main.cpp\
        maindialog.cpp \
    clientmanager.cpp \
    client.cpp \
    printer.cpp \
    pdf.cpp

HEADERS  += maindialog.h \
    constant.h \
    clientmanager.h \
    client.h \
    printer.h \
    pdf.h

FORMS    += maindialog.ui

win32:RC_FILE = Pulas.rc
RESOURCES += Pulas.qrc

LIBS += -lKernel32 -lwinspool

win32: LIBS += -L$$PWD/../external_libs/poppler/lib/ -llibpoppler-qt5

INCLUDEPATH += $$PWD/../external_libs/poppler/include
DEPENDPATH += $$PWD/../external_libs/poppler/include
