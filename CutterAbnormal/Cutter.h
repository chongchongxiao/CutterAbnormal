#pragma once
//���߲����࣬��Ҫ��װ�˵����쳣�б���㷨
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
#define HIST 0 //ֱ��ͼ21
#define INCEPTION_V3 1//���ѧϰ



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





//���߲����࣬���߱Ƚ��㷨���Լ���ȡ��׼����λ��
class Cutter
{
private:
 
	PythonInterface * pi;
	string judgeImagePath;//·��
	string inceptionImage = "inception.png";//����
	double histThreshold;//ֱ��ͼ��ֵ�����������ֵ����ʾ�쳣
	HistResult histResult;//ֱ��ͼ���ؽ��������ʹ��


	double histCompare(const Mat&image1, const Mat&image2);
	Mat getHistImg(const MatND& hist);//�����Ѿ��������ֱ��ͼ
	int getInceptionV3(const Mat& image);//InceptionV3ģ��,1��ʾ�쳣��0��ʾ����

	
	
public:
	Cutter();//Ĭ�Ϲ��캯������4mm����
	~Cutter();

	
	int compareImage(const Mat&image1, const Mat& image2, int method);//�ȽϷ�����ȡ�жϽ��
	void* compareImageTest(const Mat&image1, const Mat& image2, int method);//�ȽϷ�����ȡ�жϽ�������Ի���ʹ�ã���Ҫ�ǻ᷵�ظ�������ݣ�����Ӱ��Ч��
	//�ǱȽϷ�����ȡ�жϽ��
	int judgeImage(const Mat& image, int method);
	/*��ʱ�������б����*/
	bool endJudgeImage(int method);
	
	
	void setHistThreshold(double tr);
	
};
