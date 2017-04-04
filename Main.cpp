#include "MainWindow.h"
#include "OptionsManager.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OptionsManager::Initialize();
    MainWindow w;
    w.show();
    return a.exec();
}
