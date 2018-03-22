#pragma once
//c++调用的Python封装在这个类
#include<iostream>
#include<fstream> //ifstream ofstream
#include<string>//getline
#include<thread>//thread
using namespace std;

#include<Python.h>

#include"Util.h"

class PythonInterface
{
public:
	PythonInterface();
	~PythonInterface();
	int getInceptionV3();
	bool endPython();//结束掉python进程

private:
	bool isInit = false;//是否初始化python运行环境，保证整个程序运行中只初始化一次
	bool isGetInception = false;//是否获取Inception模型，只能获取一次，多次获取会报异常
	bool isStartInception = false;//记录inception线程是否开启
	//string cmdInception = "python E:\\VisualProjects\\judgeImage\\Classify_V3.py";//调用inception的python脚本文件
	//string inceptionPyName = "Classify_V3.py";//inception方法python文件的名称,需要存放到指定路径下
	string resultPath;//存放结果和py文件的路径
	string resultInceptionName = "resultInception.txt";//inception方法结果存放文件名称
	string pythonName = "python";//python脚本的名字，因为可能干扰到本机的其他程序，所以后续可能会调整，

private:
	//void startInceptionThread();//开始inception预测的线程
	bool checkPython();//检测python程序是否在运行
	bool destroyPython();
};