#include "CornerDetect.h"
vector<Point2f> CornerDetect::cornerDetect(Mat inputForCorner)
{
	cvtColor(inputForCorner, inputForCorner_gray, CV_BGR2GRAY);
	blur(inputForCorner_gray, inputForCorner_gray, Size(11, 11));
	/// Create Window
	/// Create Trackbar to set the number of corners
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 100;
	int blockSize = 30;
	bool useHarrisDetector = false;
	double k = 0.04;

	/// Copy the source image
	//Mat copy;
	//copy = src.clone();

	/// Apply corner detection
	goodFeaturesToTrack(inputForCorner_gray,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k);
	return corners;
}
