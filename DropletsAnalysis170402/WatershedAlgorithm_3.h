
/*
*  Vincent and Soille 分水岭浸没算法(1991)的实现
*/

#ifndef WATERSHEDALGORITHM_H
#define WATERSHEDALGORITHM_H

#include <cv.h>
//#include <highgui.h>
#include <string>
#include <queue>
#include<opencv.hpp>
#include<iostream>
#include "highgui.hpp"
#include "imgproc.hpp"
#include "WatershedStructure.h"

using namespace cv;
using namespace std;
//void fillHole(Mat src,Mat& dst);

class WatershedAlgorithm 
{

	static const int HMIN = 0;	// 最小层
	static const int HMAX = 256;	// 最大层
private:
	Mat src;
	Mat src_gray;
	Mat binary;
	void fillHole(Mat srcBw, Mat &dstBw);
	Mat DistanceTransform(Mat binary,Mat dstSrc_Bw);
public:
	WatershedAlgorithm(Mat image);
	Mat mainProcess();

	//Mat run(const Mat src) { // 分水岭核心算法
	//	cvtColor(src, src_gray, CV_RGB2GRAY);
	//	GaussianBlur(src_gray, src_gray, Size(5, 5), 5); //滤波  
	//	Mat binary = Mat::zeros(src.size(), CV_8UC3);
	//	//threshold(src_gray, binary, 90, 255, CV_THRESH_BINARY); //阈值化  
	//	adaptiveThreshold(src_gray, binary, 255, 1, 0,51,7);
	//	//Mat dstBinary= Mat::zeros(src.size(), CV_8UC3);

	//	//namedWindow("Binary", 4);
	//	//imshow("Binary", binary);
	//	Mat src_bw(src_gray.size(), CV_32FC1);
	//	Mat src_BW(src_gray.size(), CV_8UC1);

	//	binary = ~binary;
	//	fillHole(binary, binary);
	//	namedWindow("Binary", 4);
	//	imshow("Binary", binary);
	//	//imshow("dstBinary", binary);
	//	distanceTransform(binary, src_bw,CV_DIST_L12, 0);  //距离变换
	//	for (int i = 0; i<src_bw.rows; i++)
	//	{
	//		for (int j = 0; j<src_bw.cols; j++)
	//		{
	//			src_BW.at<uchar>(i, j) = src_bw.at<float>(i, j);
	//		}
	//	}

	//	normalize(src_BW, src_BW, 0, 255, CV_MINMAX); //为了显示清晰，做了0~255归一化
	//	//grayScaleRe(src_BW,src_BW);

	//	src_BW = ~src_BW;

	//	/* 获取图像信息 */
	//	uchar* pixels = src_BW.data;
	//	int width = src_BW.cols;
	//	int height = src_BW.rows;

	//	/* Vincent and Soille 分水岭算法（1991）第一步: 将像素点存入结构体并排序 */
	//	WatershedStructure  watershedStructure(pixels, width, height);

	//	/* Vincent and Soille 分水岭算法（1991）第二步: 泛洪（模拟浸没） */
	//	/************************ 泛洪（浸没）开始 ****************************/
	//	std::queue<WatershedPixel*> pque;	// 存储像素的临时队列
	//	int curlab = 0;
	//	int heightIndex1 = 0;
	//	int heightIndex2 = 0;
	//	for (int h = HMIN; h < HMAX; ++h) { // h-1 层的 Geodesic SKIZ
	//		for (int pixelIndex = heightIndex1; pixelIndex < watershedStructure.size(); ++pixelIndex) {
	//			WatershedPixel* p = watershedStructure.at(pixelIndex);

	//			/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
	//			if (p->getIntHeight() != h) { heightIndex1 = pixelIndex; break; }

	//			p->setLabelToMASK(); // 标记此像素将被处理

	//			std::vector<WatershedPixel*> neighbours = p->getNeighbours();
	//			for (unsigned i = 0; i < neighbours.size(); ++i) {
	//				WatershedPixel* q = neighbours.at(i);

	//				/* 将处于盆地或分水岭的h层的邻接像素点入队 */
	//				//if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
	//				if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
	//			}
	//		}
	//					
	//		int curdist = 1;
	//		pque.push(new WatershedPixel());

	//		while (true) { // 扩展聚水盆地
	//			WatershedPixel* p = pque.front(); pque.pop();

	//			if (p->isFICTITIOUS())
	//				if (pque.empty()) { delete p; p = NULL; break; }
	//				else {
	//					pque.push(new WatershedPixel());
	//					curdist++;
	//					delete p; p = pque.front(); pque.pop();
	//				}

	//				std::vector<WatershedPixel*> neighbours = p->getNeighbours();
	//				for (unsigned i = 0; i < neighbours.size(); ++i) { // 通过检查邻接像素来标记 p
	//					WatershedPixel* q = neighbours.at(i);

	//					/* q属于一个存在的盆地或分水线 */
	//					if ((q->getDistance() <= curdist) && (q->getLabel() >= 0)) {

	//						if (q->getLabel() > 0) {
	//							if (p->isLabelMASK())
	//								p->setLabel(q->getLabel());
	//							else if (p->getLabel() != q->getLabel())
	//								p->setLabelToWSHED();
	//						}
	//						else if (p->isLabelMASK())
	//							p->setLabelToWSHED();
	//					}
	//					else if (q->isLabelMASK() && (q->getDistance() == 0)) {
	//						q->setDistance(curdist + 1);
	//						pque.push(q);
	//					}
	//				} // 处理邻接像素的for循环
	//		} // 扩展盆地的while循环

	//		  /* 搜寻并处理h层中新的最小值 */
	//		for (int pixelIndex = heightIndex2; pixelIndex < watershedStructure.size(); pixelIndex++) {
	//			WatershedPixel* p = watershedStructure.at(pixelIndex);

	//			/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
	//			if (p->getIntHeight() != h) { heightIndex2 = pixelIndex; break; }

	//			p->setDistance(0); // 重置距离为0

	//			if (p->isLabelMASK()) { // 该像素位于新最小值区域
	//				curlab++;
	//				p->setLabel(curlab);
	//				pque.push(p);

	//				while (!pque.empty()) {
	//					WatershedPixel* q = pque.front();
	//					pque.pop();

	//					std::vector<WatershedPixel*> neighbours = q->getNeighbours();

	//					for (unsigned i = 0; i < neighbours.size(); i++) { // 检查p2的邻域像素
	//						WatershedPixel* r = neighbours.at(i);

	//						if (r->isLabelMASK()) { r->setLabel(curlab); pque.push(r); }
	//					}
	//				} // end while
	//			} // end if
	//		} // end for
	//	}


	//	/************************ 泛洪（浸没）结束 ****************************/

	//	/* 生成分水线图像 */		
	//	Mat img_Watershed = Mat::zeros(src_BW.size(), 0);
	//	
	//	uchar* wsPixels = img_Watershed.data;
	//	uchar* grayPixels = src_gray.data;
	//	for (int pixelIndex = 0; pixelIndex < watershedStructure.size(); pixelIndex++) {
	//		WatershedPixel* p = watershedStructure.at(pixelIndex);

	//		if (p->isLabelWSHED() && !p->allNeighboursAreWSHED()) {//
	//			wsPixels[p->getX() + p->getY()*width] = (char)255; // 在黑色背景中绘制白色分水线
	//			grayPixels[p->getX() + p->getY()*width] = (char)255;	// 在灰度图中绘制白色分水线
	//			//cout <<*binaryPixels << "binary" << endl;
	//		}
	//	}
	//	//inTmp = imgName + "_WS.jpg"; imwrite(inTmp.c_str(), img_Watershed);
	//	//inTmp = imgName + "_Gray_WS.jpg"; imwrite(inTmp.c_str(), src_gray);
	//	//
	//	//imshow("src_gray", src_gray);
	//	//imshow("img_Watershed", img_Watershed);
	//	Mat result = (binary)&(~img_Watershed);
	//	namedWindow("result", 4);
	//	imshow("result", result);
	//	Mat boundary = (binary)&(img_Watershed);
	//	//imshow("boundary", boundary);
	//	return boundary;
	//	
	//};
	//void fillHole(Mat srcBw,Mat &dstBw)
	//{
	//	Size m_Size = srcBw.size();
	//	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像  
	//	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	//	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	//	Mat cutImg;//裁剪延展的图像  
	//	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	//	dstBw = srcBw | (~cutImg);
	//}
};
#endif