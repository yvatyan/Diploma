#include "Core/ProcessingEngine/Demo/MonoChromeProc.h"

MonoChromeProc::MonoChromeProc() {
	configured = false;
}
VideoFrameData MonoChromeProc::Process(const VideoFrameData& dataToProcess) {
	if(configured) {
		ImageData image = dataToProcess.GetImageData();
		const uchar* imageRaw = image.GetRawRgba8888();
		
		Size imageSize = image.GetSize();
		size_t count = imageSize.height*imageSize.width*4;

		uchar* imageRes = new uchar[count];
		uchar red, green, blue;
		size_t colorShift;
		std::vector<AnnotatedPoint> avgPoints;
		switch(ChromeMode) {
			case Red	: {
				colorShift = 2;
				break;
			}
			case Green	: {
				colorShift = 1;
				break;
			}
			default : {
				colorShift = 0;
			}
		};
		for(int i = 0; i < count; i += 4) {
			switch(ChromeMode) {
				case Red	: {
					green = blue = 0;
					red = imageRaw[i+2];
					colorShift = 2;
					break;
				}
				case Green	: {
					red = blue = 0;
					green = imageRaw[i+1];
					colorShift = 1;
					break;
				}
				case Blue	: {
					red = green = 0;
					blue = imageRaw[i+0];
					break;
				}
				case White	: {
					red = green = blue = (imageRaw[i+0] + imageRaw[i+1] + imageRaw[i+2])/3;
				}
			};
			imageRes[i] = blue;
			imageRes[i+1] = green;
			imageRes[i+2] = red;
			imageRes[i+3] = 0xff;
			if(ChromeMode != White) {
				if(_INTERVAL <= imageRes[i + colorShift] && imageRes[i + colorShift] <= INTERVAL_) {
					avgPoints.push_back(AnnotatedPoint(
							Point((i/4)%imageSize.width, i/(4*imageSize.width)),
							RgbaColor(255, 0, 0, 255),
							std::string("Pxl=")+std::to_string(imageRaw[i+colorShift])));
				}
			}
		}
		std::vector<AnnotatedRectangle> avgWindows;
		for(size_t i = 0; i < imageSize.height; i += KernelSize) {
			for(size_t j = 0; j < imageSize.width*4; j += KernelSize*4) {
				size_t sum = 0, count = 0;
				for(size_t ker_i = i; ker_i < i + KernelSize && ker_i < imageSize.height; ++ker_i) {
					for(size_t ker_j = j; ker_j < j + KernelSize*4 && ker_j < imageSize.width*4; ker_j += 4) {
						sum += imageRes[ker_i*imageSize.width*4 + ker_j + colorShift];
						++count;
					}
				}
				size_t avg = sum/count;
				if(avg < Threshold) {
					avgWindows.push_back(AnnotatedRectangle(
							Rectangle(j/4, i, KernelSize, KernelSize),
							RgbaColor(255, 255, 0, 255),
							std::string("Avg=")+std::to_string(avg)));
				}
			}
		}
		if(MakeTransformation) {
			ImageData resultImage(imageRes, image.GetSize());
			return VideoFrameData(
				resultImage,
				Name(),
				dataToProcess.GetFrameNumber(),
				avgWindows,
				avgPoints
			);
		} else {
			return VideoFrameData(
				ImageData(),
				Name(),
				dataToProcess.GetFrameNumber(),
				avgWindows,
				avgPoints
			);
		}
	}
	return VideoFrameData::NoResult;
}
void MonoChromeProc::Configure(const ConfigCollection& config) {
	KernelSize = config.GetConfigValue<size_t>("KernelSize");
	ChromeMode = config.GetConfigValue<Mode>("ChromeMode");
	Threshold = config.GetConfigValue<size_t>("Threshold");
	MakeTransformation = config.GetConfigValue<bool>("MakeTransformation");
	_INTERVAL = config.GetConfigValue<size_t>("PixelIntervalStart");
	INTERVAL_ = config.GetConfigValue<size_t>("PixelIntervalEnd");
	configured = true;
}
const char* MonoChromeProc::Name() const {
	return "Monochrome processing";
}
