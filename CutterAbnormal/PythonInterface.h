#pragma once
//c++调用的Python封装在这个类
#include<iostream>
#include<fstream> //ifstream ofstream
#include<string>//getline
using namespace std;

#include<Python.h>

class PythonInterface
{
public:
	PythonInterface();
	~PythonInterface();
	int getInceptionV3();
	void test();
private:
	bool isInit = false;//是否初始化python运行环境，保证整个程序运行中只初始化一次
	bool isGetInception = false;//是否获取Inception模型，只能获取一次，多次获取会报异常
	string cmdInception = "python E://VisualProjects//judgeImage//Classify_V3.py";//调用inception的python脚本文件
	string resultInception = "E://VisualProjects//judgeImage//resultInception.txt";//inception方法结果存放文件
};