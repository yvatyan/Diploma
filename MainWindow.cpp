#include "MainWindow.h"
#include "OptionsManager.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , appStyles(OptionsManager::OptionValue("Style").c_str())
{
    ui->setupUi(this);
    ui->AppThemeCombo->insertItems(0, appStyles.ThemeNames());
    QObject::connect(ui->AppThemeCombo, SIGNAL(activated(int)), &appStyles, SLOT(SetStyle(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
