#ifndef VIDEOFRAMEPROCESSOR_H
#define VIDEOFRAMEPROCESSOR_H

#include <vector>
#include <QList>
#include "Core/ThreadManager/ThreadRunnableTask.h"
#include "Core/VideoFrameData.h"

class VideoFrameProcessor {
	public:
		void AddRunnableTask(const ThreadRunnableTask& task);
		void Process(const VideoFrameData& frameData);
		void ConfigureJob(const QString task, const QString job, const ConfigCollection& config);
		const VideoFrameData* GetResultDataPointer() const;
		const VideoFrameData& GetResult() const;
		bool HasCompleteTask() const;
		bool HasFreeTask() const;
	private:
		static bool multiThreadingEnabled;
		VideoFrameData procResult;
		QList<ThreadRunnableTask> runnableTasks;

		void mergeTaskResults();
};

#endif	// VIDEOFRAMEPROCESSOR_H
