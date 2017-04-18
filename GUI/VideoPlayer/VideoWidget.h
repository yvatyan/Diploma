#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QAbstractVideoSurface>
#include <QVideoRendererControl>

#include "Core/VideoFrameProcessor.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// VideoSurface ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VideoSurface : public QAbstractVideoSurface {
    Q_OBJECT
public:
    VideoSurface(QWidget* widget, QObject* parent = Q_NULLPTR);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat& format, QVideoSurfaceFormat* similar) const;

    bool start(const QVideoSurfaceFormat& format);
    void stop();
    bool present(const QVideoFrame& frame);

    void updateVideoRect();
    void paint(QPainter* painter);

	void SetVideoFrameProcessor(VideoFrameProcessor* vp);
	void SetCoverWidget(QWidget* widget);
private:
    QWidget* videoWidget;
    QImage::Format imageFormat;
    QRect displayableRectRegion;
    QRect clippedRectRegion;
    QVideoFrame currentFrame;

	size_t frameNumber;
	VideoFrameProcessor* videoProcessor;

	QWidget* coverWidget;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  VideoWidget ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VideoWidget : public QWidget {
    Q_OBJECT
 public:
    VideoWidget(QWidget *parent = Q_NULLPTR);
    ~VideoWidget();

    QAbstractVideoSurface* videoSurface() const;
	void SetVideoFrameProcessor(VideoFrameProcessor* vfProcessor);
	void SetCoverWidget(QWidget* widget);

    QSize sizeHint() const;
 protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
 private:
    VideoSurface* surface;
};

#endif // VIDEOWIDGET_H
