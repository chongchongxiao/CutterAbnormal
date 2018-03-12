#pragma once

#include<opencv2/opencv.hpp>
using namespace cv;

#include "Cutter.h"
#include "makeTemplate.h"


//UI界面接口，提供给UI调用的各种函数，实现界面展示
class UIOperation
{
public:
	UIOperation();
	~UIOperation();
	//void setCutterType(CutterType cutterType);//设置刀具类型
	void setTimeDiff(int t);//设置时差
	//void setVideoPath(string path);//设置待测试视频路径
	void setVideoPath(string path);//设置待判断视频的文件路径
	//void setStandardVideoPath(string path);//设置基准视频的路径
	int getJudgeResult(int ms,int method);//获取判断结果，
	//这里注意这个函数在timeDiff+ms<0时，无法使用，所以最好自行先判断
	void* getJudgeResultTest(int ms, int method);//获取判断结果，暂时测试环境下使用
	void setHistThreshold(double ht);//设置直方图阈值
	void setAreaThreshold(double areaTr, double tr1 = 128, double tr2 = 255);
	Mat getVideoImage(int ms);//获取视频ms毫秒的图片,用于显示视频流
	Mat getVideoCutterImage(int ms);//获取视频ms毫秒的刀具图片，刷新视频流
	Mat getStandardVideoImage(int ms);//获取基准视频ms毫秒的图片,用于显示视频流，这里的ms是待测视频的时间（由timeDiff校正之后）
	//bool isAbnormal();//是否出现异常
	TemplatePara makeTmp(string path, int ms=0, int length = 140, int width = 80);//制作模板图片，输入标准视频路径、选取的固定时间的照片(单位ms)，默认选择第一帧，参数为选择刀具框的大小

	bool addStdVideo(ConfigPara conf);//添加基准视频的一些配置参数
	
	vector<string> getAllCutterType();//获取所有的刀具类型
	vector<string> getAllPartType(string cutter);//获取所有的零件类型
	vector<string> getAllMethod();//获取所有的处理方法
	void setConfigePara(string cutterType, string partType);//开始判别前设置的参数

	bool openCamera(int dev);//打开摄像头,如果一个摄像头，直接填0，多摄像头会有变化
	void colseCamera();//关闭摄像头
	Mat getFrameFromCamera();//从摄像头读取图片
	

private:
	Mat getFrameFromVideo(string videoName, int ms);//读取视频第ms毫秒的帧，向下取整

private:
	Mat templateImage;//模板图片
	//int templateFrameTime;
	int timeDiff;//两个视频的时间差，单位为毫秒, 
	//例如待判断视频0s位置对应基准视频1s位置，那么timeDiff取值为1000
	string videoPath;//获取本地文件的视频路径
	string standardVideoPath;//基准视频路径
	Cutter *cutter;//刀具操作类
	StandardVideo *stdVideo;//基准视频操作类
	ConfigPara conf;//
	bool abnormal;//是否异常 true表示出现异常
	VideoCapture *cameraCapture;//摄像头
};

