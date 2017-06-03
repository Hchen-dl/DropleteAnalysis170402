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

	cv::Mat src = cv::imread("F:\\study\\����\\IMG_0629.JPG");
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
	cv::threshold(src_gray, binary, 120, 255, CV_THRESH_BINARY); //��ֵ�ָ�ԭͼ�ĻҶ�ͼ����ö�ֵͼ��  
	// Display the binary image														  
	cv::namedWindow("binary Image");
	cv::imshow("binary Image", binary);  

	//// CLOSE operation  
	//cv::Mat element5(3, 3, CV_8U, cv::Scalar(1));//5*5�����Σ�8λuchar�ͣ�ȫ1�ṹԪ��  
	//cv::Mat fg1;
	//cv::morphologyEx(binary, fg1, cv::MORPH_CLOSE, element5, Point(-1, -1), 1);// ���������������ϸС�ն��������ڽ�����  

	//// Display the foreground image																		     
	//cv::namedWindow("Foreground Image");
	//cv::imshow("Foreground Image", fg1);
	//

	//cv::Mat bg1;
	//cv::dilate(binary, bg1, cv::Mat(), cv::Point(-1, -1), 4);//����4�Σ�ê��Ϊ�ṹԪ�����ĵ�  
	//cv::threshold(bg1, bg1, 1, 128, cv::THRESH_BINARY_INV);//>=1����������Ϊ128����������  
	//													   // Display the background image  
	//cv::namedWindow("Background Image");
	//cv::imshow("Background Image", bg1);
	////waitKey();

	Mat markers1;
	distanceTransform(binary, markers1, 4, 3, 5);
	

	//Mat markers1 = fg1 + bg1; //ʹ��Mat������������+���ϲ�ͼ��  
	cv::namedWindow("markers Image");
	cv::imshow("markers Image", markers1);
	//waitKey();

	// Apply watershed segmentation  

	WatershedSegmenter segmenter1;  //ʵ����һ����ˮ��ָ���Ķ���  
	segmenter1.setMarkers(markers1);//�����㷨�ı��ͼ��ʹ��ˮ�͹��̴�����Ԥ�ȶ���õı�����ؿ�ʼ  
	segmenter1.process(src);     //������ָ�ԭͼ  

									// Display segmentation result  
	cv::namedWindow("Segmentation1");
	cv::imshow("Segmentation1", segmenter1.getSegmentation());//���޸ĺ�ı��ͼmarkersת��Ϊ����ʾ��8λ�Ҷ�ͼ�����طָ�������ɫΪǰ������ɫΪ������0Ϊ��Ե��  
	//waitKey();
	// Display watersheds  
	cv::namedWindow("Watersheds1");
	cv::imshow("Watersheds1", segmenter1.getWatersheds());//��ͼ�����ʽ���ط�ˮ�루�ָ�������  
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
			if (*data == 0) //�������ɺ�ɫ��Ϊ��ɫ��ʾ  
				*data = 255;
			data++;//ָ���������Ϊuchar��ָ�����ƶ�1���ֽڣ����ƶ�����1��  
		}
	}
	cv::namedWindow("result");
	cv::imshow("result", maskimage);
	waitKey();
}


