#include "GUI/MainWindow.h"
#include "Core/OptionsManager.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->AppThemeCoB->insertItems(0, appStyles.ThemeNames());

    videoPlayer = new VideoPlayer(ui->VideoFrame);
    videoPlayer->setObjectName(QStringLiteral("VideoPlayer"));

    videoFilePathEdit = new FileLineEdit(
                            QString("Video Files (*.wmv *.avi *.mp4 *.3gp)"),
                            FileLineEdit::ColorfulAlways,
                            ui->VideoStreamSrcGp,
                            ui->VideoFileLoadButton
                        );
    videoFilePathEdit->setObjectName(QStringLiteral("VideoFilePathEdit"));
    videoFilePathEdit->setGeometry(QRect(10, 30, 350, 22));

    QObject::connect(ui->AppThemeCoB, SIGNAL(activated(int)), &appStyles, SLOT(SetStyle(int)));
    QObject::connect(ui->PlayVideoButton, SIGNAL(clicked(bool)), this, SLOT(StartAndStopVideoPlayer()));
    QObject::connect(ui->PauseVideoButton, SIGNAL(clicked(bool)), this, SLOT(PauseAndResumeVideoPlayer()));
    QObject::connect(ui->WebcamChB, SIGNAL(clicked(bool)), this, SLOT(SwitchVideoPlayerSource(bool)));
    QObject::connect(videoFilePathEdit, SIGNAL(fileSelected(QString)), videoPlayer, SLOT(SetVideoFile(QString)));

    configureOptions();
}
MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::configureOptions() {
    QString optionValue;
    optionValue = OptionsManager::OptionValue("Style").c_str(); {
        int index = appStyles.SetStyle("");
        ui->AppThemeCoB->setItemText(index, appStyles.StyleName(index) + " (default)");
        if(optionValue.isEmpty()) {
            ui->AppThemeCoB->setCurrentIndex(index);
        } else {
            ui->AppThemeCoB->setCurrentIndex(appStyles.SetStyle(optionValue));
        }
    }
    optionValue = OptionsManager::OptionValue("WebCamEnabled").c_str(); {
        ui->WebcamChB->setChecked(false);
        if(optionValue.isEmpty()) {
            ui->WebcamChB->click();
            ui->WebcamChB->click();
        } else {
            ui->WebcamChB->click();
        }
    }
    optionValue = OptionsManager::OptionValue("VideoFile").c_str(); {
        if(optionValue.isEmpty()) {
        } else {
            videoFilePathEdit->SetFile(optionValue);
        }
    }
}
void MainWindow::StartAndStopVideoPlayer() {
    if(videoPlayer->State() == VideoPlayer::Active) {
        ui->PlayVideoButton->setText("Play");
        videoPlayer->Stop();
        ui->VideoProgress->reset();
    } else if(videoPlayer->State() == VideoPlayer::Idle) {
        ui->PlayVideoButton->setText("Stop");
        videoPlayer->Start();
        videoPlayer->ConnectProgressBar(ui->VideoProgress);
    }
}
void MainWindow::PauseAndResumeVideoPlayer(){
    if(videoPlayer->State() == VideoPlayer::Paused) {
        ui->PauseVideoButton->setText("Pause");
        videoPlayer->Resume();
    } else if(videoPlayer->State() == VideoPlayer::Active) {
        ui->PauseVideoButton->setText("Resume");
        videoPlayer->Pause();
    }
}
void MainWindow::SwitchVideoPlayerSource(bool webCamEnabled) {
    if(webCamEnabled) {
        videoPlayer->SwitchInput(VideoPlayer::WebCamera);
        ui->VideoProgress->setEnabled(false);
        videoFilePathEdit->setEnabled(false);
    } else {
        videoPlayer->SwitchInput(VideoPlayer::VideoFile);
        ui->VideoProgress->setEnabled(true);
        videoFilePathEdit->setEnabled(true);
    }
}
