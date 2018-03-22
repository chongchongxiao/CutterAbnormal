#include "UIOperation.h"
#include <qDebug>


UIOperation::UIOperation()
{
	abnormal = false;//初始状态没有异常
	//templateFrameTime = 0;//制作模板的图片，毫秒，默认标准视频第一帧
	timeDiff=0;//两个视频的时间差，默认没有时间差
	videoPath="E://VisualProjects//video//4mm//Z-5（无冷却）.mp4";//获取本地文件的视频路径，随意设置一个默认值
	cutter=new Cutter();//创建刀具操作类
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
	standardVideoPath = cutter->getStandardVideo();//重新设置刀具类型之后，刀具的基准视频需要更新
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
	int result = 0;//解释最终异常原因，二进制数，1表示出现异常，0表示正常
	Mat src = getFrameFromVideo(videoPath, ms);
	Mat standard = getFrameFromVideo(standardVideoPath, timeDiff + ms);
	//imshow("1_1", src);
	//imshow("1_2", standard);
	src = stdVideo->getCutterByTemplate(src);
	//深度学习方法判断的时候不需要基准视频，此时基准视频为空，调用获取木板的方法会报错，因此需要判断一下
	standard = standard.empty() ? standard : stdVideo->getCutterByTemplate(standard);
	//standard = stdVideo->getCutterByTemplate(standard);
	//imshow("2_1", src);
	//imshow("2_2", standard);
	//waitKey();
	result = timeDiff + ms < 0 ? result | 1 : result;//第一位表示是否时差为负数，并且绝对值大于ms
	result = src.empty() ? result | 2 : result;//第二位表示待测图像是否读取失败
	

	switch (method)
	{
	case HIST:
	{     //学到了，case中定义变量必须把整个case下的语句放在{}中
		result = standard.empty() ? result | 4 : result;//第三位表示基准图像是否读取失败,放到这里是因为有些方法不需要基准视频
		HistResult * histResult = (HistResult*)cutter->compareImage(src, standard, HIST);
		result = histResult->abnormal ? result | 8 : result;//第四位表示判断结果，只有前三位均为0的时候，这个判断为异常才是真正的出现异常
		histResult->result = result;
		return result;
		break;
	}
	case CONTOURAREA:
	{
		result = standard.empty() ? result | 4 : result;//第三位表示基准图像是否读取失败
		AreaResult * areaResult = (AreaResult*)cutter->compareImage(src, standard, CONTOURAREA);
		result = areaResult->abnormal ? result | 8 : result;//第四位表示判断结果，只有前三位均为0的时候，这个判断为异常才是真正的出现异常
		areaResult->result = result;
		return result;
		break;
	}
	case INCEPTION_V3:
	{
		int t = cutter->judgeImage(src, INCEPTION_V3);
		result = t == 1 ? result | 8 : result;//返回结果为1则把result第四位赋值为1
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
	int result = 0;//解释最终异常原因，二进制数，1表示出现异常，0表示正常
	Mat src = getFrameFromVideo(videoPath, ms);
	Mat standard = getFrameFromVideo(standardVideoPath, timeDiff + ms);
	//imshow("1_1", src);
	//imshow("1_2", standard);
	src = stdVideo->getCutterByTemplate(src);
	standard = stdVideo->getCutterByTemplate(standard);
	//imshow("2_1", src);
	//imshow("2_2", standard);
	//waitKey();
	result = timeDiff + ms < 0 ? result | 1 : result;//第一位表示是否时差为负数，并且绝对值大于ms
	result = src.empty() ? result | 2 : result;//第二位表示待测图像是否读取失败
	result = standard.empty() ? result | 4 : result;//第三位表示基准图像是否读取失败
	
	switch (method)
	{
	case HIST:
	{     //学到了，case中定义变量必须把整个case下的语句放在{}中
		HistResult * histResult = (HistResult*)cutter->compareImage(src, standard, HIST);
		result = histResult->abnormal ? result | 8 : result;//第四位表示判断结果，只有前三位均为0的时候，这个判断为异常才是真正的出现异常
		histResult->result = result;
		return histResult;
		break;
	}
	case CONTOURAREA:
	{
		AreaResult * areaResult = (AreaResult*)cutter->compareImage(src, standard, CONTOURAREA);
		result = areaResult->abnormal ? result | 8 : result;//第四位表示判断结果，只有前三位均为0的时候，这个判断为异常才是真正的出现异常
		areaResult->result = result;
		return areaResult;
		break;
	}
	case INCEPTION_V3:
	{
		return NULL;
		//int t = cutter->judgeImage(src,INCEPTION_V3);
		//result = t == 1 ? result | 8 : result;
		//return result;  //这里返回一定会出错，最好不要使用，使用另外一个方法getJudgeResult
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

TemplatePara UIOperation::makeTmp(string path,int ms, int length , int width )//如果参数有默认值，则只需要在声明的时候写出即可，定义不需要再写
{
	Mat image = getFrameFromVideo(path, ms);//从视频中读取待制作的图片
	TemplatePara para;
	IMAGETEMPLATE.cutterRectWide = width;
	IMAGETEMPLATE.cutterRectLength = length;
	para.templateImage = makeTemplate(image);//获取模板图片
	para.distanceCenter = IMAGETEMPLATE.distanceCenter;//两个框的中心点的距离
	para.cutterRectLength = length;
	para.cutterRectWide = width;
	return para;
}



bool UIOperation::openCamera(int dev)
{
	cameraCapture=new VideoCapture(dev);
	//cameraCapture->open(0); //不明白为什么不需要open
	if (cameraCapture->isOpened())
		return true;
	return false;
}

void UIOperation::colseCamera()
{
	cameraCapture->release();
	delete cameraCapture;//调用析构函数，自动关闭摄像头
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
	//TODO 保存参数 刀具类型 零件类型 视频路径 模板相关的一些参数 直方图阈值
	
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
	//templateImage = conf.templateImage;//更新模板图片
	standardVideoPath = conf.stdVideoPath;//更新基准视频路径
	setHistThreshold(conf.histThreshold);//更新直方图阈值
}

Mat UIOperation::getFrameFromVideo(string videoName,int ms)
{
	if (access(videoName.c_str(), 00) == -1)
	{
		return Mat();
	}
	if (ms < 0)return Mat();
	VideoCapture capture(videoName);
	//检测是否正常打开:
	if (!capture.isOpened())
		return Mat();
	//获取整个帧数  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	//帧率
	double fps = capture.get(CV_CAP_PROP_FPS);
	double sec = (double)ms / 1000; //转化为秒
	int startFrame = (int)floor(fps * sec);//对非整数，向下取整
	Mat frame=Mat();
	if (startFrame > totalFrameNumber)//秒数大于视频长度
		return Mat();
	capture.set(CAP_PROP_POS_FRAMES, startFrame);//设置开始帧为将要获取的帧
	capture.read(frame);//读取帧，并存入矩阵frame
	capture.release();//释放视频流
	return frame;
}
