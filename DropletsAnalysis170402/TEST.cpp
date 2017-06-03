#pragma once
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"
using namespace std;
using namespace cv;

int main2()
{
	float maxValue = 0;  //定义距离变换矩阵中的最大值  
	Point Pt(0, 0);
	Mat src = imread("F:\\study\\毕设\\QQ图片20170413105630.PNG");

	Mat imageGray;
	cvtColor(src, imageGray, CV_RGB2GRAY);
	imageGray = ~imageGray;  //取反  
	GaussianBlur(imageGray, imageGray, Size(5, 5), 2); //滤波  
	threshold(imageGray, imageGray, 20, 200, CV_THRESH_BINARY); //阈值化     
	Mat imageThin(imageGray.size(), CV_32FC1); //定义保存距离变换结果的Mat矩阵  
	distanceTransform(imageGray, imageThin, CV_DIST_L2, 3);  //距离变换  

	Mat distShow;
	distShow = Mat::zeros(imageGray.size(), CV_8UC1); //定义细化后的字符轮廓  
	for (int i = 0; i<imageThin.rows; i++)
	{
		for (int j = 0; j<imageThin.cols; j++)
		{
			distShow.at<uchar>(i, j) = imageThin.at<float>(i, j);
			if (imageThin.at<float>(i, j)>maxValue)
			{
				maxValue = imageThin.at<float>(i, j);  //获取距离变换的极大值  
				Pt = Point(j, i);  //坐标  
			}
		}
	}
	normalize(distShow, distShow, 0, 255, CV_MINMAX); //为了显示清晰，做了0~255归一化  
	/*circle(src, Pt, maxValue, Scalar(0, 0, 255), 3);
	circle(src, Pt, 3, Scalar(0, 255, 0), 3);*/
	imshow("Source Image", src);
	imshow("Thin Image", distShow);
	imshow("d Image", imageThin);
	waitKey();
	return 0;
}