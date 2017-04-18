#ifndef JOB_H
#define JOB_H

#include "Core/ConfigCollection.hpp"
#include "Core/VideoFrameData.h"

class Job {
	public:
		virtual VideoFrameData Process(const VideoFrameData& dataToProcess) = 0;
		virtual void Configure(const ConfigCollection& config){};
		virtual const char* Name() const = 0;
		virtual ~Job(){}
};

#endif // JOB_H
