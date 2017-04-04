#include "VideoPlayer/VideoPlayer.h"

VideoPlayer::VideoPlayer(QWidget* parent)
    : backgroundWidget(parent)
    , videoWidget(parent)
    , coverWidget(parent)
{
    videoWidget.hide();
    state = Idle;
    backgroundWidget.SwitchBgrMode(Background::NoSignal);
    player.setVideoOutput(videoWidget.videoSurface());
    QObject::connect(&player, SIGNAL(positionChanged(qint64)), this, SLOT(OnPositionChange(qint64)));
    QObject::connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(OnDurationChange(qint64)));
    // TODO: Remove this when resize funcionallity will be present
    backgroundWidget.resize(QSize(640, 480));
    coverWidget.resize(QSize(640, 480));
    videoWidget.resize(QSize(640, 480));
}
void VideoPlayer::SwitchInput(VideoStreamFrom source) {
    videoSource = source;
}
void VideoPlayer::SetVideoFile(const QString& filePath) {
    videoFilePath = QUrl::fromLocalFile(filePath);
}
void VideoPlayer::FocusIn(){

}
void VideoPlayer::FocusOut(){

}
void VideoPlayer::Pause() {
    if(state == Active) {
        state = Paused;
        if(videoSource == VideoFile) {
            player.pause();
        }
    }
}
void VideoPlayer::Resume() {
    if(state == Paused ) {
        state = Active;
        if(videoSource == VideoFile) {
            player.play();
        }
    }
}
void VideoPlayer::Start() {
    state = Active;
    backgroundWidget.SwitchBgrMode(Background::Black);
    videoWidget.show();
    if(videoSource == VideoFile) {
        player.setMedia(videoFilePath);
        player.play();
    }
}
void VideoPlayer::Stop() {
    videoWidget.hide();
    state = Idle;
    backgroundWidget.SwitchBgrMode(Background::NoSignal);
    if(videoSource == VideoFile) {
        player.stop();
    }
}
size_t VideoPlayer::FPS() const {
    return 0;
}
size_t VideoPlayer::PFPS() const {
    return 0;
}
VideoPlayer::VideoPlayerState VideoPlayer::State() const {
    return state;
}
void VideoPlayer::OnPositionChange(qint64 pos) {
    emit playerPositionChanged((int) pos);
}
void VideoPlayer::OnDurationChange(qint64 dur) {
    emit playerDurationChanged((int) dur);
}
void VideoPlayer::ConnectProgressBar(QProgressBar* pb) {
    if( state == Active ) {
        pb->setMinimum(0);
        QObject::connect(this, SIGNAL(playerPositionChanged(int)), pb, SLOT(setValue(int)));
        QObject::connect(this, SIGNAL(playerDurationChanged(int)), pb, SLOT(setMaximum(int)));
    }
}
