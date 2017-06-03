#pragma once
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"
using namespace std;
using namespace cv;

int main2()
{
	float maxValue = 0;  //�������任�����е����ֵ  
	Point Pt(0, 0);
	Mat src = imread("F:\\study\\����\\QQͼƬ20170413105630.PNG");

	Mat imageGray;
	cvtColor(src, imageGray, CV_RGB2GRAY);
	imageGray = ~imageGray;  //ȡ��  
	GaussianBlur(imageGray, imageGray, Size(5, 5), 2); //�˲�  
	threshold(imageGray, imageGray, 20, 200, CV_THRESH_BINARY); //��ֵ��     
	Mat imageThin(imageGray.size(), CV_32FC1); //���屣�����任�����Mat����  
	distanceTransform(imageGray, imageThin, CV_DIST_L2, 3);  //����任  

	Mat distShow;
	distShow = Mat::zeros(imageGray.size(), CV_8UC1); //����ϸ������ַ�����  
	for (int i = 0; i<imageThin.rows; i++)
	{
		for (int j = 0; j<imageThin.cols; j++)
		{
			distShow.at<uchar>(i, j) = imageThin.at<float>(i, j);
			if (imageThin.at<float>(i, j)>maxValue)
			{
				maxValue = imageThin.at<float>(i, j);  //��ȡ����任�ļ���ֵ  
				Pt = Point(j, i);  //����  
			}
		}
	}
	normalize(distShow, distShow, 0, 255, CV_MINMAX); //Ϊ����ʾ����������0~255��һ��  
	/*circle(src, Pt, maxValue, Scalar(0, 0, 255), 3);
	circle(src, Pt, 3, Scalar(0, 255, 0), 3);*/
	imshow("Source Image", src);
	imshow("Thin Image", distShow);
	imshow("d Image", imageThin);
	waitKey();
	return 0;
}