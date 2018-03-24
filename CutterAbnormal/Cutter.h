#pragma once
//刀具操作类，主要封装了刀具异常判别的算法
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
#define HIST 0 //直方图21
#define INCEPTION_V3 1//深度学习



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





//刀具操作类，刀具比较算法，以及获取精准刀具位置
class Cutter
{
private:
 
	PythonInterface * pi;
	string judgeImagePath;//路径
	string inceptionImage = "inception.png";//名称
	double histThreshold;//直方图阈值，低于这个阈值，表示异常
	HistResult histResult;//直方图返回结果，测试使用


	double histCompare(const Mat&image1, const Mat&image2);
	Mat getHistImg(const MatND& hist);//绘制已经计算出的直方图
	int getInceptionV3(const Mat& image);//InceptionV3模型,1表示异常，0表示正常

	
	
public:
	Cutter();//默认构造函数创建4mm刀具
	~Cutter();

	
	int compareImage(const Mat&image1, const Mat& image2, int method);//比较方法获取判断结果
	void* compareImageTest(const Mat&image1, const Mat& image2, int method);//比较方法获取判断结果，测试环境使用，主要是会返回更多的内容，但会影响效率
	//非比较方法获取判断结果
	int judgeImage(const Mat& image, int method);
	/*暂时结束掉判别程序*/
	bool endJudgeImage(int method);
	
	
	void setHistThreshold(double tr);
	
};
