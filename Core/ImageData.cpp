#include "Core/ImageData.h"

ImageData::ImageData()
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
{
	imageSize = Size(0, 0);
}
ImageData::ImageData(QImage* image)
	: qImageSptr(image)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
{
	imageSize = image ? Size(image->height(), image->width()) : Size(0, 0);
}
ImageData::ImageData(QVideoFrame* image)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(image)
	, rawRgba8888Sptr(Q_NULLPTR)
{
	imageSize = image ? Size(image->height(), image->width()) : Size(0, 0);
}
ImageData::ImageData(uchar* image, Size imgSize)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(image)
{
	imageSize = imgSize;
}
const QImage& ImageData::GetQImage() const {
	if(qImageSptr.isNull()) {
		const_cast<ImageData*>(this)->buildQImage();
	}
	return *qImageSptr;
}
const QVideoFrame& ImageData::GetQVideoFrame() const {
	if(qVFrameSptr.isNull()){
		const_cast<ImageData*>(this)->buildQVideoFrame();
	}
	return *qVFrameSptr;
}
const uchar* ImageData::GetRawRgba8888() const {
	if(rawRgba8888Sptr.isNull()){
		const_cast<ImageData*>(this)->buildRawRgba8888();
	}
	return rawRgba8888Sptr.data();
}
Size ImageData::GetSize() const {
	return imageSize;
}
void ImageData::buildQImage() {
	if(!qVFrameSptr.isNull()) {
    	if (qVFrameSptr->map(QAbstractVideoBuffer::ReadOnly)) {
			qImageSptr.reset(new QImage(
					qVFrameSptr->bits(),
    		    	qVFrameSptr->width(),
    		    	qVFrameSptr->height(),
    		    	qVFrameSptr->bytesPerLine(),
					QVideoFrame::imageFormatFromPixelFormat(qVFrameSptr->pixelFormat())
				)
    		);
			qVFrameSptr->unmap();
		}
	} else if(!rawRgba8888Sptr.isNull()) {
		qImageSptr.reset(new QImage(
				rawRgba8888Sptr.data(),
				imageSize.width,
				imageSize.height,
				QImage::Format_ARGB32
			)
		);
	}
}
void ImageData::buildQVideoFrame() {
	if(!qImageSptr.isNull()) {
		qVFrameSptr.reset(new QVideoFrame(
				*qImageSptr
			)
		);
	} else if(!rawRgba8888Sptr.isNull()) {
		qVFrameSptr.reset(new QVideoFrame(
				QImage(
					rawRgba8888Sptr.data(),
					imageSize.width,
					imageSize.height,
					QImage::Format_ARGB32
				)
			)
		);
	}
}
void ImageData::buildRawRgba8888() {
	if(!qImageSptr.isNull()) {
		if(qImageSptr->format() != QImage::Format_ARGB32) {
			rawRgba8888Sptr.reset(
				deepCopyOfRawBuffer(qImageSptr->convertToFormat(QImage::Format_ARGB32).constBits())
			);
		} else {
			rawRgba8888Sptr.reset(
				deepCopyOfRawBuffer(qImageSptr->constBits())
			);
		}
	} else if(!qVFrameSptr.isNull()) {
		// There is no way to convert video frame format, so build
		// QImage and then take raw data from it
		buildQImage();
		buildRawRgba8888();
	}
}
uchar* ImageData::deepCopyOfRawBuffer(const uchar* oldBuffer) {
	size_t count = imageSize.height * imageSize.width * 4;
	uchar* buffer = new uchar[count];
	for(size_t i = 0; i < count; ++i) {
		buffer[i] = oldBuffer[i];
	}
	return buffer;
}

