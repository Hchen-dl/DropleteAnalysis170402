#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"

//#include "ContoursDetect.h"
#include "WatershedAlgorithm_3.h"

//#include "Watershed.h"
#include "GetCharacters.h"
//#include "ModifiedContours.h"
#include "GetRect.h"
#include "CornerDetect.h"

using namespace std;
using namespace cv;

Mat src;
Mat binary;
//Mat src_gray;
Mat edges;
//Mat dst;

//int thresh1 = 200;
//int thresh2 = 200;
//int max_thresh = 255;
vector<double>radius;
Rect ROI=Rect(0,0,260,760);

Rect GetROI();
//void Thresh(int, void*);
//void DrawContours();
int SortingContours();
//int test();

int a(int argc, char** argv)
{
	//char* fileName = "F:\\study\\毕设\\IMG_0626.JPG";
	src=imread(argv[1],1);
	//src = imread(fileName, 1);
	if (!src.data)
	{
		printf("empty");	waitKey();	return -1;
	}
	GetRect getrect;
	src= getrect.getRect(src,ROI);
	Rect roi(30, 30, src.cols - 30, src.rows - 30);
	src = src(roi);
	//namedWindow("region", CV_WINDOW_AUTOSIZE);
	//imshow("region",src);
	////waitKey();
	Canny(src, edges, 200, 200);
	namedWindow("Edges", 4);
	imshow("Edges", edges);
	//////edges=ModifiedContours(edges);
	WatershedAlgorithm watershedAlgorithm(src);
	binary = watershedAlgorithm.mainProcess();
	//binary=testWatershed(src);
	//cvtColor(src, src_gray,0);
	//src = src_gray&~binary;
	edges = edges | binary;
	namedWindow("output", 4);
	imshow("output", edges);
	//DrawContours();
	//createTrackbar("thresh1", "Binary", &thresh1, max_thresh, Thresh);
	//createTrackbar("thresh2", "Binary", &thresh2, max_thresh, Thresh);

	//namedWindow("output", 4);
	//waitKey();
	SortingContours();
	GetCharacters getchartacters;
	getchartacters.GettingCharacters(src,roi, radius);
	waitKey();
	return 0;
}
//void Thresh(int, void*)
//{
//	Canny(src, edges, thresh1, thresh2,5);
//	cout << "+!" << endl;
//	imshow("output", edges);
//
//}
int test()
{
	/*Mat src = cv::imread("F:\\study\\毕设\\IMG_0630.JPG");
	Mat dst = Mat::zeros(src.size(), CV_8UC3);
	Mat src_gray= Mat::zeros(src.size(), CV_8UC3);
	resize(src, src, Size(src.cols / 6, src.rows / 6), 0, 0, CV_INTER_LINEAR);
	if (!src.data)
		return -1;
	blur(src, dst, Size(7, 7));
	cvtColor(dst, src_gray, CV_BGR2GRAY);
	Canny(src_gray, edges, 25, 25);
	imshow("src", src);
	imshow("edges",edges);
	waitKey(0);*/
	return 0;
}

int SortingContours()
{
	vector<vector<Point> > contours_out;
	vector<Vec4i> hierarchy_out;
	findContours(edges, contours_out, hierarchy_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy_all;
	findContours(edges, contours, hierarchy_all, RETR_TREE, CHAIN_APPROX_NONE);

	if (contours.size() == contours_out.size()) return -1;//没有内轮廓，则提前返回  

	for (int i = 0; i < contours_out.size(); i++)
	{
		int conloursize = contours_out[i].size();
		for (int j = 0; j < contours.size(); j++)
		{
			int tem_size = contours[j].size();
			if (conloursize == tem_size)//等效于contours_out[i]==contours[j]
			{
				swap(contours[j], contours[contours.size() - 1]);
				contours.pop_back();
				break;
			}
		}
	}
	//
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = i + 1; j < contours.size(); j++)
		{
			if (arcLength(contours[i], 1) > arcLength(contours[j], 1))
			{
				swap(contours[i], contours[j]);
				//vector<Point>temp = contours[i];
				//contours[i] = contours[j];
				//contours[j] = temp;
			}
		}
		if (arcLength(contours[i], 1) == 0)
			radius.push_back(0.5);
		else
		{
			radius.push_back(sqrt(contourArea(contours[i]) / 3.14));
		}
		//cout <<arcLength( contours[i],1 )<<"___"<<i<< endl;
	}
	return 0;
}
//
//void DrawContours()
//{
//	vector<vector<Point>>contours;
//	vector<Vec4i>hierarchy;
//	//Canny(src_gray, canny_output, thresh, thresh * 2, 3);
//	//threshold(src_gray, canny_output, thresh, 255,THRESH_BINARY);
//	//namedWindow("src_gray", CV_WINDOW_AUTOSIZE);
//	//imshow("src_gray", canny_output);
//	findContours(edges, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
//	//for (int i = 0; i < contours.size(); i++) { ModifiedContours(contours[i]); cout << endl; }
//	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
//
//		/*if (contourArea(contours[i]) == 0)
//		{
//		color = Scalar(0, 0, 255);
//		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
//		}
//		if (arcLength(contours[i], 1) == 0)
//		{
//		color = Scalar(0, 255, 0);
//		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
//		}*/
//	}
//	imshow("drawing", drawing);
//	//imwrite("after_blur_and_getROI.jpg", drawing);
//}
