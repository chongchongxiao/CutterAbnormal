#pragma once
//c++���õ�Python��װ�������
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
	bool endPython();//������python����

private:
	bool isInit = false;//�Ƿ��ʼ��python���л�������֤��������������ֻ��ʼ��һ��
	bool isGetInception = false;//�Ƿ��ȡInceptionģ�ͣ�ֻ�ܻ�ȡһ�Σ���λ�ȡ�ᱨ�쳣
	bool isStartInception = false;//��¼inception�߳��Ƿ���
	//string cmdInception = "python E:\\VisualProjects\\judgeImage\\Classify_V3.py";//����inception��python�ű��ļ�
	//string inceptionPyName = "Classify_V3.py";//inception����python�ļ�������,��Ҫ��ŵ�ָ��·����
	string resultPath;//��Ž����py�ļ���·��
	string resultInceptionName = "resultInception.txt";//inception�����������ļ�����
	string pythonName = "python";//python�ű������֣���Ϊ���ܸ��ŵ������������������Ժ������ܻ������

private:
	//void startInceptionThread();//��ʼinceptionԤ����߳�
	bool checkPython();//���python�����Ƿ�������
	bool destroyPython();
};