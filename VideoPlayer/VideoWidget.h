#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QAbstractVideoSurface>
#include <QVideoRendererControl>

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
private:
    QWidget* widget;
    QImage::Format imageFormat;
    QRect displayableRectRegion;
    QRect clippedRectRegion;
    QVideoFrame currentFrame;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  VideoWidget ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VideoWidget : public QWidget {
     Q_OBJECT
 public:
     VideoWidget(QWidget *parent = Q_NULLPTR);
     ~VideoWidget();

     QAbstractVideoSurface* videoSurface() const { return surface; }

     QSize sizeHint() const;

 protected:
     void paintEvent(QPaintEvent* event);
     void resizeEvent(QResizeEvent* event);

 private:
     VideoSurface* surface;
};

#endif // VIDEOWIDGET_H
