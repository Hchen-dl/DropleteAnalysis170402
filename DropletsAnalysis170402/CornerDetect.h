#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

class CornerDetect
{
private:
	Mat inputForCorner_gray;
	int maxCorners = 200;
	int maxTrackbar = 200;
	//RNG rng(12345);
	//char* source_window = "Image";

	/// Function header
	//void goodFeaturesToTrack_Demo(int, void*);
public:
	vector<Point2f> cornerDetect(Mat inputForCorner);

};
//Mat src, src_gray;

/*
* @function main
*/
/*
vector<Point2f> CornerDetect(Mat inputForCorner)
{
	//Mat inputForCorner_gray;
	/// Load source image and convert it to gray
	//resize(inputForCorner, src, Size(src.cols / 6, src.rows / 6), 0, 0, CV_INTER_LINEAR);
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
	//cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 30;
	Mat copy;
	copy = inputForCorner_gray.clone();
	for (int i = 0; i < corners.size(); i++)
	{
		circle(copy, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}

	/// Show what you got
	//namedWindow("corners", 4);
	//imshow("corners", copy);
	//waitKey();
	return corners;

	/// Show what you got
	//namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	//imshow(source_window, copy);
	//waitKey();
	//return corners;

	//createTrackbar("Max  corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo);
	////imshow(source_window, src);

	//goodFeaturesToTrack_Demo(0, 0);
	//waitKey(0);
	//return corners;
}

/**
* @function goodFeaturesToTrack_Demo.cpp
* @brief Apply Shi-Tomasi corner detector
*/
/*
void goodFeaturesToTrack_Demo(int, void*)
{
	if (maxCorners < 1) { maxCorners = 1; }

	/// Parameters for Shi-Tomasi algorithm
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 11;
	bool useHarrisDetector = false;
	double k = 0.04;

	/// Copy the source image
	Mat copy;
	copy = inputForCorner_gray.clone();
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


	/// Draw corners detected
	cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 4;
	for (int i = 0; i < corners.size(); i++)
	{
		circle(copy, corners[i], r, Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
			rng.uniform(0, 255)), -1, 8, 0);
	}

	/// Show what you got
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, copy);
}
*/