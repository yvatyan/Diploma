#ifndef THREADRUNNABLETASK_H
#define THREADRUNNABLETASK_H

#include <QRunnable>
#include "Core/VideoFrameData.h"
#include "Core/Job.h"

class ThreadRunnableTask : public QRunnable {
	public:
		ThreadRunnableTask();

		void run();

		QString TaskName() const;
		QStringList JobsNames() const;
		void AddJob(Job* job);
		void ConfigureJob(QString job, const ConfigCollection& config);
		void SetProcData(const VideoFrameData& dataToProceed);
		void SetTaskName(const char* task);
		const VideoFrameData& GetResult() const;
		const VideoFrameData* GetResultDataPointer() const;
		bool IsComplete() const;
		bool IsFree() const;
	private:
		QList<Job*> jobsChain;
		VideoFrameData procResult;
		VideoFrameData procData;
		bool isComplete;
		mutable bool isFree;
		QString name;
};

#endif
