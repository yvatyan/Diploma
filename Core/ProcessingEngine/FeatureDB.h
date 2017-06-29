#include <vector>
#include <map>
#include <list>
#include "Core/Misc.h"
#include "Core/ProcessingEngine/OpenSift/include/imgfeatures.h"
#include "Core/ProcessingEngine/OpenSift/include/kdtree.h"
#include <QDebug>

class FeatureDB {
	public:
		FeatureDB();
		/*
		FeatureDB(const FeatureDB& copy) {
			qDebug() << "CTOR";
		}
		FeatureDB& operator=(const FeatureDB& copy) {
			qDebug() << "Assignment operator";
			qDebug() << "DescriptorVector ao";
			std::vector<double> testVec(2);
			testVec[0] = 888.; testVec[1] = 9908.;
			DescriptorVector test(testVec);
			DescriptorVector opy = test;
			qDebug() << "FeatureWrapper ao";
			feature testF;
			testF.x = 90;
			testF.y = 89;
			FeatureWrapper testW(testF);
			qDebug() << "ao";
			FeatureWrapper popy = testW;
			qDebug() << "Map Assignment operator";
			database = copy.database;
			qDebug() << "END";
		}
		*/
		//void Add(const feature& feat);
		void Update(feature* features, size_t count);
		bool IsEmpty() const;
		
		std::list<std::list<AnnotatedPoint> > GetRigidObjectPointsList(feature* features, size_t count) const;
		std::vector<AnnotatedRectangle> GetRigidObjectsBoundingRegion(feature* features, size_t count) const;
	private:
	/*
		struct FeatureWrapper {
			FeatureWrapper();
			FeatureWrapper(const feature& f);
			FeatureWrapper(const FeatureWrapper& copy);
			~FeatureWrapper();

			int GetX() const;
			int GetY() const;
    		size_t GetDescriptorSize() const;
			double DescriptorComponentAt(size_t index) const;

			FeatureWrapper& operator=(const FeatureWrapper& copy);
			private:
				feature* feat;
		};
		struct DescriptorVector {
			DescriptorVector(const std::vector<double>& descriptor); 
			
			bool operator==(const DescriptorVector& rhp) const;
			bool operator!=(const DescriptorVector& rhp) const;
			bool operator>=(const DescriptorVector& rhp) const;
			bool operator<=(const DescriptorVector& rhp) const;
			bool operator<(const DescriptorVector& rhp) const;
			bool operator>(const DescriptorVector& rhp) const;
			private:
				static unsigned int threshold;
				std::vector<double> descr;
		};
	*/	
		static double threshold;
		static double descrDistanceThreshold;
  		kd_node* kdTreeRoot;
		//std::map<DescriptorVector, FeatureWrapper> database;
		
		std::pair<std::list<Point>, std::list<Point> > getCommonElements(feature* features, size_t count) const;
		inline double sqr(double x) const {
			return x*x;
		}
};
