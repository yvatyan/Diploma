#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ApplicationStyles.h>
#include "FileLineEdit.h"
#include "VideoPlayer/VideoPlayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void StartAndStopVideoPlayer();
    void PauseAndResumeVideoPlayer();
    void SwitchVideoPlayerSource(bool webCamEnabled);

private:
    Ui::MainWindow *ui;
    ApplicationStyles appStyles;
    VideoPlayer* videoPlayer;
    FileLineEdit* videoFilePathEdit;

    void configureOptions();
};

#endif // MAINWINDOW_H
