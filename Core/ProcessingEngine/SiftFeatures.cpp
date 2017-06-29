#include "Core/ProcessingEngine/SiftFeatures.h"
#include <QDebug>

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
	qDebug() << "--------" << featCount;
	if(!database.IsEmpty()) {
		//std::vector<AnnotatedRectangle> boundingRegions(database.GetRigidObjectsBoundingRegion(features, featCount));
		//qDebug() << "Regions count: " << boundingRegions.size();
		typedef std::list<AnnotatedPoint>::const_iterator list_iterator;
		typedef std::list<std::list<AnnotatedPoint> >::const_iterator list_list_iterator;
		std::list<std::list<AnnotatedPoint> > objectPoints(database.GetRigidObjectPointsList(features, featCount));	
		std::vector<AnnotatedPoint> annoPoints;
		for(list_list_iterator it1 = objectPoints.cbegin(); it1 != objectPoints.cend(); ++it1) {
			for(list_iterator it2 = it1->cbegin(); it2 != it1->cend(); ++it2) {
				annoPoints.push_back(*it2);
			}
		}
		database.Update(features, featCount);
		return VideoFrameData(
					ImageData(),
					Name(),
					dataToProcess.GetFrameNumber(),
					//boundingRegions);
					std::vector<AnnotatedRectangle>(),
					annoPoints);
	} else {
		database.Update(features, featCount);
		return VideoFrameData::NoResult;
	}
	// free( features ); TODO: Should make it somewhere to avois memory leak
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
