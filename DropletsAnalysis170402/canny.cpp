#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"

using namespace std;
using namespace cv;
Mat src_, src_gray_;
Mat dst_, detected_edges_;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
* @���� CannyThreshold
* @��飺 trackbar �����ص� - Canny��ֵ�������1:3
*/
void CannyThreshold(int, void*)
{
	/// ʹ�� 3x3�ں˽���

	/// ����Canny����
	//test
	Canny(src_gray_, detected_edges_, lowThreshold, lowThreshold*ratio, kernel_size);

	/// ʹ�� Canny���������Ե��Ϊ������ʾԭͼ��
	dst_ = Scalar::all(0);

	src_.copyTo(dst_, detected_edges_);
	imshow(window_name, dst_);
}


/** @���� main */
int main(int argc, char** argv)
{
	/// װ��ͼ��
	src_ = imread("F:\\study\\����\\quad.jpg");

	if (!src_.data)
	{
		return -1;
	}
	Rect roi(30, 30, src_.cols - 30, src_.rows - 30);
	src_ = src_(roi);
	/// ������srcͬ���ͺʹ�С�ľ���(dst)
	dst_.create(src_.size(), src_.type());

	/// ԭͼ��ת��Ϊ�Ҷ�ͼ��
	cvtColor(src_, src_gray_, CV_BGR2GRAY);

	/// ������ʾ����
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// ����trackbar
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// ��ʾͼ��
	CannyThreshold(0, 0);

	/// �ȴ��û���Ӧ
	waitKey(0);

	return 0;
}