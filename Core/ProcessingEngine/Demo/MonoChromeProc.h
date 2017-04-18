#include "Core/Job.h"

class MonoChromeProc : public Job {
	public:
	enum Mode {
		Red,
		Green,
		Blue,
		White
	};

	MonoChromeProc();
	VideoFrameData Process(const VideoFrameData& dataToProcess);
	void Configure(const ConfigCollection& config);
	const char* Name() const;

	private:
	// Options to configure
	size_t KernelSize;
	Mode ChromeMode;
	size_t Threshold;
	bool MakeTransformation;
	// end
	bool configured;
};
