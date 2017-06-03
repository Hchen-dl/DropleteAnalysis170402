#pragma once
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"
#include "GetCharacters.h"
#include "watershed.h"
#include "ModifiedContours.h"
using namespace std;
using namespace cv;

Mat src;
Mat src_gray;
Mat dst;
Mat bf;

Mat edges;
Rect ROI;
RNG rng(123345);
vector<double>radius_sorted;

Rect GetROI();
void DrawContours();
void SortingContours();

int testFindContours()
{
	char* fileName = "F:\\study\\����\\IMG_0629.JPG";
	
	src = imread(fileName, 1);
	if (!src.data)
	{
		printf("empty");
		waitKey();
		return -1;
	}
	//the source image from the phone is too large to deal with.
	resize(src, src, Size(src.cols / 6, src.rows / 6), 0, 0, CV_INTER_LINEAR);
	//ROI = Rect(src.cols / 10, src.rows/10, src.cols / 2, src.rows/2);
	//src=src(ROI);
	testWatershed(src, "F:\\study\\����\\IMG_0629.JPG");

	/*blur(src, dst, Size(3, 3));
	cvtColor(src, src_gray, CV_BGR2GRAY);
	Canny(src_gray, edges, 200, 200);*/
	//imshow("edges", edges);
	//ROI = GetROI();
	//src=src(GetROI());
	//Canny(src, edges, 200, 200);
	//imshow("ROI", src);
	//DrawContours();
	//SortingContours();
	//GettingCharacters(ROI,radius_sorted);
	waitKey();
}
Rect GetROI()
{
	//�ƽ����õ�����,ע��ͼ�εĳ�����������
	/*Rect rect(src.cols/3,src.rows/3,src.cols/3,src.rows/3);
	src = src(rect);*/
	int max_area_index;
	double max_area = 0;
	Rect max_rect;
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// �ҵ�����
	findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// ����αƽ����� + ��ȡ����(��Բ�α߽��)
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		/*cout<<contourArea(contours[i])<<endl;
		cout<<arcLength(contours[i],1)<<endl;*/
		if (boundRect[i].width*boundRect[i].height >= max_area)
		{
			max_area_index = i;
			max_area = boundRect[i].width*boundRect[i].height;
		}
		//minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}
	return boundRect[max_area_index];

}

void DrawContours()
{
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	//Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	//threshold(src_gray, canny_output, thresh, 255,THRESH_BINARY);
	namedWindow("src_gray", CV_WINDOW_AUTOSIZE);
	//imshow("src_gray", canny_output);
	findContours(edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//for (int i = 0; i < contours.size(); i++) { ModifiedContours(contours[i]); cout << endl; }


	Mat drawing = Mat::zeros(src.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		
		/*if (contourArea(contours[i]) == 0)
		{
			color = Scalar(0, 0, 255);
			drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		}
		if (arcLength(contours[i], 1) == 0)
		{
			color = Scalar(0, 255, 0);
			drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		}*/
	}
	imshow("drawing", drawing);
	//imwrite("after_blur_and_getROI.jpg", drawing);
}

///������ܳ�����

void SortingContours()
{	
	vector<Vec4i>hierarchy;
	vector<vector<Point>>contours;

	//Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	//threshold(src_gray, canny_output, thresh, 255,THRESH_BINARY);
	namedWindow("src_gray", CV_WINDOW_AUTOSIZE);
	//imshow("src_gray", canny_output);
	findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = i + 1; j < contours.size(); j++)
		{
			if (arcLength(contours[i], 1) > arcLength(contours[j], 1))
			{
				vector<Point>temp = contours[i];
				contours[i] = contours[j];
				contours[j] = temp;
			}			
		}
		radius_sorted.push_back( arcLength(contours[i], 1)/3.14);
		//cout <<arcLength( contours[i],1 )<<"___"<<i<< endl;
	}	
}
