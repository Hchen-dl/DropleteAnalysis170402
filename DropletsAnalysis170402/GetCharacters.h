#pragma once
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"

using namespace std;
using namespace cv;

class GetCharacters 
{
private:
	double GetSumVolume(int n, vector<double>radius_sorted);
public :
	void GettingCharacters(Mat src, Rect ROI, vector<double>radius_sorted);
};
//
////void GettingCharacters(Mat src,Rect ROI, vector<double>radius_sorted);
//double GetSumVolume(int n, vector<double>radius_sorted);
//
/////计算各个雾滴参数
//
//void GettingCharacters(Mat src, Rect ROI, vector<double>radius_sorted)//
//{
//	double ratio = 76.0/(double)ROI.height;//actual length and the pixal length.
//	int amount = radius_sorted.size();
//	double dentisty = (float)amount / (2*7);
//	double mediem_amount = 0;
//	mediem_amount = radius_sorted[amount / 2];
//
//	double mediem_volume = 0;
//	/*for (int i = 0; 
//		abs(GetSumVolume(i, radius_sorted) - GetSumVolume(amount - 1, radius_sorted)) >
//		abs(GetSumVolume(i + 1, radius_sorted) - GetSumVolume(amount - 1, radius_sorted));
//		i++)
//	{
//		mediem_volume = radius_sorted[i];
//	};*/
//	for (int i = 0; i < amount-1; i++)
//	{
//		if (abs(GetSumVolume(i, radius_sorted) - 0.5*GetSumVolume(amount - 2, radius_sorted)) <
//			abs(GetSumVolume(i + 1, radius_sorted) - 0.5*GetSumVolume(amount - 2, radius_sorted)))
//		{
//			mediem_volume = radius_sorted[i];
//			break;
//		}
//	}
//
//
//
//	double uniformity = 0;
//	uniformity = mediem_amount / mediem_volume;
//
//	double coverage = 0;
//	double sum_area = 0;
//	for (int i = 0; i < amount; i++)
//	{
//		sum_area += 3.14*radius_sorted[i] * radius_sorted[i];
//	}
//	double total_area = ROI.area();
//	coverage = sum_area / total_area;
//
//	double unifority_in_groups = 0;
//
//	cout << "ratio:" << ratio << endl;
//	cout << "amount/cm2:" << dentisty << endl;
//	cout << "radius_mediem_amount:" << mediem_amount << endl;
//	cout << "radius_mediem_volume:" << mediem_volume << endl;
//	cout << "circle_uniformity:" << uniformity << endl;
//	cout << "area_coverage:" << coverage << endl;
//}
/////get the sum of the top n volume.
//double GetSumVolume(int n, vector<double>radius_sorted)
//{
//	double sum = 0;
//	for (int i = 0; i <= n; i++)
//	{
//		sum += (4*3.14 / 3.0)*radius_sorted[i] * radius_sorted[i] * radius_sorted[i];
//	}
//	return sum;
//}
