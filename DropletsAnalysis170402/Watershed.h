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
//	cvtColor(image, imageGray, CV_RGB2GRAY);//�Ҷ�ת��  
//	GaussianBlur(imageGray, imageGray, Size(9, 9), 2);   //��˹�˲�  
//	Mat binary = GetBinary(src);
//	namedWindow("Gray Image", CV_WINDOW_KEEPRATIO);
//	imshow("Gray Image", imageGray);
//	Canny(imageGray, imageGray, 20, 50);
//	namedWindow("Canny Image", CV_WINDOW_KEEPRATIO);
//	imshow("Canny Image", imageGray);
//
//	//��������  
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//	findContours(imageGray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
//	Mat imageContours = Mat::zeros(image.size(), CV_8UC1);  //����     
//	Mat marks(image.size(), CV_32S);   //Opencv��ˮ��ڶ����������  
//	marks = Scalar::all(0);
//	int index = 0;
//	int compCount = 0;
//	for (; index >= 0; index = hierarchy[index][0], compCount++)
//	{
//		//��marks���б�ǣ��Բ�ͬ������������б�ţ��൱������עˮ�㣬�ж������������ж���עˮ��  
//		drawContours(marks, contours, index, Scalar::all(compCount + 1), 1, 8, hierarchy);
//		drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
//	}
//
//	//��������һ�´���ľ���marks����ʲô����  
//	Mat marksShows;
//	convertScaleAbs(marks, marksShows);
//	//namedWindow("marksShow", CV_WINDOW_KEEPRATIO);
//	//imshow("marksShow", marksShows);
//	//namedWindow("����", CV_WINDOW_KEEPRATIO);
//	//imshow("����", imageContours);
//	image = DistanceTansform(src);
//	watershed(image, marks);
//
//	//����������һ�·�ˮ���㷨֮��ľ���marks����ʲô����  
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
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//��չͼ��  
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//�ü���չ��ͼ��  
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
Mat DistanceTansform(Mat src)
{
	Mat image = src;
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);//�Ҷ�ת��  
	Mat binary;
	GaussianBlur(imageGray, imageGray, Size(5, 5), 5); //�˲�  
	adaptiveThreshold(imageGray, binary, 255, 1, 0, 47, 7);

	Mat dstBinary = Mat::zeros(src.size(), CV_8UC3);


	Mat src_bw(imageGray.size(), CV_32FC1);
	Mat src_BW(imageGray.size(), CV_8UC1);

	binary = ~binary;
	fillHole(binary, binary);
	//imshow("dstBinary", binary);
	distanceTransform(binary, src_bw, CV_DIST_L12, 0);  //����任
	for (int i = 0; i<src_bw.rows; i++)
	{
		for (int j = 0; j<src_bw.cols; j++)
		{
			src_BW.at<uchar>(i, j) = src_bw.at<float>(i, j);
		}
	}

	normalize(src_BW, src_BW, 0, 255, CV_MINMAX); //Ϊ����ʾ����������0~255��һ��
	src_BW = ~src_BW;
	cvtColor(src_BW, src_BW, CV_GRAY2BGR);
	return src_BW;
}
Mat GetBinary(Mat src)
{
	Mat image = src;
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);//�Ҷ�ת��  
	Mat binary;
	GaussianBlur(imageGray, imageGray, Size(5, 5), 5); //�˲�  
	adaptiveThreshold(imageGray, binary, 255, 1, 0, 47, 7);
	binary = ~binary;
	fillHole(binary, binary);
	return binary;
}