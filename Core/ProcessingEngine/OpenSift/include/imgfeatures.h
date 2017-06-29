#include <QDebug>
/**@file
   Functions and structures for dealing with image features.
   
   Copyright (C) 2006-2012  Rob Hess <rob@iqengines.com>

   @version 1.1.2-20100521
*/

#ifndef IMGFEATURES_H
#define IMGFEATURES_H

#include "opencv2/imgproc/imgproc.hpp"

/** FEATURE_OXFD <BR> FEATURE_LOWE */
enum feature_type
  {
    FEATURE_OXFD,
    FEATURE_LOWE,
  };

/** FEATURE_FWD_MATCH <BR> FEATURE_BCK_MATCH <BR> FEATURE_MDL_MATCH */
enum feature_match_type
  {
    FEATURE_FWD_MATCH,
    FEATURE_BCK_MATCH,
    FEATURE_MDL_MATCH,
  };


/* colors in which to display different feature types */
#define FEATURE_OXFD_COLOR CV_RGB(255,255,0)
#define FEATURE_LOWE_COLOR CV_RGB(255,0,255)

/** max feature descriptor length */
#define FEATURE_MAX_D 128


/**
   Structure to represent an affine invariant image feature.  The fields
   x, y, a, b, c represent the affine region around the feature:

   a(x-u)(x-u) + 2b(x-u)(y-v) + c(y-v)(y-v) = 1
*/
struct feature
{
  feature() {
	  init_default();
  }
  feature(const feature& copy) {
//	  qDebug() << "feature CTOR";
	  init_copy(copy);
//	  qDebug() << "feature CTOR -- end";
  }
  feature& operator=(const feature& copy) {
//	  qDebug() << "feature AO";
	  init_copy(copy);
//	  qDebug() << "feature AO -- end";
  }
  ~feature(){}
  double x;                      /**< x coord */
  double y;                      /**< y coord */
  double a;                      /**< Oxford-type affine region parameter */
  double b;                      /**< Oxford-type affine region parameter */
  double c;                      /**< Oxford-type affine region parameter */
  double scl;                    /**< scale of a Lowe-style feature */
  double ori;                    /**< orientation of a Lowe-style feature */
  int d;                         /**< descriptor length */
  double descr[FEATURE_MAX_D];   /**< descriptor */
  int type;                      /**< feature type, OXFD or LOWE */
  int category;                  /**< all-purpose feature category */
  struct feature* fwd_match;     /**< matching feature from forward image */
  struct feature* bck_match;     /**< matching feature from backmward image */
  struct feature* mdl_match;     /**< matching feature from model */
  CvPoint2D64f img_pt;           /**< location in image */
  CvPoint2D64f mdl_pt;           /**< location in model */
  void* feature_data;            /**< user-definable data */
  private:
  	void init_default() {
		d = 0;
		fwd_match = NULL;
		bck_match = NULL;
		mdl_match = NULL;
		feature_data = NULL;
	}
	void init_copy(const feature& copy) {
		x = copy.x;
		y = copy.y;
		a = copy.a;
		b = copy.b;
		c = copy.c;
		scl = copy.scl;
		ori = copy.ori;
		d = copy.d;
		for(size_t i = 0; i < d; ++i) {
			descr[i] = copy.descr[i];
		}
		type = copy.type;
		category = copy.category;
		fwd_match = copy.fwd_match;
		bck_match = copy.bck_match;
		mdl_match = copy.mdl_match;
		img_pt = copy.img_pt;
		mdl_pt = copy.mdl_pt;
		feature_data = copy.feature_data;
	}
};


/**
   Reads image features from file.  The file should be formatted either as
   from the code provided by the Visual Geometry Group at Oxford or from
   the code provided by David Lowe.
   
   
   @param filename location of a file containing image features
   @param type determines how features are input.  If \a type is FEATURE_OXFD,
     the input file is treated as if it is from the code provided by the VGG
     at Oxford: http://www.robots.ox.ac.uk:5000/~vgg/research/affine/index.html
     <BR><BR>
     If \a type is FEATURE_LOWE, the input file is treated as if it is from
     David Lowe's SIFT code: http://www.cs.ubc.ca/~lowe/keypoints  
   @param feat pointer to an array in which to store imported features; memory
     for this array is allocated by this function and must be released by
     the caller using free(*feat)
   
   @return Returns the number of features imported from filename or -1 on error
*/
extern int import_features( char* filename, int type, struct feature** feat );


/**
   Exports a feature set to a file formatted depending on the type of
   features, as specified in the feature struct's type field.

   @param filename name of file to which to export features
   @param feat feature array
   @param n number of features 

   @return Returns 0 on success or 1 on error
*/
extern int export_features( char* filename, struct feature* feat, int n );


/**
   Displays a set of features on an image

   @param img image on which to display features
   @param feat array of Oxford-type features
   @param n number of features
*/
extern void draw_features( IplImage* img, struct feature* feat, int n );


/**
   Calculates the squared Euclidian distance between two feature descriptors.
   
   @param f1 first feature
   @param f2 second feature

   @return Returns the squared Euclidian distance between the descriptors of
     \a f1 and \a f2.
*/
extern double descr_dist_sq( struct feature* f1, struct feature* f2 );


#endif
