#include "Core/VideoFrameProcessor.h"
#include "Job.h"
#include <cassert>
#include <QThreadPool>
#define NDEBUG

bool VideoFrameProcessor::multiThreadingEnabled = true;
void VideoFrameProcessor::AddRunnableTask(const ThreadRunnableTask& task) {
	runnableTasks.append(task);
}
void VideoFrameProcessor::Process(const VideoFrameData& frameData) {
	for(size_t i = 0; i < runnableTasks.size(); ++i) {
		if(runnableTasks[i].IsFree()) {
			runnableTasks[i].SetProcData(frameData);
			if(multiThreadingEnabled) {
				QThreadPool::globalInstance()->start(&runnableTasks[i]);
			} else {
				runnableTasks[i].run();
			}
		}
	}
}
void VideoFrameProcessor::ConfigureJob(const QString task, const QString job, const ConfigCollection& config) {
	for(size_t i = 0; i < runnableTasks.size(); ++i) {
		if(runnableTasks[i].TaskName() == task) {
			runnableTasks[i].ConfigureJob(job, config);
		}
	}
}
const VideoFrameData* VideoFrameProcessor::GetResultDataPointer() const {
	const_cast<VideoFrameProcessor*>(this)->mergeTaskResults();
	return &procResult;
}
const VideoFrameData& VideoFrameProcessor::GetResult() const {
	const_cast<VideoFrameProcessor*>(this)->mergeTaskResults();
	return procResult;
}
bool VideoFrameProcessor::HasCompleteTask() const {
	for(size_t i = 0; i < runnableTasks.size(); ++i) {
		if(runnableTasks[i].IsComplete()) {
			return true;
		}
	}
	return false;
}
bool VideoFrameProcessor::HasFreeTask() const {
	for(size_t i = 0; i < runnableTasks.size(); ++i) {
		if(runnableTasks[i].IsFree()) {
			return true;
		}
	}
	return false;
}
void VideoFrameProcessor::mergeTaskResults() {
	bool transformResult = false;
	procResult.Clear();
	for(size_t i = 0; i < runnableTasks.size(); ++i) {
		if(runnableTasks[i].IsComplete()) {
			const VideoFrameData* result = runnableTasks[i].GetResultDataPointer();
			if(result->ContainsRegionResult()) {
				procResult.AddObjectRegions(runnableTasks[i].GetResult().GetObjectRegions());
			}
			if(result->ContainsTransformationResult()) {
				assert(!transformResult);
				procResult.SetImageData(result->GetImageData());
				transformResult = true;
			}
			if(result->ContainsPointedResult()) {
				procResult.AddObjectPoints(runnableTasks[i].GetResult().GetObjectPoints());
			}
		}
	}
}
