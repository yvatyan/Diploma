#include "Core/ImageData.h"

ImageData::ImageData()
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
	, cvMatSptr(Q_NULLPTR)
	, cvIplImageSptr(Q_NULLPTR)
{
	imageSize = Size(0, 0);
}
ImageData::ImageData(QImage* image)
	: qImageSptr(image)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
	, cvMatSptr(Q_NULLPTR)
	, cvIplImageSptr(Q_NULLPTR)
{
	imageSize = image ? Size(image->height(), image->width()) : Size(0, 0);
}
ImageData::ImageData(QVideoFrame* image)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(image)
	, rawRgba8888Sptr(Q_NULLPTR)
	, cvMatSptr(Q_NULLPTR)
	, cvIplImageSptr(Q_NULLPTR)
{
	imageSize = image ? Size(image->height(), image->width()) : Size(0, 0);
}
ImageData::ImageData(uchar* image, Size imgSize)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(image)
	, cvMatSptr(Q_NULLPTR)
	, cvIplImageSptr(Q_NULLPTR)
{
	imageSize = imgSize;
}
ImageData::ImageData(cv::Mat* cvMatImg)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
	, cvMatSptr(cvMatImg)
	, cvIplImageSptr(Q_NULLPTR)
{
}
ImageData::ImageData(IplImage* cvIplImage)
	: qImageSptr(Q_NULLPTR)
	, qVFrameSptr(Q_NULLPTR)
	, rawRgba8888Sptr(Q_NULLPTR)
	, cvMatSptr(Q_NULLPTR)
	, cvIplImageSptr(cvIplImage)
{
}
const QImage& ImageData::GetQImage() const {
	if(qImageSptr.isNull()) {
		const_cast<ImageData*>(this)->buildQImage();
	}
	return *qImageSptr;
}
const QVideoFrame& ImageData::GetQVideoFrame() const {
	if(qVFrameSptr.isNull()) {
		const_cast<ImageData*>(this)->buildQVideoFrame();
	}
	return *qVFrameSptr;
}
const uchar* ImageData::GetRawRgba8888() const {
	if(rawRgba8888Sptr.isNull()) {
		const_cast<ImageData*>(this)->buildRawRgba8888();
	}
	return rawRgba8888Sptr.data();
}
const cv::Mat& ImageData::GetcvMat() const {
	if(cvMatSptr.isNull()) {
		const_cast<ImageData*>(this)->buildCvMat();
	}
	return *cvMatSptr;
}
const IplImage& ImageData::GetIplImage() const {
	if(cvIplImageSptr.isNull()) {
		const_cast<ImageData*>(this)->buildCvIplImage();
	}
	return *cvIplImageSptr;
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
	} else if(!cvMatSptr.isNull()) {
		qImageSptr.reset(new QImage(
				(uchar*) cvMatSptr.data()->data,
				imageSize.width,
				imageSize.height,
				(*cvMatSptr).step,
				QImage::Format_RGB888
			)
		);
	} else if(!cvIplImageSptr.isNull()) {
		cv::Mat tmp = cv::cvarrToMat(cvIplImageSptr.data(), false);
		qImageSptr.reset(new QImage(
				(uchar*) tmp.data,
				imageSize.width,
				imageSize.height,
				tmp.step,
				QImage::Format_RGB888
			)
		);
	}
}
void ImageData::buildQVideoFrame() {
	if(!qImageSptr.isNull()) {
		qVFrameSptr.reset( new QVideoFrame(
				*qImageSptr
			)
		);
	} else if(!rawRgba8888Sptr.isNull()) {
		qVFrameSptr.reset( new QVideoFrame(
				QImage(
					rawRgba8888Sptr.data(),
					imageSize.width,
					imageSize.height,
					QImage::Format_ARGB32
				)
			)
		);
	} else if(!cvMatSptr.isNull()) {
		qVFrameSptr.reset( new QVideoFrame(
				QImage(
					(uchar*) cvMatSptr.data()->data,
					imageSize.width,
					imageSize.height,
					(*cvMatSptr).step,
					QImage::Format_RGB888
				)
			)
		);
	} else if(!cvIplImageSptr.isNull()) {
		cv::Mat tmp = cv::cvarrToMat(cvIplImageSptr.data(), false);
		qVFrameSptr.reset( new QVideoFrame(
				QImage(
					(uchar*) tmp.data,
					imageSize.width,
					imageSize.height,
					tmp.step,
					QImage::Format_RGB888
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
	} else if(!cvMatSptr.isNull()) {
		rawRgba8888Sptr.reset(
			convertBGR888ToRGBA8888((*cvMatSptr).data)
		);
	} else if(!cvIplImageSptr.isNull()) {
		cv::Mat tmp = cv::cvarrToMat(cvIplImageSptr.data(), false);
		rawRgba8888Sptr.reset(
			convertBGR888ToRGBA8888(tmp.data)
		);
	}
}
void ImageData::buildCvMat() {
	if(!qImageSptr.isNull()) {
		if(qImageSptr->format() != QImage::Format_ARGB32) {
			cvMatSptr.reset(
				new cv::Mat(
					imageSize.height,
					imageSize.width,
					CV_8UC3,	
					convertRGBA8888ToBGR888(qImageSptr->convertToFormat(QImage::Format_ARGB32).constBits())	// possibly memory-leak, if cv::Mat is not responsible for deleting data passed as pointer
				)
			);
		} else {
			rawRgba8888Sptr.reset(
				convertRGBA8888ToBGR888(qImageSptr->constBits())
			);
		}
	} else if(!qVFrameSptr.isNull()) {
		buildQImage();
		buildCvMat();
	} else if(!rawRgba8888Sptr.isNull()) {
		cvMatSptr.reset(
			new cv::Mat(
				imageSize.height,
				imageSize.width,
				CV_8UC3,	
				convertRGBA8888ToBGR888(rawRgba8888Sptr.data()) // possibly memory-leak, if cv::Mat is not responsible for deleting data passed as pointer
			)
		);
	} else if(!cvIplImageSptr.isNull()) {
		cvMatSptr.reset(
			new cv::Mat(cv::cvarrToMat(cvIplImageSptr.data(), false))
		);
	}
}
void ImageData::buildCvIplImage() {
	if(cvMatSptr.isNull()) {
		buildCvMat();
		buildCvIplImage();
	} else {
		cvIplImageSptr.reset( new IplImage(*cvMatSptr));
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
uchar* ImageData::convertBGR888ToRGBA8888(const uchar* cvBuffer) {
	size_t count = imageSize.height * imageSize.width * 3;
	uchar* buffer = new uchar[(count/3)*4];
	for(size_t i = 0, j = 0; i < count; i += 3, j += 4) {
		buffer[j]   = cvBuffer[i+2];
		buffer[j+1] = cvBuffer[i+1];
		buffer[j+2] = cvBuffer[i];
		buffer[j+3] = 255;
	}
	return buffer;
}
uchar* ImageData::convertRGBA8888ToBGR888(const uchar* buffer) {
	size_t count = imageSize.height * imageSize.width * 4;
	uchar* bufferCv = new uchar[(count/4)*3];
	for(size_t i = 0, j = 0; i < count; i += 4, j += 3) {
		bufferCv[j] = buffer[i+2];
		bufferCv[j+1] = buffer[i+1];
		bufferCv[j+2] = buffer[i];
	}
	return bufferCv;
}
