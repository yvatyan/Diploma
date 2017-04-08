#include "GUI/MainWindow.h"
#include "Core/OptionsManager.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    OptionsManager::Initialize();
    MainWindow w;
    w.show();
    return a.exec();
}
