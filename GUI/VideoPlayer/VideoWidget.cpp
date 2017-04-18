#include "GUI/VideoPlayer/VideoWidget.h"
#include <QVideoSurfaceFormat>
#include <QPainter>
#include <QDebug>
#include "GUI/VideoPlayer/CoverWidget.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////  VideoSurface ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VideoSurface::VideoSurface(QWidget* widget, QObject* parent)
                : QAbstractVideoSurface(parent)
                , videoWidget(widget)
                , imageFormat(QImage::Format_Invalid)
{
	videoProcessor = Q_NULLPTR;
	coverWidget = Q_NULLPTR;
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
void VideoSurface::SetVideoFrameProcessor(VideoFrameProcessor* vfProcessor) {
	videoProcessor = vfProcessor;
}
void VideoSurface::SetCoverWidget(QWidget* widget) {
	coverWidget = widget;
}
bool VideoSurface::start(const QVideoSurfaceFormat& format) {
    if (isFormatSupported(format, NULL)) {
        imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());

        QAbstractVideoSurface::start(format);

        videoWidget->updateGeometry();
        updateVideoRect();
        clippedRectRegion = QRect(QPoint(0, 0), displayableRectRegion.size());
		frameNumber = 0;
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
    videoWidget->update();
	if(coverWidget) {
		static_cast<CoverWidget*>(coverWidget)->ResetRegions();
	}
}
bool VideoSurface::present(const QVideoFrame& frame) {
	frameNumber ++;
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
        || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();
        return false;
    } else {
        currentFrame = frame;
    	if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
			currentFrame.unmap();
			if(videoProcessor && videoProcessor->HasFreeTask()) {
				if(!coverWidget) {
					videoProcessor->Process(
						VideoFrameData(
							ImageData(new QVideoFrame(currentFrame)),
							NULL,
							frameNumber
						)
					);
				} else {
					videoProcessor->Process(
						VideoFrameData(
							ImageData(new QVideoFrame(currentFrame)),
							NULL,
							frameNumber,
							static_cast<CoverWidget*>(coverWidget)->GetAnnoRectRegions()
						)
					);
				}
			}
        	videoWidget->repaint(displayableRectRegion);
        	return true;
		}
    }
}
void VideoSurface::updateVideoRect() {
    QSize size = surfaceFormat().sizeHint();
    size.scale(videoWidget->size().boundedTo(size), Qt::KeepAspectRatio);

    displayableRectRegion = QRect(QPoint(0, 0), size);
    displayableRectRegion.moveCenter(videoWidget->rect().center());

	coverWidget->setGeometry(displayableRectRegion);
	coverWidget->update();
}
void VideoSurface::paint(QPainter* painter) {
	bool showOriginFrame = true;
	if (videoProcessor && videoProcessor->HasCompleteTask()) {
		const VideoFrameData* data = videoProcessor->GetResultDataPointer();
		if(data->ContainsTransformationResult()) {
    	    painter->drawImage(displayableRectRegion, data->GetImageData().GetQImage(), clippedRectRegion);
			showOriginFrame = false;
		}
		if(data->ContainsRegionResult()) {
			if(coverWidget) {
				static_cast<CoverWidget*>(coverWidget)->SetAnnoRectRegions(data->GetObjectRegions());
			}
		}
	} 
	if(showOriginFrame) {
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// VideoWidget ////////////////////////////////////////////////////////
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
QAbstractVideoSurface* VideoWidget::videoSurface() const {
	return surface;
}
void VideoWidget::SetVideoFrameProcessor(VideoFrameProcessor* vfProcessor) {
	surface->SetVideoFrameProcessor(vfProcessor);
}
void VideoWidget::SetCoverWidget(QWidget* widget) {
	surface->SetCoverWidget(widget);
}
