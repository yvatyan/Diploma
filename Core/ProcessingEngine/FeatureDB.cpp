#include "Core/ProcessingEngine/FeatureDB.h"
#include <QDebug>

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49


/*
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  FeatureWrapper class  ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
FeatureDB::FeatureWrapper::FeatureWrapper() {
	feat = NULL;
}
FeatureDB::FeatureWrapper::FeatureWrapper(const feature& f) {
	feat = new feature(f);
}
FeatureDB::FeatureWrapper::FeatureWrapper(const FeatureDB::FeatureWrapper& copy) {
	//qDebug() << "Delete 2" << feat;
	//if(feat) delete feat;
	//qDebug() << "Delete escape 2";
	feat = new feature(*copy.feat);
}
FeatureDB::FeatureWrapper::~FeatureWrapper() {
	//if(feat) delete feat;
}
int FeatureDB::FeatureWrapper::GetX() const {
	return feat->x;
}
int FeatureDB::FeatureWrapper::GetY() const {
	return feat->y;
}
size_t FeatureDB::FeatureWrapper::GetDescriptorSize() const {
	return feat->d;
}
double FeatureDB::FeatureWrapper::DescriptorComponentAt(size_t index) const {
	return index < feat->d ? feat->descr[index] : 0.;
}
FeatureDB::FeatureWrapper& FeatureDB::FeatureWrapper::operator=(const FeatureDB::FeatureWrapper& copy) {
//	qDebug() << "Delete " << feat;
	//if(feat) delete feat;
//	qDebug() << "Delete escape";
	feat = new feature(*copy.feat);
}
*/
/*
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  DescriptorVector class  ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int FeatureDB::DescriptorVector::threshold = 1;
FeatureDB::DescriptorVector::DescriptorVector(const std::vector<double>& descriptor)
    : descr(descriptor)
{
}
bool FeatureDB::DescriptorVector::operator==(const DescriptorVector& rhp) const {
    if(descr.size() != rhp.descr.size()) {
        return false;
    }
    for(size_t i = 0; i < descr.size(); ++i) {
        if(descr[i] < rhp.descr[i] - threshold || descr[i] > rhp.descr[i] + threshold ) {
            return false;
        }
    }
    return true;
}
bool FeatureDB::DescriptorVector::operator!=(const DescriptorVector& rhp) const {
    return !operator==(rhp);
}
bool FeatureDB::DescriptorVector::operator>=(const DescriptorVector& rhp) const {
    return operator>(rhp) || operator==(rhp);
}
bool FeatureDB::DescriptorVector::operator<=(const DescriptorVector& rhp) const {
    return operator<(rhp) || operator==(rhp);
}
bool FeatureDB::DescriptorVector::operator<(const DescriptorVector& rhp) const {
    if(descr.size() != rhp.descr.size()) {
        return false;
    }
    for(size_t i = 0; i < descr.size(); ++i) {
        if((int)descr[i]  <= (int)rhp.descr[i] - threshold) {
            return false;
        }
    }
    return true;
}
bool FeatureDB::DescriptorVector::operator>(const DescriptorVector& rhp) const {
    if(descr.size() != rhp.descr.size()) {
        return false;
    }
    for(size_t i = 0; i < descr.size(); ++i) {
        if((int)descr[i]  <= (int)rhp.descr[i] + threshold) {
            return false;
        }
    }
    return true;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  FeatureDB class  //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
double FeatureDB::threshold = 0.1;	//TODO: HARDCODE
double FeatureDB::descrDistanceThreshold = 100; //TODO: HARDCODE
FeatureDB::FeatureDB() {
	kdTreeRoot = NULL;
}
/*
std::pair<std::list<FeatureDB::FeatureWrapper>, std::list<FeatureDB::FeatureWrapper> > FeatureDB::getCommonElements(const FeatureDB& fdb) const { // passing previous frame db
	qDebug() << "GetCommon";
    typedef std::map<DescriptorVector, FeatureWrapper>::const_iterator map_iterator;
    std::list<FeatureWrapper> firstDb;
    std::list<FeatureWrapper> secondDb;
    for(map_iterator it2 = fdb.database.cbegin(), it1; it2 != fdb.database.cend(); ++it2) {
		iter ++;
        if((it1 = database.find(it2->first)) != database.cend()) {
            firstDb.push_back(it1->second);
            secondDb.push_back(it2->second);
        }
    }
	qDebug() << "Iterations: " << iter;
	qDebug() << "First: " << firstDb.size() << " Second: " << secondDb.size();
    return std::pair<std::list<FeatureWrapper>, std::list<FeatureWrapper> >(firstDb, secondDb);
}*/
std::pair<std::list<Point>, std::list<Point> > FeatureDB::getCommonElements(feature* features, size_t count) const {
	qDebug() << "Common";
	struct feature* feat;
	struct feature** nbrs;
	double d0, d1;

	std::list<Point> currentFrame;
	std::list<Point> previousFrame;
	size_t j = 0;
	for(size_t i = 0; i < count; i++ ){
	    feat = features + i;	// looping through current frame features
	    int k = kdtree_bbf_knn( kdTreeRoot, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
	    if( k == 2 ) {
			j++;
			qDebug() << j << "Found at " << feat->x << feat->y << " vs " << nbrs[0]->x << " " << nbrs[0]->y;
	    	d0 = descr_dist_sq(feat, nbrs[0]);
	    	d1 = descr_dist_sq(feat, nbrs[1]);
			qDebug() << "Distance: " << d0;
	    	if(d0 < d1 * NN_SQ_DIST_RATIO_THR && d0 <= descrDistanceThreshold) {
	  			currentFrame.push_back(Point(cvRound(feat->x), cvRound(feat->y)));
	  			previousFrame.push_back(Point(cvRound(nbrs[0]->x), cvRound(nbrs[0]->y)));
	    	    features[i].fwd_match = nbrs[0];
	    	}
	    }
		qDebug() << "Before NBRS";
	    free(nbrs);
		qDebug() << "After NBRS";
	}
	return std::pair<std::list<Point>, std::list<Point> >(currentFrame, previousFrame);
}
void FeatureDB::Update(feature* features, size_t count) {
 if(kdTreeRoot) kdtree_release(kdTreeRoot);	// TODO: MEMLEAK
 qDebug() << "Before build";
  kdTreeRoot = kdtree_build(features, count);
  qDebug() << "After build";
}
/*void FeatureDB::Add(const feature& feat) {
	static size_t count = 0;
//	qDebug() << "Add " << count;
	FeatureWrapper value(feat);
    std::vector<double> descriptor(value.GetDescriptorSize());
    for(size_t i = 0; i < value.GetDescriptorSize(); ++i) {
        descriptor[i] = value.DescriptorComponentAt(i);
//		qDebug() << descriptor[i];
    }
//	qDebug() << "Before add";
    database[DescriptorVector(descriptor)] = value;
//	qDebug() << "After add";
}*/
bool FeatureDB::IsEmpty() const {
	/*
	qDebug() << "IsEmpty? Size" << database.size();
	bool isEmpty = database.empty();
	return isEmpty;
	*/
	return !kdTreeRoot;
}
std::list<std::list<AnnotatedPoint> > FeatureDB::GetRigidObjectPointsList(feature* features, size_t count) const {
	qDebug() << "Points";
    typedef std::pair<std::list<Point>, std::list<Point> > list_pair;
    typedef std::list<Point>::const_iterator list_iterator;

    std::list<std::list<AnnotatedPoint> > objects;
    std::list<AnnotatedPoint> object;

    list_pair commonPoints = getCommonElements(features, count);
	size_t object_index = 0;
	qDebug() << "Size:" << commonPoints.first.size() << commonPoints.second.size();
    for(list_iterator it1 = commonPoints.first.cbegin(),
                      it2 = commonPoints.second.cbegin();

        it1 != commonPoints.first.cend();

        ++it1,
        ++it2,
        object.clear())
    {
        for(list_iterator it12 = it1, it22 = it2; it12 != commonPoints.first.cend();) {
            if(it12 == it1) {
				qDebug() << "Push first";
				object.push_back(AnnotatedPoint(
						*it1,
						RgbaColor(255, 0, 0, 255)
					));
            } else {
                double sqrDist1 = sqr(it12->x - it1->x) + sqr(it12->y - it1->y);
                double sqrDist2 = sqr(it22->x - it2->x) + sqr(it22->y - it2->y);
                if(abs(sqrDist1 - sqrDist2) <= threshold) {
					qDebug() << "Add AnnoPoint";
					object.push_back(AnnotatedPoint(
							*it12,
							RgbaColor(255, 0, 0, 255),
							std::to_string(object_index)
						));
					qDebug() << "Before erase";
                    it12 = commonPoints.first.erase(it12);
					qDebug() << "Before erase 2";
                    it22 = commonPoints.second.erase(it22);
					qDebug() << "After erase";
                    continue;
                }
            }
            ++it12;
            ++it22;
        }
        objects.push_back(object);
		object_index++;
    }
	qDebug() << "Before returning objects";
    return objects;
}
std::vector<AnnotatedRectangle> FeatureDB::GetRigidObjectsBoundingRegion(feature* features, size_t count) const {
	qDebug() << "Region";
    std::list<std::list<AnnotatedPoint> > objectPointsList(GetRigidObjectPointsList(features, count));
    size_t qty = objectPointsList.size();
    std::vector<AnnotatedRectangle> objectRegionsArray(qty); 
    typedef std::list<AnnotatedPoint>::const_iterator list_iterator;
    typedef std::list<std::list<AnnotatedPoint> >::const_iterator list_Mat_iterator;
    int max_x, max_y, min_x, min_y;
	size_t i = 0;
    for(list_Mat_iterator it1 = objectPointsList.cbegin(); it1 != objectPointsList.cend(); ++it1, ++i) {
        max_x = max_y = INT_MIN;
        min_x = min_y = INT_MAX;
        for(list_iterator it2 = it1->cbegin(); it2 != it1->cend(); ++it2) {
            if(it2->GetPoint().x > max_x) {
                max_x = it2->GetPoint().x;
            } else if(it2->GetPoint().x < min_x) {
                min_x = it2->GetPoint().x;
            }

            if(it2->GetPoint().y > max_y) {
                max_y = it2->GetPoint().y;
            } else if(it2->GetPoint().y < min_y) {
                min_y = it2->GetPoint().y;
            }
        }
        objectRegionsArray[i] = AnnotatedRectangle(
									Rectangle(min_x, max_y, max_y - min_y, max_x - min_x),
									RgbaColor(255, 0, 0, 255)
								);
    }
    return objectRegionsArray;
}
