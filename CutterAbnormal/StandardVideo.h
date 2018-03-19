#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include "Util.h"
using namespace std;
using namespace cv;
/*基准视频的参数读取
**获取刀具子图
**获取深度学习的数据（图片）
**训练深度学习模型
*/

struct ConfigPara
{
	string confID;//配置参数的唯一标识符，用刀具和零件的组合来确定
	string cutterType;//刀具类型
	string partType;//零件类型
	string stdVideoPath;//基准视频的路径
	int isCutCutter;//是否裁剪刀具，1表示裁剪,如果为0 则后面三个参数没有意义
	int distanceCenter;//模板框和刀具框中心点的距离
	int cutterRectLength, cutterRectWide;//刀具框的长和宽
	double histThreshold;//直方图阈值
	
	Mat templateImage;//模板图片

	void setPara(ConfigPara para) 
	{
		confID = para.confID;
		cutterType = para.cutterType;
		partType = para.partType;
		stdVideoPath = para.stdVideoPath;
		isCutCutter = para.isCutCutter;
		distanceCenter = para.distanceCenter;
		cutterRectLength = para.cutterRectLength;
		cutterRectWide = para.cutterRectWide;
		histThreshold = para.histThreshold;
	}
	bool getConfigParaByString(string buffer)//通过读取到的字符串解析参数信息
	{
		vector<string> buffers;
		SplitString(buffer, buffers, "\t");
		if (buffers.size() != 9) return false; //如果不是按照配置文件的标准格式那么无法解析
		confID = buffers[0];
		cutterType = buffers[1];//刀具类型
		partType = buffers[2];//加工零件类型
		stdVideoPath = buffers[3];//基准视频路径
		isCutCutter = atoi(buffers[4].c_str());//是否裁剪刀具
		distanceCenter = atoi(buffers[5].c_str());//刀具框和模板框中心点间距
		cutterRectLength = atoi(buffers[6].c_str());//刀具框的长
		cutterRectWide = atoi(buffers[7].c_str());//刀具框的宽
		histThreshold = atof(buffers[8].c_str());//直方图阈值
		buffers.clear();
		return true;
	}

	string getString()//将参数信息按照数据保存格式拼接成string，
	{
		string text;
		text += confID + "\t" + cutterType + "\t" + partType + "\t" + stdVideoPath + "\t";
		text += to_string(isCutCutter) + "\t";
		text += to_string(distanceCenter) + "\t";
		text += to_string(cutterRectLength) + "\t";
		text += to_string(cutterRectWide) + "\t";
		text += to_string(histThreshold);
		return text;
	}
};



struct TemplatePara //制作模板时的返回值
{
	int distanceCenter;//模板框和刀具框中心点的距离
	int cutterRectLength, cutterRectWide;//刀具框的长和宽
	Mat templateImage;//模板图片
};


class StandardVideo
{
public:
	StandardVideo();
	~StandardVideo();

	Mat getCutterByTemplate(const Mat& image_source);
	bool saveStdVideoToFile(ConfigPara pr);//保存基准视频以及与基准视频相关的参数到配置文件中
	ConfigPara getStdVideoConfigeByFile(string cutterType,string partType);//通过刀具类型和零件类型在文件读取基准视频以及相关参数
	
	vector<string> getAllCutter();//获取所有的刀具类型
	vector<string> getAllPart(string cutter);//根据选择的刀具获取所有的加工零件类型

	void makeTrainData(vector<string> files);//制作深度学习数据，主要是需要提取刀具子图，所以放到这个类中，虽然感觉有点不太合适

private:
	string getTime();//获取当前时间，精确到秒,创建文件夹的名字
	//bool getConfigParaByString(string buffer,ConfigPara& para);//通过读取到的字符串解析参数信息

private:
	//string cutterType;
	//string partType;
	//string stdVideoPath;
	//int distanceCenter;//模板框和刀具框中心点的距离
	// cutterRectLength, cutterRectWide;//刀具框的长和宽
	Mat cutterTemplateImage;//模板图片，提取刀具用
	ConfigPara configPara;//存储配置文件的相关参数
	string templatePath; /*"E:\\VisualProjects\\template";*///配置文件路径
	string templateFileName="template.conf";//配置文件名称
	string dataDir;//存放生成的训练集
	//string templateImagePath = "E:\\VisualProjects\\template";//模板图片保存目录
	
};

