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
    Main.cpp \
    Core/ImageData.cpp \
    Core/Job.cpp \
    Core/OptionsManager.cpp \
    Core/VideoFrameProcessor.cpp \
    Core/ThreadManager/ThreadRunnableTask.cpp \
    GUI/ApplicationStyles.cpp \
    GUI/FileLineEdit.cpp \
    GUI/MainWindow.cpp \
    GUI/VideoPlayer/Background.cpp \
    GUI/VideoPlayer/CoverWidget.cpp \
    GUI/VideoPlayer/VideoPlayer.cpp \
    GUI/VideoPlayer/VideoWidget.cpp


HEADERS  +=\
    Core/ImageData.h \
    Core/Job.h \
    Core/OptionsManager.h \
    Core/VideoFrameProcessor.h \
    Core/ThreadManager/ThreadRunnableTask.h \
    GUI/ApplicationStyles.h \
    GUI/FileLineEdit.h \
    GUI/MainWindow.h \
    GUI/VideoPlayer/Background.h \
    GUI/VideoPlayer/CoverWidget.h \
    GUI/VideoPlayer/VideoPlayer.h \
    GUI/VideoPlayer/VideoWidget.h

FORMS    += GUI/MainWindow.ui

