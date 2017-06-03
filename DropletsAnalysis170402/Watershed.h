#pragma once
#include "WatershedAlgorithm_3.h"

void fillHole(Mat srcBw, Mat &dstBw);
Mat DistanceTansform(Mat src); 
Mat GetBinary(Mat src);

Mat testWatershed(Mat src)
{
	Mat result;
	WatershedAlgorithm watershedAlgorithm;
	result=watershedAlgorithm.run(src);
	return result;
}
//Mat testWatershed(Mat src, Mat edges)
//{
//
//	Mat image = src;
//	Mat imageGray;
//	cvtColor(image, imageGray, CV_RGB2GRAY);//灰度转换  
//	GaussianBlur(imageGray, imageGray, Size(9, 9), 2);   //高斯滤波  
//	Mat binary = GetBinary(src);
//	namedWindow("Gray Image", CV_WINDOW_KEEPRATIO);
//	imshow("Gray Image", imageGray);
//	Canny(imageGray, imageGray, 20, 50);
//	namedWindow("Canny Image", CV_WINDOW_KEEPRATIO);
//	imshow("Canny Image", imageGray);
//
//	//查找轮廓  
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//	findContours(imageGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
//	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);  //轮廓     
//	Mat marks(image.size(), CV_32S);   //Opencv分水岭第二个矩阵参数  
//	marks = Scalar::all(0);
//	int index = 0;
//	int compCount = 0;
//	for (; index >= 0; index = hierarchy[index][0], compCount++)
//	{
//		//对marks进行标记，对不同区域的轮廓进行编号，相当于设置注水点，有多少轮廓，就有多少注水点  
//		drawContours(marks, contours, index, Scalar::all(compCount + 1), 1, 8, hierarchy);
//		drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
//	}
//
//	//我们来看一下传入的矩阵marks里是什么东西  
//	Mat marksShows;
//	convertScaleAbs(marks, marksShows);
//	//namedWindow("marksShow", CV_WINDOW_KEEPRATIO);
//	//imshow("marksShow", marksShows);
//	//namedWindow("轮廓", CV_WINDOW_KEEPRATIO);
//	//imshow("轮廓", imageContours);
//	image = DistanceTansform(src);
//	watershed(image, marks);
//
//	//我们再来看一下分水岭算法之后的矩阵marks里是什么东西  
//	Mat afterWatershed;
//	convertScaleAbs(marks, afterWatershed, 100);
//	threshold(afterWatershed, afterWatershed, 100, 255, 0);
//
//	//afterWatershed = -afterWatershed;
//	namedWindow("After Watershed", CV_WINDOW_KEEPRATIO);
//	imshow("After Watershed", afterWatershed);
//
//
//	binary = binary&afterWatershed;
//	namedWindow("After edges", CV_WINDOW_KEEPRATIO);
//	imshow("After edges", binary);
//	//Canny(binary, edges, 200, 200);
//	waitKey();
//	
//	return edges;
//}
void fillHole(Mat srcBw, Mat &dstBw)
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像  
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像  
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
Mat DistanceTansform(Mat src)
{
	Mat image = src;
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);//灰度转换  
	Mat binary;
	GaussianBlur(imageGray, imageGray, Size(5, 5), 5); //滤波  
	adaptiveThreshold(imageGray, binary, 255, 1, 0, 47, 7);

	Mat dstBinary = Mat::zeros(src.size(), CV_8UC3);


	Mat src_bw(imageGray.size(), CV_32FC1);
	Mat src_BW(imageGray.size(), CV_8UC1);

	binary = ~binary;
	fillHole(binary, binary);
	//imshow("dstBinary", binary);
	distanceTransform(binary, src_bw, CV_DIST_L12, 0);  //距离变换
	for (int i = 0; i<src_bw.rows; i++)
	{
		for (int j = 0; j<src_bw.cols; j++)
		{
			src_BW.at<uchar>(i, j) = src_bw.at<float>(i, j);
		}
	}

	normalize(src_BW, src_BW, 0, 255, CV_MINMAX); //为了显示清晰，做了0~255归一化
	src_BW = ~src_BW;
	cvtColor(src_BW, src_BW, CV_GRAY2BGR);
	return src_BW;
}
Mat GetBinary(Mat src)
{
	Mat image = src;
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);//灰度转换  
	Mat binary;
	GaussianBlur(imageGray, imageGray, Size(5, 5), 5); //滤波  
	adaptiveThreshold(imageGray, binary, 255, 1, 0, 47, 7);
	binary = ~binary;
	fillHole(binary, binary);
	return binary;
}