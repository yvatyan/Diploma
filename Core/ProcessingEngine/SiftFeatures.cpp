#include "Core/ProcessingEngine/SiftFeatures.h"

SiftFeatures::SiftFeatures(const bool configNeeded) {
	if(configNeeded) {
		configured = false;
	} else {
		initDefaultConfig();
	}
}
VideoFrameData SiftFeatures::Process(const VideoFrameData& dataToProcess) {
  	struct feature* features;
  	size_t featCount = 0;
  	const IplImage* img = &dataToProcess.GetImageData().GetIplImage();
	featCount = _sift_features(
  					img,
					&features,
					intvls,
					sigma,
					contr_thr,
					curv_thr,
		      		img_dbl,
					descr_width,
					descr_hist_bins);
  
	std::vector<AnnotatedPoint> avgPoints(featCount);
	for(size_t i = 0; i < featCount; ++i) {
		avgPoints[i] = AnnotatedPoint(
						Point(features[i].x, features[i].y),
						RgbaColor(255, 0, 0, 255),
						std::string(""));
	}
	return VideoFrameData(
				ImageData(),
				Name(),
				dataToProcess.GetFrameNumber(),
				std::vector<AnnotatedRectangle>(),
				avgPoints);
}
void SiftFeatures::Configure(const ConfigCollection& config) {
	intvls = config.GetConfigValue<int>("Intvls");
	sigma = config.GetConfigValue<double>("Sigma");
	contr_thr = config.GetConfigValue<double>("ContrThr");
	curv_thr = config.GetConfigValue<int>("CurvThr");
	img_dbl = config.GetConfigValue<int>("ImgDbl");
	descr_width = config.GetConfigValue<int>("DescriptorWidth");
	descr_hist_bins = config.GetConfigValue<int>("HistogramBins");
	configured = true;
}
const char* SiftFeatures::Name() const {
	return "SIFT";
}
void SiftFeatures::initDefaultConfig() {
	intvls = SIFT_INTVLS;
	sigma = SIFT_SIGMA;
	contr_thr = SIFT_CONTR_THR;
	curv_thr = SIFT_CURV_THR;
	img_dbl = SIFT_IMG_DBL;
	descr_width = SIFT_DESCR_WIDTH;
	descr_hist_bins = SIFT_DESCR_HIST_BINS;
	configured = true;
}
