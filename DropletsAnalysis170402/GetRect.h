// affine transformation.cpp : 定义控制台应用程序的入口点。  
//  

#include <opencv.hpp>  

/**
* Automatic perspective correction for quadrilateral objects. See the tutorial at
* http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
*/
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>  
#include "math.h"
#include "CornerDetect.h"
class GetRect
{
private:
	Point2f center;
	bool IsOneSide(cv::Vec4i a, cv::Vec4i b);
	bool IsClose(cv::Vec4i a, cv::Vec4i b);

	cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b);
	double Distance(Point2f Point1, Point2f Point2);
	void sortCorners(std::vector<cv::Point2f>& corners,
		cv::Point2f center);
public :	
	Mat getRect(Mat& src, Rect ROI);
};
/*
cv::Point2f center(0, 0);

//int thresh1=25;
//int thresh2 = 25;
//int linelenth = 10;
//int thresh_max = 255;
//cv::Mat src;
//cv::Mat src_gray;
//Mat edges = Mat::zeros(src.size(), CV_8UC3);;
//
//void testCanny(int, void*);

//计算两线角度判断是否平行（当两线平行并靠近时判定为同一边）
bool IsOneSide(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float up = (x4 - x3)*(x2 - x1) + (y4 - y3)*(y2 - y1);
	float under = sqrt(((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))*sqrt(((x4 - x3)*(x4 - x3) + (y4 - y3)*(y4 - y3)));
	float cos_q = up / under;//计算cos(sita)；
	if (cos_q > 0.5)return true;
	return false;
}

//计算交点

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float denom;
	if(x1==0&&y1==0&&x2==0&&y2==0)return Point2f(-1, -1);

	if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		if ((x1 - x2) == 0)
		{
			if (x3 - x4 == 0)return Point2f(-1, -1);
			else 
			{
				pt.x = x1;
				pt.y = ((y3 - y4)*x1 + x3*y4 - x4*y3) / (x3 - x4);
			}
		}
		else if ((x3 - x4) == 0)
		{
			pt.x = x3;
			pt.y = ((y2 - y1)*x3 + x2*y1 - x1*y2) / (x2 - x1);
		}
		else
		{
			pt.x = (x1 * y2 - y1 * x2)/d * (x3 - x4) - (x1 - x2)/d * (x3 * y4 - y3 * x4);
			pt.y = -(x2 * y1 - y2 * x1) * ((y3 - y4)/d) + ((y1 - y2)/d) * (x4 * y3 - y4 * x3);
		}
  		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

//计算两线是否靠近
bool IsClose(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float length = sqrt((x1 - x2)*(x1 - x2) + (y2 - y1)*(y2 - y1));
	float distance = 0.5*sqrt((x4 + x3 - x2 - x1)*(x4 + x3 - x2 - x1) + (y4 + y3 - y2 - y1)*(y4 + y3 - y2 - y1));
	if(distance<0.2*length)return true;
	return false;
}
//计算两线距离
double Distance(Point2f Point1, Point2f Point2)
{
	double distance = sqrt((Point1.x-Point2.x)*(Point1.x - Point2.x)+ (Point1.y - Point2.y)*(Point1.y - Point2.y));
	return distance;
}


void sortCorners(std::vector<cv::Point2f>& corners,
	cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;

	for (int i = 0; i < corners.size(); i++)
	{
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	corners.clear();

	if (top.size() == 2 && bot.size() == 2) {
		cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
		cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
		cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
		cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];


		corners.push_back(tl);
		corners.push_back(tr);
		corners.push_back(br);
		corners.push_back(bl);
	}
}

Mat GetRect(Mat& src,Rect ROI)
{
	//src = cv::imread("F:\\study\\毕设\\IMG_0626.JPG");
	//resize(src, src, Size(src.cols / 6, src.rows / 6), 0, 0, CV_INTER_LINEAR);
	if (!src.data);
		//return src;
	//Mat src_gray = Mat::zeros(src.size(), CV_8UC3);
	Mat src_gray;
	cv::cvtColor(src, src_gray, CV_BGR2GRAY);
	cv::blur(src_gray, src_gray, cv::Size(17, 17));
	cv::Canny(src_gray, src_gray, 2, 17);//通过createTrackbar调节后两个参数

	//namedWindow("src_gray", 4);	
	//imshow("src_gray", src_gray);
	//waitKey();
	std::vector<cv::Vec4i> lines;
   	cv::HoughLinesP(src_gray, lines, 0.5, CV_PI / 180, 30, 100, 10);//通过createTrackbar调节后三个参数。

	//for (size_t i = 0; i < lines.size(); i++)
	//{
	//	Vec4i l = lines[i];
	//	line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	//}
	//namedWindow("linesbefore", 4);
	//imshow("linesbefore", src);
	//waitKey(0);
	//Expand the lines  
	for (int i = 0; i < lines.size(); i++)
	{
		cv::Vec4i v = lines[i];
		if (abs(v[2] - v[0]) >abs( v[3] - v[1]))
		{
			lines[i][0] = 0;
			lines[i][1] = ((float)v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1];
			lines[i][2] = src.cols;
			lines[i][3] = ((float)v[1] - v[3]) / (v[0] - v[2]) * (src.cols - v[2]) + v[3];
		}
		else
		{
			lines[i][0]= ((float)v[0] - v[2]) / (v[1] - v[3]) * -v[1] + v[0];
			lines[i][1] = 0;
			lines[i][2] = ((float)v[0] - v[2]) / (v[1] - v[3])*(src.rows - v[1]) + v[0];
			lines[i][3] = src.rows;
		}

	}

	//每条边只提取一条线，其余的线置为(0,0),(0,0);
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = i + 1; j < lines.size(); j++)
		{
			if (IsOneSide(lines[i], lines[j]) && IsClose(lines[i],lines[j]))
			{
				lines[j][0] = 0;
				lines[j][1] = 0;
				lines[j][2] = 0;
				lines[j][3] = 0;
			}
		}
	}

	std::vector<cv::Point2f> corners;
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = i + 1; j < lines.size(); j++)
		{
			cv::Point2f pt = computeIntersect(lines[i], lines[j]);
			if (pt.x >= 0 && pt.y >= 0)
				corners.push_back(pt);
		}
	}
	//cout << corners.size()<<endl;
	std::vector<cv::Point2f> corners_Harris;
	corners_Harris = CornerDetect(src);

	for (int corners_index = 0; corners_index < corners.size(); corners_index++)
	{
		for (int Harris_index = 0; Harris_index < corners_Harris.size(); Harris_index++)
		{
			if (Distance(corners[corners_index], corners_Harris[Harris_index]) < 30)
			{
				corners[corners_index] = corners_Harris[Harris_index];
				break;
			}
		};
	};


	std::vector<cv::Point2f> approx;
	cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);

	if (approx.size() != 4)
	{
		std::cout << "The object is not quadrilateral!" << std::endl;
		cout << approx.size();
		waitKey(0);
		//return -1;
	}

	// Get mass center  
	for (int i = 0; i < corners.size(); i++)
		center += corners[i];
	center *= (1. / corners.size());

	sortCorners(corners, center);
	if (corners.size() == 0) {
		std::cout << "The corners were not sorted correctly!" << std::endl;
		cout << corners.size();
		waitKey(0);
		//return -1;
	}
	//cv::Mat dst = src.clone();

	////// Draw lines  
	//for (int i = 0; i < lines.size(); i++)
	//{
	//	cv::Vec4i v = lines[i];
	//	cv::line(dst, cv::Point(v[0], v[1]), cv::Point(v[2], v[3]), CV_RGB(0, 255, 0));
	//}

	//// Draw corner points  
	//cv::circle(dst, corners[0], 30, CV_RGB(255, 0, 0), 2);
	//cv::circle(dst, corners[1], 30, CV_RGB(0, 255, 0), 2);
	//cv::circle(dst, corners[2], 30, CV_RGB(0, 0, 255), 2);
	//cv::circle(dst, corners[3], 30, CV_RGB(255, 255, 255), 2);

	//// Draw mass center  
	//cv::circle(dst, center, 3, CV_RGB(255, 255, 0), 2);
	
	//cv::Mat quad = cv::Mat::zeros(300, 100, CV_8UC3);
	cv::Mat quad = cv::Mat::zeros(ROI.height, ROI.width, CV_8UC3);

	std::vector<cv::Point2f> quad_pts;
 	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));

	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	cv::warpPerspective(src, quad, transmtx, quad.size());
	//namedWindow("Image", 4);
	//cv::imshow("Image", dst);
	//cv::imshow("quadrilateral", quad);
	////
	//cv::waitKey();
	return quad;
}


//
//void testCanny(int, void*)
//{
//	/*createTrackbar("thresh1", "line", &thresh1, thresh_max, testCanny);
//	createTrackbar("thresh2", "line", &thresh2, thresh_max, testCanny);
//	createTrackbar("length", "line", &linelenth, thresh_max, testCanny);
//	testCanny(0, 0);*///在外面的部分
//	std::vector<cv::Vec4i> lines;
//	Mat dst;
//	src_gray.copyTo(dst);
//	cv::HoughLinesP(src_gray, lines, 1, CV_PI / 180, thresh1, thresh2, linelenth);
//	for (size_t i = 0; i < lines.size(); i++)
//	{
//		Vec4i l = lines[i];
//		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
//	}
//	namedWindow("lines", 4);
//	imshow("lines", dst);
//
//}