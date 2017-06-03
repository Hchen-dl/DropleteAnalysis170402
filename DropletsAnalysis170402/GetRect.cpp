#include "GetRect.h"
Mat GetRect::getRect(Mat& src, Rect ROI)
{
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
	for (int i = 0; i < lines.size(); i++)
	{
		cv::Vec4i v = lines[i];
		if (abs(v[2] - v[0]) >abs(v[3] - v[1]))
		{
			lines[i][0] = 0;
			lines[i][1] = ((float)v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1];
			lines[i][2] = src.cols;
			lines[i][3] = ((float)v[1] - v[3]) / (v[0] - v[2]) * (src.cols - v[2]) + v[3];
		}
		else
		{
			lines[i][0] = ((float)v[0] - v[2]) / (v[1] - v[3]) * -v[1] + v[0];
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
			if (IsOneSide(lines[i], lines[j]) && IsClose(lines[i], lines[j]))
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
	CornerDetect cornerdetector;
	corners_Harris = cornerdetector.cornerDetect(src);

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
	
	cv::Mat quad = cv::Mat::zeros(ROI.height, ROI.width, CV_8UC3);

	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));

	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	cv::warpPerspective(src, quad, transmtx, quad.size());

	return quad;
}


bool GetRect::IsClose(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float length = sqrt((x1 - x2)*(x1 - x2) + (y2 - y1)*(y2 - y1));
	float distance = 0.5*sqrt((x4 + x3 - x2 - x1)*(x4 + x3 - x2 - x1) + (y4 + y3 - y2 - y1)*(y4 + y3 - y2 - y1));
	if (distance<0.2*length)return true;
	return false;
}
bool GetRect::IsOneSide(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float up = (x4 - x3)*(x2 - x1) + (y4 - y3)*(y2 - y1);
	float under = sqrt(((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))*sqrt(((x4 - x3)*(x4 - x3) + (y4 - y3)*(y4 - y3)));
	float cos_q = up / under;//计算cos(sita)；
	if (cos_q > 0.5)return true;
	return false;
}
Point2f GetRect::computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
	float denom;
	if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0)return Point2f(-1, -1);

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
			pt.x = (x1 * y2 - y1 * x2) / d * (x3 - x4) - (x1 - x2) / d * (x3 * y4 - y3 * x4);
			pt.y = -(x2 * y1 - y2 * x1) * ((y3 - y4) / d) + ((y1 - y2) / d) * (x4 * y3 - y4 * x3);
		}
		return pt;
	}
	else
		return cv::Point2f(-1, -1);
}
double GetRect:: Distance(Point2f Point1, Point2f Point2)
{
	double distance = sqrt((Point1.x - Point2.x)*(Point1.x - Point2.x) + (Point1.y - Point2.y)*(Point1.y - Point2.y));
	return distance;
}
void GetRect:: sortCorners(std::vector<cv::Point2f>& corners,
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