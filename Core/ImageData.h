#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <vector>
#include <QImage>
#include <QVideoFrame>
#include <QSharedPointer>
#include "Core/Misc.h"

class ImageData {
    public:
		ImageData();
        ImageData(QImage* image);
        ImageData(QVideoFrame* image);
		ImageData(uchar* image, Size imgSize);

        const QImage& GetQImage() const;
        const QVideoFrame& GetQVideoFrame() const;
		const uchar* GetRawRgba8888() const;

        Size GetSize() const;
    private:
        // Based which constructor was called, one of the following will be initialized.
        // Others will be initialized only when they will be needed.
        QSharedPointer<QImage> qImageSptr;
        QSharedPointer<QVideoFrame> qVFrameSptr;
		QSharedPointer<uchar> rawRgba8888Sptr;

        Size imageSize;

		void buildQImage();
		void buildQVideoFrame();
		void buildRawRgba8888();

		uchar* deepCopyOfRawBuffer(const uchar* oldBuffer);
};

#endif // IMAGEDATA_H
