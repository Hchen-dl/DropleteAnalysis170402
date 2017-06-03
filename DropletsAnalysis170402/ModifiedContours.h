#pragma once
#include<opencv.hpp>

using namespace std;
using namespace cv;

vector<Point>contour;
vector<Point> CloseContour(vector<Point> contour);

vector<Point> CloseContour(vector<Point> contour);
bool IsClose(vector<Point> contour);

Mat ModifiedContours(Mat edges)
{
	Mat newEdges(Mat::zeros(edges.size(), CV_8UC3));
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	findContours(edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	contours[i] = CloseContour(contours[i]);
	//	//approxPolyDP(contours[i], contours[i], 3, true);
	//}
	int n = contours.size();
	for (int i = 0; i < contours.size(); i++)
	{
		if(contourArea(contours[i])!=0)
		drawContours(newEdges, contours, i, Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
	}
	imshow("newedge", newEdges);
	return newEdges;

}
vector<Point> CloseContour(vector<Point> contour)
{

	for (int i = 0; i < contour.size(); i++)cout<<" x:" << contour[i].x <<" y:"<< contour[i].y;
	cout <<" C:"<< arcLength(contour,1)<<" S:"<<contourArea(contour) << endl;
	int size = contour.size();
	if(contourArea(contour) != 0)
		return contour;
	for (int i = 0; i < size; i++)
	{
		if (size > 1)
			for (int j = i + 1; j < size; j++)
			{
				if (contour[i] == contour[j])
				{
					contour.erase(contour.begin() + j);
				}
				break;
			}
				
	}
	//int m = contour.size();
	Point newEnd;
	newEnd.x = contour[0].x + 1;
	newEnd.y = contour[0].y;
	contour.push_back(newEnd);
	//int newn = contour.size();
	return contour;
}

bool IsClose(vector<Point> contour)
{
	if (contourArea(contour) == 0)return false;
	/*Point start = contour[0];
	Point end = contour[contour.size()-1];
	if ((abs(start.x - end.x) <= 1) && (abs(start.y - end.y) <= 1))
		return true;*/
	return true;
}