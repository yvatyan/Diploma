#include "VideoWidget.h"
#include <QVideoSurfaceFormat>
#include <QPainter>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  VideoSurface ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VideoSurface::VideoSurface(QWidget* widget, QObject* parent)
                : QAbstractVideoSurface(parent)
                , widget(widget)
                , imageFormat(QImage::Format_Invalid)
{
}
QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                    << QVideoFrame::Format_RGB32
                    << QVideoFrame::Format_ARGB32
                    << QVideoFrame::Format_ARGB32_Premultiplied
                    << QVideoFrame::Format_RGB565
                    << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
bool VideoSurface::isFormatSupported(
        const QVideoSurfaceFormat& format, QVideoSurfaceFormat* similar) const
{
    Q_UNUSED(similar);

    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}
bool VideoSurface::start(const QVideoSurfaceFormat& format) {
    if (isFormatSupported(format, NULL)) {
        imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());

        QAbstractVideoSurface::start(format);

        widget->updateGeometry();
        updateVideoRect();
        clippedRectRegion = QRect(QPoint(0, 0), displayableRectRegion.size());
        return true;
    } else {
        return false;
    }
}
void VideoSurface::stop() {
    currentFrame = QVideoFrame();
    displayableRectRegion = QRect();
    clippedRectRegion = QRect();
    QAbstractVideoSurface::stop();
    widget->update();
}
bool VideoSurface::present(const QVideoFrame& frame) {
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
        || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();
        return false;
    } else {
        currentFrame = frame;
        widget->repaint(displayableRectRegion);
        return true;
    }
}
void VideoSurface::updateVideoRect() {
    QSize size = surfaceFormat().sizeHint();
    size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);

    displayableRectRegion = QRect(QPoint(0, 0), size);
    displayableRectRegion.moveCenter(widget->rect().center());
}
void VideoSurface::paint(QPainter* painter) {
    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage image(
            currentFrame.bits(),
            currentFrame.width(),
            currentFrame.height(),
            currentFrame.bytesPerLine(),
            imageFormat
        );
        painter->drawImage(displayableRectRegion, image, clippedRectRegion);
        currentFrame.unmap();
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  VideoWidget ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VideoWidget::VideoWidget(QWidget *parent)
        : QWidget(parent)
        , surface(0)
{
    surface = new VideoSurface(this);
}
VideoWidget::~VideoWidget() {
    delete surface;
}
QSize VideoWidget::sizeHint() const {
    return surface->surfaceFormat().sizeHint();
}
void VideoWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    surface->paint(&painter);
}
void VideoWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    surface->updateVideoRect();
}
