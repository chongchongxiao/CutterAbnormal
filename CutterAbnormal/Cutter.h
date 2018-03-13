#pragma once
//刀具操作类
#include<iostream>
#include<vector>
#include<map>
using namespace std;

#include<opencv2/opencv.hpp>
using namespace cv;

#include "StandardVideo.h"
#include "Util.h"   
#include "PythonInterface.h"

//比较算法
#define HIST 0 //直方图
#define CONTOURAREA 1//轮廓面积
#define INCEPTION_V3 2//深度学习

//各种算法阈值threshold
//直方图阈值
#define HISTTHRESHOLD_A4MM 0.90
#define HISTTHRESHOLD_A6MM 0.80
#define HISTTHRESHOLD_A10MM 0.95


struct HistResult  //直方图的返回结果，暂时测试使用
{
	int result=0;//二进制数
	bool abnormal;
	double sim;//相似度
	Mat image;
	Mat stdImage;
	Mat hist;
	Mat stdHist;
};

struct AreaResult  //直方图的返回结果，暂时测试使用
{
	int result=0;//二进制数
	bool abnormal;
	double diffArea;//面积差
	double area,stdArea;
	Mat image;
	Mat stdImage;
	Mat areaImage;
	Mat stdAreaImage;
};




//刀具操作类，刀具比较算法，以及获取精准刀具位置
class Cutter
{
private:
 
	PythonInterface * pi;
	string judgeImagePath = "E:\\VisualProjects\\judgeImage";
	string inceptionImage = "inception.png";
	double histThreshold;//直方图阈值，低于这个阈值，表示异常
	double cannyThreshold1, cannyThreshold2;//canny算子阈值，计算轮廓面积时候设定的值
	double areaThreshold;//轮廓面积阈值，差值大于这个值表示异常
	HistResult histResult;//直方图返回结果，测试使用
	AreaResult areaResult;//面积比较返回结果，测试使用

	double histCompare(const Mat&image1, const Mat&image2);
	Mat getHistImg(const MatND& hist);//绘制已经计算出的直方图
	int getInceptionV3(const Mat& image);//InceptionV3模型,1表示异常，0表示正常

	double getArea(const Mat& srcImage, Mat& areaImage);//获取面积srcImage最大轮廓的面积，并且将轮廓图存入areaImage中
	double contourAreaCompare(const Mat& image1, const Mat& image2);//求出两个图像的面积差
	
	
public:
	Cutter();//默认构造函数创建4mm刀具
	~Cutter();

	
	//bool compareImage(const Mat&image1,const Mat& image2,int method);//比较两张图片,true表示异常
	void* compareImage(const Mat&image1, const Mat& image2, int method);//比较方法获取判断结果
	int judgeImage(const Mat& image, int method);//非比较方法获取判断结果
	void setHistThreshold(double tr);
	void setAreaThreshold(double areaTr, double tr1 = 20, double tr2 = 60);
	
};
