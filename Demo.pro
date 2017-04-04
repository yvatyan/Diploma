#-------------------------------------------------
#
# Project created by QtCreator 2017-02-26T19:54:42
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES +=\
    MainWindow.cpp \
    Main.cpp \
    ApplicationStyles.cpp \
    OptionsManager.cpp \
    VideoPlayer/VideoPlayer.cpp \
    VideoPlayer/VideoWidget.cpp \
    VideoPlayer/CoverWidget.cpp \
    VideoPlayer/Background.cpp \
    FileLineEdit.cpp

HEADERS  += MainWindow.h \
    ApplicationStyles.h \
    OptionsManager.h \
    VideoPlayer/VideoPlayer.h \
    VideoPlayer/VideoWidget.h \
    VideoPlayer/CoverWidget.h \
    VideoPlayer/Background.h \
    FileLineEdit.h

FORMS    += MainWindow.ui
