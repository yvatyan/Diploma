#include "Core/VideoFrameData.h"

VideoFrameData VideoFrameData::NoResult = VideoFrameData();
VideoFrameData::VideoFrameData() {
	dataTypeMask = Empty;
}
VideoFrameData::VideoFrameData(const ImageData& img, const char* task, size_t frame, const std::vector<AnnotatedRectangle>& objects)
	: imgData(img)
	, objRegions(objects)
{
	taskName = task;
	frameNumber = frame;
	setupMask();
}
const std::vector<AnnotatedRectangle>& VideoFrameData::GetObjectRegions() const {
	return objRegions;
}
const ImageData& VideoFrameData::GetImageData() const {
	return imgData;
}
size_t VideoFrameData::GetFrameNumber() const {
	return frameNumber;
}
const char* VideoFrameData::GetTaskName() const {
	return taskName;
}
bool VideoFrameData::ContainsNoResult() const {
	return !dataTypeMask;
}
bool VideoFrameData::ContainsTransformationResult() const {
	return dataTypeMask & ImageTransform;
}
bool VideoFrameData::ContainsRegionResult() const {
	return dataTypeMask & ObjectRegions;
}
void VideoFrameData::AddObjectRegions(const std::vector<AnnotatedRectangle>& regions) {
	for(size_t i = 0; i < regions.size(); ++i) {
		bool skipRectangle = false;
		for(size_t j = 0; j < objRegions.size(); ++j) {
			if( regions[i].GetStdString().size() &&
					objRegions[i].GetStdString().size()) {
			}
			if(regions[i] == objRegions[j]) {
				skipRectangle = true;
				break;
			}
		}
		if(!skipRectangle) {
			objRegions.push_back(regions[i]);
		} else {
		}
	}
	setupMask();
}
void VideoFrameData::SetImageData(const ImageData& img) {
	imgData = img;
	setupMask();
}
void VideoFrameData::SetTaskName(const char* task) {
	taskName = task;
}
void VideoFrameData::setupMask() {
	dataTypeMask = 0;
	if(imgData.GetSize() != Size(0, 0)) {
		dataTypeMask |= ImageTransform;
	}
	if(objRegions.size() != 0) {
		dataTypeMask |= ObjectRegions;
	}
}
void VideoFrameData::Clear() {
	objRegions.clear();
	// Image data is not erased because it is not added, only fully reset
}
