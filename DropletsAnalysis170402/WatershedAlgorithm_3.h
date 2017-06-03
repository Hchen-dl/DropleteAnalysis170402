
/*
*  Vincent and Soille ��ˮ���û�㷨(1991)��ʵ��
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

	static const int HMIN = 0;	// ��С��
	static const int HMAX = 256;	// ����
private:
	Mat src;
	Mat src_gray;
	Mat binary;
	void fillHole(Mat srcBw, Mat &dstBw);
	Mat DistanceTransform(Mat binary,Mat dstSrc_Bw);
public:
	WatershedAlgorithm(Mat image);
	Mat mainProcess();

	//Mat run(const Mat src) { // ��ˮ������㷨
	//	cvtColor(src, src_gray, CV_RGB2GRAY);
	//	GaussianBlur(src_gray, src_gray, Size(5, 5), 5); //�˲�  
	//	Mat binary = Mat::zeros(src.size(), CV_8UC3);
	//	//threshold(src_gray, binary, 90, 255, CV_THRESH_BINARY); //��ֵ��  
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
	//	distanceTransform(binary, src_bw,CV_DIST_L12, 0);  //����任
	//	for (int i = 0; i<src_bw.rows; i++)
	//	{
	//		for (int j = 0; j<src_bw.cols; j++)
	//		{
	//			src_BW.at<uchar>(i, j) = src_bw.at<float>(i, j);
	//		}
	//	}

	//	normalize(src_BW, src_BW, 0, 255, CV_MINMAX); //Ϊ����ʾ����������0~255��һ��
	//	//grayScaleRe(src_BW,src_BW);

	//	src_BW = ~src_BW;

	//	/* ��ȡͼ����Ϣ */
	//	uchar* pixels = src_BW.data;
	//	int width = src_BW.cols;
	//	int height = src_BW.rows;

	//	/* Vincent and Soille ��ˮ���㷨��1991����һ��: �����ص����ṹ�岢���� */
	//	WatershedStructure  watershedStructure(pixels, width, height);

	//	/* Vincent and Soille ��ˮ���㷨��1991���ڶ���: ���飨ģ���û�� */
	//	/************************ ���飨��û����ʼ ****************************/
	//	std::queue<WatershedPixel*> pque;	// �洢���ص���ʱ����
	//	int curlab = 0;
	//	int heightIndex1 = 0;
	//	int heightIndex2 = 0;
	//	for (int h = HMIN; h < HMAX; ++h) { // h-1 ��� Geodesic SKIZ
	//		for (int pixelIndex = heightIndex1; pixelIndex < watershedStructure.size(); ++pixelIndex) {
	//			WatershedPixel* p = watershedStructure.at(pixelIndex);

	//			/* �����ص�λ�� h+1 �㣬�ݲ���������ѭ�� */
	//			if (p->getIntHeight() != h) { heightIndex1 = pixelIndex; break; }

	//			p->setLabelToMASK(); // ��Ǵ����ؽ�������

	//			std::vector<WatershedPixel*> neighbours = p->getNeighbours();
	//			for (unsigned i = 0; i < neighbours.size(); ++i) {
	//				WatershedPixel* q = neighbours.at(i);

	//				/* ��������ػ��ˮ���h����ڽ����ص���� */
	//				//if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
	//				if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
	//			}
	//		}
	//					
	//		int curdist = 1;
	//		pque.push(new WatershedPixel());

	//		while (true) { // ��չ��ˮ���
	//			WatershedPixel* p = pque.front(); pque.pop();

	//			if (p->isFICTITIOUS())
	//				if (pque.empty()) { delete p; p = NULL; break; }
	//				else {
	//					pque.push(new WatershedPixel());
	//					curdist++;
	//					delete p; p = pque.front(); pque.pop();
	//				}

	//				std::vector<WatershedPixel*> neighbours = p->getNeighbours();
	//				for (unsigned i = 0; i < neighbours.size(); ++i) { // ͨ������ڽ���������� p
	//					WatershedPixel* q = neighbours.at(i);

	//					/* q����һ�����ڵ���ػ��ˮ�� */
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
	//				} // �����ڽ����ص�forѭ��
	//		} // ��չ��ص�whileѭ��

	//		  /* ��Ѱ������h�����µ���Сֵ */
	//		for (int pixelIndex = heightIndex2; pixelIndex < watershedStructure.size(); pixelIndex++) {
	//			WatershedPixel* p = watershedStructure.at(pixelIndex);

	//			/* �����ص�λ�� h+1 �㣬�ݲ���������ѭ�� */
	//			if (p->getIntHeight() != h) { heightIndex2 = pixelIndex; break; }

	//			p->setDistance(0); // ���þ���Ϊ0

	//			if (p->isLabelMASK()) { // ������λ������Сֵ����
	//				curlab++;
	//				p->setLabel(curlab);
	//				pque.push(p);

	//				while (!pque.empty()) {
	//					WatershedPixel* q = pque.front();
	//					pque.pop();

	//					std::vector<WatershedPixel*> neighbours = q->getNeighbours();

	//					for (unsigned i = 0; i < neighbours.size(); i++) { // ���p2����������
	//						WatershedPixel* r = neighbours.at(i);

	//						if (r->isLabelMASK()) { r->setLabel(curlab); pque.push(r); }
	//					}
	//				} // end while
	//			} // end if
	//		} // end for
	//	}


	//	/************************ ���飨��û������ ****************************/

	//	/* ���ɷ�ˮ��ͼ�� */		
	//	Mat img_Watershed = Mat::zeros(src_BW.size(), 0);
	//	
	//	uchar* wsPixels = img_Watershed.data;
	//	uchar* grayPixels = src_gray.data;
	//	for (int pixelIndex = 0; pixelIndex < watershedStructure.size(); pixelIndex++) {
	//		WatershedPixel* p = watershedStructure.at(pixelIndex);

	//		if (p->isLabelWSHED() && !p->allNeighboursAreWSHED()) {//
	//			wsPixels[p->getX() + p->getY()*width] = (char)255; // �ں�ɫ�����л��ư�ɫ��ˮ��
	//			grayPixels[p->getX() + p->getY()*width] = (char)255;	// �ڻҶ�ͼ�л��ư�ɫ��ˮ��
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
	//	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//��չͼ��  
	//	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	//	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	//	Mat cutImg;//�ü���չ��ͼ��  
	//	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	//	dstBw = srcBw | (~cutImg);
	//}
};
#endif