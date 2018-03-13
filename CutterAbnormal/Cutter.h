#pragma once
//���߲�����
#include<iostream>
#include<vector>
#include<map>
using namespace std;

#include<opencv2/opencv.hpp>
using namespace cv;

#include "StandardVideo.h"
#include "Util.h"   
#include "PythonInterface.h"

//�Ƚ��㷨
#define HIST 0 //ֱ��ͼ
#define CONTOURAREA 1//�������
#define INCEPTION_V3 2//���ѧϰ

//�����㷨��ֵthreshold
//ֱ��ͼ��ֵ
#define HISTTHRESHOLD_A4MM 0.90
#define HISTTHRESHOLD_A6MM 0.80
#define HISTTHRESHOLD_A10MM 0.95


struct HistResult  //ֱ��ͼ�ķ��ؽ������ʱ����ʹ��
{
	int result=0;//��������
	bool abnormal;
	double sim;//���ƶ�
	Mat image;
	Mat stdImage;
	Mat hist;
	Mat stdHist;
};

struct AreaResult  //ֱ��ͼ�ķ��ؽ������ʱ����ʹ��
{
	int result=0;//��������
	bool abnormal;
	double diffArea;//�����
	double area,stdArea;
	Mat image;
	Mat stdImage;
	Mat areaImage;
	Mat stdAreaImage;
};




//���߲����࣬���߱Ƚ��㷨���Լ���ȡ��׼����λ��
class Cutter
{
private:
 
	PythonInterface * pi;
	string judgeImagePath = "E:\\VisualProjects\\judgeImage";
	string inceptionImage = "inception.png";
	double histThreshold;//ֱ��ͼ��ֵ�����������ֵ����ʾ�쳣
	double cannyThreshold1, cannyThreshold2;//canny������ֵ�������������ʱ���趨��ֵ
	double areaThreshold;//���������ֵ����ֵ�������ֵ��ʾ�쳣
	HistResult histResult;//ֱ��ͼ���ؽ��������ʹ��
	AreaResult areaResult;//����ȽϷ��ؽ��������ʹ��

	double histCompare(const Mat&image1, const Mat&image2);
	Mat getHistImg(const MatND& hist);//�����Ѿ��������ֱ��ͼ
	int getInceptionV3(const Mat& image);//InceptionV3ģ��,1��ʾ�쳣��0��ʾ����

	double getArea(const Mat& srcImage, Mat& areaImage);//��ȡ���srcImage�����������������ҽ�����ͼ����areaImage��
	double contourAreaCompare(const Mat& image1, const Mat& image2);//�������ͼ��������
	
	
public:
	Cutter();//Ĭ�Ϲ��캯������4mm����
	~Cutter();

	
	//bool compareImage(const Mat&image1,const Mat& image2,int method);//�Ƚ�����ͼƬ,true��ʾ�쳣
	void* compareImage(const Mat&image1, const Mat& image2, int method);//�ȽϷ�����ȡ�жϽ��
	int judgeImage(const Mat& image, int method);//�ǱȽϷ�����ȡ�жϽ��
	void setHistThreshold(double tr);
	void setAreaThreshold(double areaTr, double tr1 = 20, double tr2 = 60);
	
};
