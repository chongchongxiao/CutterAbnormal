#pragma once

#include<opencv2/opencv.hpp>
using namespace cv;

#include "Cutter.h"
#include "makeTemplate.h"


//UI����ӿڣ��ṩ��UI���õĸ��ֺ�����ʵ�ֽ���չʾ
class UIOperation
{
public:
	UIOperation();
	~UIOperation();
	//void setCutterType(CutterType cutterType);//���õ�������
	void setTimeDiff(int t);//����ʱ��
	//void setVideoPath(string path);//���ô�������Ƶ·��
	void setVideoPath(string path);//���ô��ж���Ƶ���ļ�·��
	//void setStandardVideoPath(string path);//���û�׼��Ƶ��·��
	int getJudgeResult(int ms,int method);//��ȡ�жϽ����
	//����ע�����������timeDiff+ms<0ʱ���޷�ʹ�ã���������������ж�
	void* getJudgeResultTest(int ms, int method);//��ȡ�жϽ������ʱ���Ի�����ʹ��
	void setHistThreshold(double ht);//����ֱ��ͼ��ֵ
	void setAreaThreshold(double areaTr, double tr1 = 128, double tr2 = 255);
	Mat getVideoImage(int ms);//��ȡ��Ƶms�����ͼƬ,������ʾ��Ƶ��
	Mat getVideoCutterImage(int ms);//��ȡ��Ƶms����ĵ���ͼƬ��ˢ����Ƶ��
	Mat getStandardVideoImage(int ms);//��ȡ��׼��Ƶms�����ͼƬ,������ʾ��Ƶ���������ms�Ǵ�����Ƶ��ʱ�䣨��timeDiffУ��֮��
	//bool isAbnormal();//�Ƿ�����쳣
	TemplatePara makeTmp(string path, int ms=0, int length = 140, int width = 80);//����ģ��ͼƬ�������׼��Ƶ·����ѡȡ�Ĺ̶�ʱ�����Ƭ(��λms)��Ĭ��ѡ���һ֡������Ϊѡ�񵶾߿�Ĵ�С

	bool addStdVideo(ConfigPara conf);//��ӻ�׼��Ƶ��һЩ���ò���
	
	vector<string> getAllCutterType();//��ȡ���еĵ�������
	vector<string> getAllPartType(string cutter);//��ȡ���е��������
	vector<string> getAllMethod();//��ȡ���еĴ�����
	void setConfigePara(string cutterType, string partType);//��ʼ�б�ǰ���õĲ���

	bool openCamera(int dev);//������ͷ,���һ������ͷ��ֱ����0��������ͷ���б仯
	void colseCamera();//�ر�����ͷ
	Mat getFrameFromCamera();//������ͷ��ȡͼƬ
	

private:
	Mat getFrameFromVideo(string videoName, int ms);//��ȡ��Ƶ��ms�����֡������ȡ��

private:
	Mat templateImage;//ģ��ͼƬ
	//int templateFrameTime;
	int timeDiff;//������Ƶ��ʱ����λΪ����, 
	//������ж���Ƶ0sλ�ö�Ӧ��׼��Ƶ1sλ�ã���ôtimeDiffȡֵΪ1000
	string videoPath;//��ȡ�����ļ�����Ƶ·��
	string standardVideoPath;//��׼��Ƶ·��
	Cutter *cutter;//���߲�����
	StandardVideo *stdVideo;//��׼��Ƶ������
	ConfigPara conf;//
	bool abnormal;//�Ƿ��쳣 true��ʾ�����쳣
	VideoCapture *cameraCapture;//����ͷ
};

