#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include "Util.h"
using namespace std;
using namespace cv;
/*��׼��Ƶ�Ĳ�����ȡ
**��ȡ������ͼ
**��ȡ���ѧϰ�����ݣ�ͼƬ��
**ѵ�����ѧϰģ��
*/

struct ConfigPara
{
	string confID;//���ò�����Ψһ��ʶ�����õ��ߺ�����������ȷ��
	string cutterType;//��������
	string partType;//�������
	string stdVideoPath;//��׼��Ƶ��·��
	int isCutCutter;//�Ƿ�ü����ߣ�1��ʾ�ü�,���Ϊ0 �������������û������
	int distanceCenter;//ģ���͵��߿����ĵ�ľ���
	int cutterRectLength, cutterRectWide;//���߿�ĳ��Ϳ�
	double histThreshold;//ֱ��ͼ��ֵ
	
	Mat templateImage;//ģ��ͼƬ

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
	bool getConfigParaByString(string buffer)//ͨ����ȡ�����ַ�������������Ϣ
	{
		vector<string> buffers;
		SplitString(buffer, buffers, "\t");
		if (buffers.size() != 9) return false; //������ǰ��������ļ��ı�׼��ʽ��ô�޷�����
		confID = buffers[0];
		cutterType = buffers[1];//��������
		partType = buffers[2];//�ӹ��������
		stdVideoPath = buffers[3];//��׼��Ƶ·��
		isCutCutter = atoi(buffers[4].c_str());//�Ƿ�ü�����
		distanceCenter = atoi(buffers[5].c_str());//���߿��ģ������ĵ���
		cutterRectLength = atoi(buffers[6].c_str());//���߿�ĳ�
		cutterRectWide = atoi(buffers[7].c_str());//���߿�Ŀ�
		histThreshold = atof(buffers[8].c_str());//ֱ��ͼ��ֵ
		buffers.clear();
		return true;
	}

	string getString()//��������Ϣ�������ݱ����ʽƴ�ӳ�string��
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



struct TemplatePara //����ģ��ʱ�ķ���ֵ
{
	int distanceCenter;//ģ���͵��߿����ĵ�ľ���
	int cutterRectLength, cutterRectWide;//���߿�ĳ��Ϳ�
	Mat templateImage;//ģ��ͼƬ
};


class StandardVideo
{
public:
	StandardVideo();
	~StandardVideo();

	Mat getCutterByTemplate(const Mat& image_source);
	bool saveStdVideoToFile(ConfigPara pr);//�����׼��Ƶ�Լ����׼��Ƶ��صĲ����������ļ���
	ConfigPara getStdVideoConfigeByFile(string cutterType,string partType);//ͨ���������ͺ�����������ļ���ȡ��׼��Ƶ�Լ���ز���
	
	vector<string> getAllCutter();//��ȡ���еĵ�������
	vector<string> getAllPart(string cutter);//����ѡ��ĵ��߻�ȡ���еļӹ��������

	void makeTrainData(vector<string> files);//�������ѧϰ���ݣ���Ҫ����Ҫ��ȡ������ͼ�����Էŵ�������У���Ȼ�о��е㲻̫����

private:
	string getTime();//��ȡ��ǰʱ�䣬��ȷ����,�����ļ��е�����
	//bool getConfigParaByString(string buffer,ConfigPara& para);//ͨ����ȡ�����ַ�������������Ϣ

private:
	//string cutterType;
	//string partType;
	//string stdVideoPath;
	//int distanceCenter;//ģ���͵��߿����ĵ�ľ���
	// cutterRectLength, cutterRectWide;//���߿�ĳ��Ϳ�
	Mat cutterTemplateImage;//ģ��ͼƬ����ȡ������
	ConfigPara configPara;//�洢�����ļ�����ز���
	string templatePath; /*"E:\\VisualProjects\\template";*///�����ļ�·��
	string templateFileName="template.conf";//�����ļ�����
	string dataDir;//������ɵ�ѵ����
	//string templateImagePath = "E:\\VisualProjects\\template";//ģ��ͼƬ����Ŀ¼
	
};

