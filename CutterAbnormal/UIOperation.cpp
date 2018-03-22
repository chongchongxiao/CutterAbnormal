#include "UIOperation.h"
#include <qDebug>


UIOperation::UIOperation()
{
	abnormal = false;//��ʼ״̬û���쳣
	//templateFrameTime = 0;//����ģ���ͼƬ�����룬Ĭ�ϱ�׼��Ƶ��һ֡
	timeDiff=0;//������Ƶ��ʱ��Ĭ��û��ʱ���
	videoPath="E://VisualProjects//video//4mm//Z-5������ȴ��.mp4";//��ȡ�����ļ�����Ƶ·������������һ��Ĭ��ֵ
	cutter=new Cutter();//�������߲�����
	stdVideo = new StandardVideo();
	//standardVideoPath = cutter->getStandardVideo();
}


UIOperation::~UIOperation()
{
	delete cutter;
	delete stdVideo;
}

/*void UIOperation::setCutterType(CutterType cutterType)
{
	cutter->setCutterType(cutterType);
	standardVideoPath = cutter->getStandardVideo();//�������õ�������֮�󣬵��ߵĻ�׼��Ƶ��Ҫ����
}*/

void UIOperation::setTimeDiff(int t)
{
	timeDiff = t;
}


void UIOperation::setVideoPath(string path)
{
	videoPath = path;
}


int UIOperation::getJudgeResult(int ms,int method)
{
	int result = 0;//���������쳣ԭ�򣬶���������1��ʾ�����쳣��0��ʾ����
	Mat src = getFrameFromVideo(videoPath, ms);
	Mat standard = getFrameFromVideo(standardVideoPath, timeDiff + ms);
	//imshow("1_1", src);
	//imshow("1_2", standard);
	src = stdVideo->getCutterByTemplate(src);
	//���ѧϰ�����жϵ�ʱ����Ҫ��׼��Ƶ����ʱ��׼��ƵΪ�գ����û�ȡľ��ķ����ᱨ�������Ҫ�ж�һ��
	standard = standard.empty() ? standard : stdVideo->getCutterByTemplate(standard);
	//standard = stdVideo->getCutterByTemplate(standard);
	//imshow("2_1", src);
	//imshow("2_2", standard);
	//waitKey();
	result = timeDiff + ms < 0 ? result | 1 : result;//��һλ��ʾ�Ƿ�ʱ��Ϊ���������Ҿ���ֵ����ms
	result = src.empty() ? result | 2 : result;//�ڶ�λ��ʾ����ͼ���Ƿ��ȡʧ��
	

	switch (method)
	{
	case HIST:
	{     //ѧ���ˣ�case�ж���������������case�µ�������{}��
		result = standard.empty() ? result | 4 : result;//����λ��ʾ��׼ͼ���Ƿ��ȡʧ��,�ŵ���������Ϊ��Щ��������Ҫ��׼��Ƶ
		HistResult * histResult = (HistResult*)cutter->compareImage(src, standard, HIST);
		result = histResult->abnormal ? result | 8 : result;//����λ��ʾ�жϽ����ֻ��ǰ��λ��Ϊ0��ʱ������ж�Ϊ�쳣���������ĳ����쳣
		histResult->result = result;
		return result;
		break;
	}
	case CONTOURAREA:
	{
		result = standard.empty() ? result | 4 : result;//����λ��ʾ��׼ͼ���Ƿ��ȡʧ��
		AreaResult * areaResult = (AreaResult*)cutter->compareImage(src, standard, CONTOURAREA);
		result = areaResult->abnormal ? result | 8 : result;//����λ��ʾ�жϽ����ֻ��ǰ��λ��Ϊ0��ʱ������ж�Ϊ�쳣���������ĳ����쳣
		areaResult->result = result;
		return result;
		break;
	}
	case INCEPTION_V3:
	{
		int t = cutter->judgeImage(src, INCEPTION_V3);
		result = t == 1 ? result | 8 : result;//���ؽ��Ϊ1���result����λ��ֵΪ1
		return result;
		break;
	}
	default:
		return NULL;
		break;
	}
}

bool UIOperation::endJudge()
{
	return cutter->endJudgeImage();
}

void* UIOperation::getJudgeResultTest(int ms,int method)
{
	int result = 0;//���������쳣ԭ�򣬶���������1��ʾ�����쳣��0��ʾ����
	Mat src = getFrameFromVideo(videoPath, ms);
	Mat standard = getFrameFromVideo(standardVideoPath, timeDiff + ms);
	//imshow("1_1", src);
	//imshow("1_2", standard);
	src = stdVideo->getCutterByTemplate(src);
	standard = stdVideo->getCutterByTemplate(standard);
	//imshow("2_1", src);
	//imshow("2_2", standard);
	//waitKey();
	result = timeDiff + ms < 0 ? result | 1 : result;//��һλ��ʾ�Ƿ�ʱ��Ϊ���������Ҿ���ֵ����ms
	result = src.empty() ? result | 2 : result;//�ڶ�λ��ʾ����ͼ���Ƿ��ȡʧ��
	result = standard.empty() ? result | 4 : result;//����λ��ʾ��׼ͼ���Ƿ��ȡʧ��
	
	switch (method)
	{
	case HIST:
	{     //ѧ���ˣ�case�ж���������������case�µ�������{}��
		HistResult * histResult = (HistResult*)cutter->compareImage(src, standard, HIST);
		result = histResult->abnormal ? result | 8 : result;//����λ��ʾ�жϽ����ֻ��ǰ��λ��Ϊ0��ʱ������ж�Ϊ�쳣���������ĳ����쳣
		histResult->result = result;
		return histResult;
		break;
	}
	case CONTOURAREA:
	{
		AreaResult * areaResult = (AreaResult*)cutter->compareImage(src, standard, CONTOURAREA);
		result = areaResult->abnormal ? result | 8 : result;//����λ��ʾ�жϽ����ֻ��ǰ��λ��Ϊ0��ʱ������ж�Ϊ�쳣���������ĳ����쳣
		areaResult->result = result;
		return areaResult;
		break;
	}
	case INCEPTION_V3:
	{
		return NULL;
		//int t = cutter->judgeImage(src,INCEPTION_V3);
		//result = t == 1 ? result | 8 : result;
		//return result;  //���ﷵ��һ���������ò�Ҫʹ�ã�ʹ������һ������getJudgeResult
		break;
	}
	default:
		return NULL;
		break;
	}
}

void UIOperation::setHistThreshold(double ht)
{
	cutter->setHistThreshold(ht);
}

void UIOperation::setAreaThreshold(double areaTr, double tr1, double tr2)
{
	cutter->setAreaThreshold(areaTr, tr1, tr2);
}


Mat UIOperation::getVideoImage(int ms)
{
	return getFrameFromVideo(videoPath, ms);
}

Mat UIOperation::getVideoCutterImage(int ms)
{
	return stdVideo->getCutterByTemplate(getFrameFromVideo(videoPath, ms));
}

Mat UIOperation::getStandardVideoImage(int ms)
{
	return getFrameFromVideo(standardVideoPath, ms);
}

/*bool UIOperation::isAbnormal()
{
	return abnormal;
}*/

TemplatePara UIOperation::makeTmp(string path,int ms, int length , int width )//���������Ĭ��ֵ����ֻ��Ҫ��������ʱ��д�����ɣ����岻��Ҫ��д
{
	Mat image = getFrameFromVideo(path, ms);//����Ƶ�ж�ȡ��������ͼƬ
	TemplatePara para;
	IMAGETEMPLATE.cutterRectWide = width;
	IMAGETEMPLATE.cutterRectLength = length;
	para.templateImage = makeTemplate(image);//��ȡģ��ͼƬ
	para.distanceCenter = IMAGETEMPLATE.distanceCenter;//����������ĵ�ľ���
	para.cutterRectLength = length;
	para.cutterRectWide = width;
	return para;
}



bool UIOperation::openCamera(int dev)
{
	cameraCapture=new VideoCapture(dev);
	//cameraCapture->open(0); //������Ϊʲô����Ҫopen
	if (cameraCapture->isOpened())
		return true;
	return false;
}

void UIOperation::colseCamera()
{
	cameraCapture->release();
	delete cameraCapture;//���������������Զ��ر�����ͷ
}

Mat UIOperation::getFrameFromCamera()
{
	Mat frame = Mat();
	if (cameraCapture->isOpened())
	{
		cameraCapture -> read(frame);
	}
	return frame;
}

void UIOperation::makeTrainData(vector<string> files)
{
	stdVideo->makeTrainData(files);
}



bool UIOperation::addStdVideo(ConfigPara conf)
{
	//TODO ������� �������� ������� ��Ƶ·�� ģ����ص�һЩ���� ֱ��ͼ��ֵ
	
	return stdVideo->saveStdVideoToFile(conf);
}


vector<string> UIOperation::getAllCutterType()
{
	return stdVideo->getAllCutter();
}

vector<string> UIOperation::getAllPartType(string cutter)
{
	return stdVideo->getAllPart(cutter);
}


void UIOperation::setConfigePara(string cutterType, string partType)
{
	conf = stdVideo->getStdVideoConfigeByFile(cutterType, partType);
	//templateImage = conf.templateImage;//����ģ��ͼƬ
	standardVideoPath = conf.stdVideoPath;//���»�׼��Ƶ·��
	setHistThreshold(conf.histThreshold);//����ֱ��ͼ��ֵ
}

Mat UIOperation::getFrameFromVideo(string videoName,int ms)
{
	if (access(videoName.c_str(), 00) == -1)
	{
		return Mat();
	}
	if (ms < 0)return Mat();
	VideoCapture capture(videoName);
	//����Ƿ�������:
	if (!capture.isOpened())
		return Mat();
	//��ȡ����֡��  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	//֡��
	double fps = capture.get(CV_CAP_PROP_FPS);
	double sec = (double)ms / 1000; //ת��Ϊ��
	int startFrame = (int)floor(fps * sec);//�Է�����������ȡ��
	Mat frame=Mat();
	if (startFrame > totalFrameNumber)//����������Ƶ����
		return Mat();
	capture.set(CAP_PROP_POS_FRAMES, startFrame);//���ÿ�ʼ֡Ϊ��Ҫ��ȡ��֡
	capture.read(frame);//��ȡ֡�����������frame
	capture.release();//�ͷ���Ƶ��
	return frame;
}
