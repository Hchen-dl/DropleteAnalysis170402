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
* @函数 CannyThreshold
* @简介： trackbar 交互回调 - Canny阈值输入比例1:3
*/
void CannyThreshold(int, void*)
{
	/// 使用 3x3内核降噪

	/// 运行Canny算子
	Canny(src_gray_, detected_edges_, lowThreshold, lowThreshold*ratio, kernel_size);

	/// 使用 Canny算子输出边缘作为掩码显示原图像
	dst_ = Scalar::all(0);

	src_.copyTo(dst_, detected_edges_);
	imshow(window_name, dst_);
}


/** @函数 main */
int main(int argc, char** argv)
{
	/// 装载图像
	src_ = imread("F:\\study\\毕设\\quad.jpg");

	if (!src_.data)
	{
		return -1;
	}
	Rect roi(30, 30, src_.cols - 30, src_.rows - 30);
	src_ = src_(roi);
	/// 创建与src同类型和大小的矩阵(dst)
	dst_.create(src_.size(), src_.type());

	/// 原图像转换为灰度图像
	cvtColor(src_, src_gray_, CV_BGR2GRAY);

	/// 创建显示窗口
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// 创建trackbar
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// 显示图像
	CannyThreshold(0, 0);

	/// 等待用户反应
	waitKey(0);

	return 0;
}