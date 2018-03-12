#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct ImageTemplate
{
	bool isTemplateMove = false;//�Ƿ�ģ����ƶ����ƶ���ͬʱ�����߿�����ƶ�
	bool isCutterMove = false;//�Ƿ񵶾߿��ƶ������������ƶ�����֤���߿����ĺ�ģ���������һ������
	bool drawingTemplate = false;//��¼�Ƿ�������ģ��
	bool drawedTemplate = false;//�Ƿ���ģ��򣬻���ģ���֮����ʾ���߿򣬲������ƶ�����
	Point templageRectStart;//ģ�忪ʼ��
	Point templageRectEnd;//ģ�������
	Point lastLocation;//�϶���ʱ��ģ���Ҫ��¼��һ��λ��
	int cutterRectLength = 150;//����Ĭ�ϴ�С
	int cutterRectWide = 150;
	int distanceCenter = 200;//���߿��ģ������ĵ�ľ��룬����ƫ��������λpixel
	ImageTemplate() {}
	void init()//��ʼ��һЩ�ؼ�����
	{
		isTemplateMove = false;//�Ƿ�ģ����ƶ����ƶ���ͬʱ�����߿�����ƶ�
		isCutterMove = false;//�Ƿ񵶾߿��ƶ������������ƶ�����֤���߿����ĺ�ģ���������һ������
		drawingTemplate = false;//��¼�Ƿ�������ģ��
		drawedTemplate = false;//�Ƿ���ģ��򣬻���ģ���֮����ʾ���߿򣬲������ƶ�����
	}
	void setCutterRectSize(int length, int wide)//���õ��߿��С
	{
		cutterRectLength = length;
		cutterRectWide = wide;
	}

	void setDistance(int distance)//����ģ���͵��߿����ĵ����
	{
		distanceCenter = distance;
	}
};

extern ImageTemplate IMAGETEMPLATE;//ȫ�ֱ�����ʹ�ã�ͷ�ļ���������cpp�ļ��ж���

void onMouseTmp(int event, int x, int y, int flags, void *ustc);
Mat makeTemplate(Mat& image);//����ģ����Ƭ���Խϴ�ͼ���������Ȼ��������
