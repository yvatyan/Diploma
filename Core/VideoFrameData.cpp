#include "Core/VideoFrameData.h"

VideoFrameData VideoFrameData::NoResult = VideoFrameData();
VideoFrameData::VideoFrameData() {
	dataTypeMask = Empty;
}
VideoFrameData::VideoFrameData(const ImageData& img, const char* task, size_t frame, const std::vector<AnnotatedRectangle>& objects, const std::vector<AnnotatedPoint>& points)
	: imgData(img)
	, objRegions(objects)
	, objPoints(points)
{
	taskName = task;
	frameNumber = frame;
	setupMask();
}
const std::vector<AnnotatedRectangle>& VideoFrameData::GetObjectRegions() const {
	return objRegions;
}
const std::vector<AnnotatedPoint>& VideoFrameData::GetObjectPoints() const {
	return objPoints;
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
bool VideoFrameData::ContainsPointedResult() const {
	return dataTypeMask & ObjectPoints;
}
void VideoFrameData::AddObjectRegions(const std::vector<AnnotatedRectangle>& regions) {
	for(size_t i = 0; i < regions.size(); ++i) {
		bool skipRectangle = false;
		for(size_t j = 0; j < objRegions.size(); ++j) {
			if(regions[i] == objRegions[j]) {
				skipRectangle = true;
				break;
			}
		}
		if(!skipRectangle) {
			objRegions.push_back(regions[i]);
		}
	}
	setupMask();
}
void VideoFrameData::AddObjectPoints(const std::vector<AnnotatedPoint>& points) {
	for(size_t i = 0; i < points.size(); ++i) {
		bool skipPoint = false;
		for(size_t j = 0; j < objPoints.size(); ++j) {
			if(points[i] == objPoints[j]) {
				skipPoint = true;
				break;
			}
		}
		if(!skipPoint) {
			objPoints.push_back(points[i]);
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
	if(objPoints.size() != 0) {
		dataTypeMask |= ObjectPoints;
	}
}
void VideoFrameData::Clear() {
	objRegions.clear();
	objPoints.clear();
	// Image data is not erased because it is not added, only fully reset
}
