#include "Core/Job.h"
#include "Core/ProcessingEngine/FeatureDB.h"
#include "Core/ProcessingEngine/OpenSift/include/sift.h"
#include "Core/ProcessingEngine/OpenSift/include/imgfeatures.h"

class SiftFeatures : public Job {
	public:
		SiftFeatures(const bool configNeeded = false);
		VideoFrameData Process(const VideoFrameData& dataToProcess);
		void Configure(const ConfigCollection& config);
		const char* Name() const;
	private:
		// Options to configure
		int intvls;
		double sigma;
		double contr_thr;
		int curv_thr;
		int img_dbl;
		int descr_width;
		int descr_hist_bins;
		// end
		bool configured;
		void initDefaultConfig();
		FeatureDB database;
};
