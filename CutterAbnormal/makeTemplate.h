#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct ImageTemplate
{
	bool isTemplateMove = false;//是否模板框移动，移动的同时，刀具框跟随移动
	bool isCutterMove = false;//是否刀具框移动，仅仅上下移动，保证刀具框中心和模板框中心在一条线上
	bool drawingTemplate = false;//记录是否在制作模板
	bool drawedTemplate = false;//是否画完模板框，画完模板框之后显示刀具框，并进行移动操作
	Point templageRectStart;//模板开始点
	Point templageRectEnd;//模板结束点
	Point lastLocation;//拖动的时候的，需要记录上一个位置
	int cutterRectLength = 150;//刀具默认大小
	int cutterRectWide = 150;
	int distanceCenter = 200;//刀具框和模板框中心点的距离，向下偏移量，单位pixel
	ImageTemplate() {}
	void init()//初始化一些关键变量
	{
		isTemplateMove = false;//是否模板框移动，移动的同时，刀具框跟随移动
		isCutterMove = false;//是否刀具框移动，仅仅上下移动，保证刀具框中心和模板框中心在一条线上
		drawingTemplate = false;//记录是否在制作模板
		drawedTemplate = false;//是否画完模板框，画完模板框之后显示刀具框，并进行移动操作
	}
	void setCutterRectSize(int length, int wide)//设置刀具框大小
	{
		cutterRectLength = length;
		cutterRectWide = wide;
	}

	void setDistance(int distance)//设置模板框和刀具框中心点距离
	{
		distanceCenter = distance;
	}
};

extern ImageTemplate IMAGETEMPLATE;//全局变量的使用，头文件中声明，cpp文件中定义

void onMouseTmp(int event, int x, int y, int flags, void *ustc);
Mat makeTemplate(Mat& image);//制作模板照片，对较大图像进行缩放然后再制作
