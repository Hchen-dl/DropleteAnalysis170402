#pragma once
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"
#include "WatershedSegmenter.h"

using namespace std;
using namespace cv;
int test()
{
	// Read input image  

	cv::Mat src = cv::imread("F:\\study\\毕设\\IMG_0629.JPG");
	if (!src.data)
		return 0;
	// Display the color image 
	resize(src, src, Size(src.cols / 6, src.rows / 6), 0, 0, CV_INTER_LINEAR);
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", src);

	// Identify image pixels with object  
	Mat src_gray;
	Mat binary;
	cv::cvtColor(src, src_gray, COLOR_BGRA2GRAY);
	cv::threshold(src_gray, binary, 120, 255, CV_THRESH_BINARY); //阈值分割原图的灰度图，获得二值图像  
	// Display the binary image														  
	cv::namedWindow("binary Image");
	cv::imshow("binary Image", binary);  

	//// CLOSE operation  
	//cv::Mat element5(3, 3, CV_8U, cv::Scalar(1));//5*5正方形，8位uchar型，全1结构元素  
	//cv::Mat fg1;
	//cv::morphologyEx(binary, fg1, cv::MORPH_CLOSE, element5, Point(-1, -1), 1);// 闭运算填充物体内细小空洞、连接邻近物体  

	//// Display the foreground image																		     
	//cv::namedWindow("Foreground Image");
	//cv::imshow("Foreground Image", fg1);
	//

	//cv::Mat bg1;
	//cv::dilate(binary, bg1, cv::Mat(), cv::Point(-1, -1), 4);//膨胀4次，锚点为结构元素中心点  
	//cv::threshold(bg1, bg1, 1, 128, cv::THRESH_BINARY_INV);//>=1的像素设置为128（即背景）  
	//													   // Display the background image  
	//cv::namedWindow("Background Image");
	//cv::imshow("Background Image", bg1);
	////waitKey();

	Mat markers1;
	distanceTransform(binary, markers1, 4, 3, 5);
	

	//Mat markers1 = fg1 + bg1; //使用Mat类的重载运算符+来合并图像。  
	cv::namedWindow("markers Image");
	cv::imshow("markers Image", markers1);
	//waitKey();

	// Apply watershed segmentation  

	WatershedSegmenter segmenter1;  //实例化一个分水岭分割方法的对象  
	segmenter1.setMarkers(markers1);//设置算法的标记图像，使得水淹过程从这组预先定义好的标记像素开始  
	segmenter1.process(src);     //传入待分割原图  

									// Display segmentation result  
	cv::namedWindow("Segmentation1");
	cv::imshow("Segmentation1", segmenter1.getSegmentation());//将修改后的标记图markers转换为可显示的8位灰度图并返回分割结果（白色为前景，灰色为背景，0为边缘）  
	//waitKey();
	// Display watersheds  
	cv::namedWindow("Watersheds1");
	cv::imshow("Watersheds1", segmenter1.getWatersheds());//以图像的形式返回分水岭（分割线条）  
	//waitKey();

	Mat maskimage = segmenter1.getSegmentation();
	cv::threshold(maskimage, maskimage, 250, 1, THRESH_BINARY);
	cv::cvtColor(maskimage, maskimage, COLOR_GRAY2BGR);

	maskimage = src.mul(maskimage);
	cv::namedWindow("maskimage");
	cv::imshow("maskimage", maskimage);
	//waitKey();

	// Turn background (0) to white (255)  
	int nl = maskimage.rows; // number of lines  
	int nc = maskimage.cols * maskimage.channels(); // total number of elements per line  

	for (int j = 0; j<nl; j++) {
		uchar* data = maskimage.ptr<uchar>(j);
		for (int i = 0; i<nc; i++)
		{
			// process each pixel ---------------------  
			if (*data == 0) //将背景由黑色改为白色显示  
				*data = 255;
			data++;//指针操作：如为uchar型指针则移动1个字节，即移动到下1列  
		}
	}
	cv::namedWindow("result");
	cv::imshow("result", maskimage);
	waitKey();
}


