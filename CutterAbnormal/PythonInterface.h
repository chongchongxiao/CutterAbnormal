#pragma once
//c++���õ�Python��װ�������
#include<iostream>
#include<fstream> //ifstream ofstream
#include<string>//getline
using namespace std;

#include<Python.h>

#include"Util.h"

class PythonInterface
{
public:
	PythonInterface();
	~PythonInterface();
	int getInceptionV3();
	void test();
private:
	bool isInit = false;//�Ƿ��ʼ��python���л�������֤��������������ֻ��ʼ��һ��
	bool isGetInception = false;//�Ƿ��ȡInceptionģ�ͣ�ֻ�ܻ�ȡһ�Σ���λ�ȡ�ᱨ�쳣
	//string cmdInception = "python E:\\VisualProjects\\judgeImage\\Classify_V3.py";//����inception��python�ű��ļ�
	string inceptionPyName = "Classify_V3.py";//inception����python�ļ�������,��Ҫ��ŵ�ָ��·����
	string resultPath;//��Ž����py�ļ���·��
	string resultInceptionName = "resultInception.txt";//inception�����������ļ�����
};