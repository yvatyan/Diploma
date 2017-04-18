#include "Core/ThreadManager/ThreadRunnableTask.h"

ThreadRunnableTask::ThreadRunnableTask() {
	isFree = true;
	isComplete = false;
	setAutoDelete(false);
}
void ThreadRunnableTask::run() {
	isComplete = false;
	isFree = false;
	for(size_t i = 0; i < jobsChain.size(); ++i) {
		procResult = (i ? jobsChain[i]->Process(procResult) : jobsChain[i]->Process(procData));
	}
	procResult.SetTaskName(name.toStdString().c_str());
	if(procResult.ContainsNoResult()) {
		isFree = true;
	}
	isComplete = true;
}
QString ThreadRunnableTask::TaskName() const {
	return name;
}
void ThreadRunnableTask::SetTaskName(const char* task) {
	name = QString(task);
}
QStringList ThreadRunnableTask::JobsNames() const {
	QStringList jobsNames;
	for(size_t i = 0; i < jobsChain.size(); ++i) {
		jobsNames << jobsChain[i]->Name();
	}
	return jobsNames;
}
void ThreadRunnableTask::AddJob(Job* job) {
	jobsChain.append(job);
}
void ThreadRunnableTask::ConfigureJob(QString job, const ConfigCollection& config) {
	for(size_t i = 0; i < jobsChain.size(); ++i) {
		if(jobsChain[i]->Name() == job) {
			jobsChain[i]->Configure(config);
		}
	}
}
void ThreadRunnableTask::SetProcData(const VideoFrameData& dataToProceed) {
	procData = dataToProceed;
}
const VideoFrameData& ThreadRunnableTask::GetResult() const {
	isFree = true;
	return procResult;
}
const VideoFrameData* ThreadRunnableTask::GetResultDataPointer() const {
	isFree = true;
	return &procResult;
}
bool ThreadRunnableTask::IsComplete() const {
	return isComplete;
}
bool ThreadRunnableTask::IsFree() const {
	return isFree;
}

