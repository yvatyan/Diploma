#ifndef VideoPlayer_H
#define VideoPlayer_H

#include <QMediaPlayer>
#include <QPainter>
#include <QPaintEvent>
#include <QProgressBar>
#include "GUI/VideoPlayer/VideoWidget.h"
#include "GUI/VideoPlayer/CoverWidget.h"
#include "GUI/VideoPlayer/Background.h"

class VideoPlayer : public QObject {
     Q_OBJECT
    public:
        enum VideoStreamFrom {
            WebCamera,
            VideoFile
        };
        enum VideoPlayerState {
            Active,
            Idle,
            Paused
        };

        VideoPlayer(QWidget* parent = Q_NULLPTR);

        void SwitchInput(VideoStreamFrom source);

        void FocusIn();
        void FocusOut();
        void Pause();
        void Resume();
        void Start();
        void Stop();

        size_t FPS() const;
        size_t PFPS() const;
        VideoPlayerState State() const;
        void ConnectProgressBar(QProgressBar* pb);
    public slots:
        void SetVideoFile(const QString& filePath);
        void OnPositionChange(qint64 pos);
        void OnDurationChange(qint64 dur);
    signals:
        void playerPositionChanged(int pos);
        void playerDurationChanged(int dur);
    private:
        QMediaPlayer player;
        QUrl videoFilePath;
        VideoPlayerState state;
        VideoStreamFrom videoSource;

        Background backgroundWidget;
        VideoWidget videoWidget;
        CoverWidget coverWidget;

		VideoFrameProcessor videoProcessor;
		void registerImageProcessingJobs();
		void registerJobDemo();
		void registerJobSiftFeatures();
};

#endif // VideoPlayer_H
