#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <vector>
#include <QImage>
#include <QVideoFrame>
#include <QSharedPointer>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/types_c.h>
#include "Core/Misc.h"

class ImageData {
    public:
		ImageData();
        ImageData(QImage* image);
        ImageData(QVideoFrame* image);
		ImageData(uchar* image, Size imgSize);
		ImageData(cv::Mat* cvMatImg);
		ImageData(IplImage* cvIplImage);

        const QImage& GetQImage() const;
        const QVideoFrame& GetQVideoFrame() const;
		const uchar* GetRawRgba8888() const;
		const cv::Mat& GetcvMat() const;
		const IplImage& GetIplImage() const;

        Size GetSize() const;
    private:
        // Based which constructor was called, one of the following will be initialized.
        // Others will be initialized only when they will be needed.
        QSharedPointer<QImage> qImageSptr;
        QSharedPointer<QVideoFrame> qVFrameSptr;
		QSharedPointer<uchar> rawRgba8888Sptr;
		QSharedPointer<cv::Mat> cvMatSptr;
		QSharedPointer<IplImage> cvIplImageSptr;

        Size imageSize;

		void buildQImage();
		void buildQVideoFrame();
		void buildRawRgba8888();
		void buildCvMat();
		void buildCvIplImage();

		uchar* deepCopyOfRawBuffer(const uchar* oldBuffer);
		uchar* convertBGR888ToRGBA8888(const uchar* cvBuffer);
		uchar* convertRGBA8888ToBGR888(const uchar* buffer);
};

#endif // IMAGEDATA_H
