#ifndef VIDEOFRAMEPROCRESULT_H
#define VIDEOFRAMEPROCRESULT_H

#include <vector>
#include "Core/Misc.h"
#include "Core/ImageData.h"

class VideoFrameData {
    public:
		enum DataType {
			Empty          = 0x00,
			ImageTransform = 0x01,
			ObjectRegions  = 0x02,
			ObjectPoints   = 0x04
		};
		static VideoFrameData NoResult;
		
		VideoFrameData();
        VideoFrameData(const ImageData& img, const char* task, size_t frame, const std::vector<AnnotatedRectangle>& objects = std::vector<AnnotatedRectangle>(), const std::vector<AnnotatedPoint>& points = std::vector<AnnotatedPoint>());

        const std::vector<AnnotatedRectangle>& GetObjectRegions() const;
        const std::vector<AnnotatedPoint>& GetObjectPoints() const;
		const ImageData& GetImageData() const;
		size_t GetFrameNumber() const;
		const char* GetTaskName() const;

		bool ContainsNoResult() const;
		bool ContainsTransformationResult() const;
		bool ContainsRegionResult() const;
		bool ContainsPointedResult() const;

		void AddObjectRegions(const std::vector<AnnotatedRectangle>& regions);
		void AddObjectPoints(const std::vector<AnnotatedPoint>& points);
		void SetImageData(const ImageData& img);
		void SetTaskName(const char* task);

		void Clear();
	private:
		ImageData imgData;
		const char* taskName;
		size_t frameNumber;
		std::vector<AnnotatedRectangle> objRegions;
		std::vector<AnnotatedPoint> objPoints;
		uchar dataTypeMask;

		void setupMask();
};

#endif // VIDEOFRAMEPROCRESULT_H
