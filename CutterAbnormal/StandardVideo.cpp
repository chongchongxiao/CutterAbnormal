#include "StandardVideo.h"


StandardVideo::StandardVideo()
{
	templatePath = getPath("template");
	dataDir = getPath("dataDir");
}


StandardVideo::~StandardVideo()
{
}


Mat StandardVideo::getCutterByTemplate(const Mat & image_source)
{
	if (configPara.isCutCutter == 0) //����û��������Ƶ��ʱ��û����Ӳü�����
		return image_source;
	Mat image_matched;
	//ģ��ƥ��  
	matchTemplate(image_source, cutterTemplateImage, image_matched, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	Point templateRectCenter;//ģ������ĵ�
	Point cutterRectCenter;//���߿�����ĵ�
						   //Ѱ�����ƥ��λ��  
	minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);

	Mat image_result = image_source;
	//cvtColor(image_source, image_color, CV_GRAY2BGR);

	//ģ�������
	templateRectCenter.x = maxLoc.x + cutterTemplateImage.cols / 2;
	templateRectCenter.y = maxLoc.y + cutterTemplateImage.rows / 2;

	//���߿�
	cutterRectCenter.x = templateRectCenter.x;
	cutterRectCenter.y = templateRectCenter.y + configPara.distanceCenter;

	image_result = image_result(Range(cutterRectCenter.y - configPara.cutterRectLength / 2, cutterRectCenter.y + configPara.cutterRectLength / 2),
		Range(cutterRectCenter.x - configPara.cutterRectWide / 2, cutterRectCenter.x + configPara.cutterRectWide / 2));
	return image_result;
}



bool StandardVideo::saveStdVideoToFile(ConfigPara pr)
{

	if (!createDirectory(templatePath)) 
	{
		return false;
	}
	string filename = (pr.cutterType + pr.partType) + "." + getFileType(pr.stdVideoPath);
	if (!copyFileToDir(pr.stdVideoPath, templatePath + "\\" + filename))
		return false;

	ifstream tempConfFileFin(templatePath+"\\"+templateFileName);
	string confID = pr.cutterType + pr.partType;//���ò�����ID�е������ͺ�������͵������ȷ��
	string buffer;
	map<string, ConfigPara> confs;//�洢����
	ConfigPara para;//��ȡ�ļ�����ʱ���λ��
	if (tempConfFileFin.is_open())
	{
		while (!tempConfFileFin.eof())
		{
			getline(tempConfFileFin, buffer);
			if (para.getConfigParaByString(buffer))
			{
				confs.insert(pair<string, ConfigPara>(para.confID, para));
			}
			else
				continue;
		}
		tempConfFileFin.close();
	}

	
	string templateImagePath = templatePath + "\\" + pr.confID + ".png";//ģ��·��ͨ���������ƺ��������ȷ��
	ofstream tempConfFileFout(templatePath + "\\" + templateFileName);
	map<string, ConfigPara>::iterator iter;
	if (!tempConfFileFout.is_open()) return false;
	if (confs.count(confID) == 0)//Դ�����ļ��в�������Ҫ����ĵ�������
	{
		confs.insert(pair<string, ConfigPara>(confID, pr));
	}else //Դ�����ļ��к�����Ҫ����ĵ������ͣ���ô������Ҫ���������
	{
		iter = confs.find(confID);
		//iter->second = pr;
		iter->second.setPara(pr);//���²�������
	}
	if (pr.isCutCutter == 1)//ֻ�вü�֮��Ÿ���ģ��
	{
		imwrite(templateImagePath, pr.templateImage);//ͼƬֱ��д�뼴�ɣ���� �Ѵ��ڣ���ֱ�Ӹ��ǵ�
	}
	//ѭ���������±��浽�ļ���
	for (iter = confs.begin(); iter != confs.end(); ++iter)
	{
		tempConfFileFout << iter->second.getString() << endl;
	}
	tempConfFileFout.close();
	return true;
	
}

ConfigPara StandardVideo::getStdVideoConfigeByFile(string cutterType, string partType)
{
	ifstream TempConfFileFin(templatePath + "\\" + templateFileName);
	string confID = cutterType + partType;//���ò�����ID�е������ͺ�������͵������ȷ��
	string buffer;
	string templateImagePath = templatePath + "\\" + confID + ".png";
	bool flag = false;//��¼�Ƿ��ȡ�ɹ�
	//TemplatePara templatePara;
	if (TempConfFileFin.is_open())
	{
		while (!TempConfFileFin.eof())     
		{
			getline(TempConfFileFin, buffer);
			if (configPara.getConfigParaByString(buffer)&& configPara.confID == confID)
			{
				if(configPara.isCutCutter==1)//�����û�����ģ��֮�����Ҫ��ȡģ��ͼƬ
					cutterTemplateImage = imread(templateImagePath);
				flag = true;
				break;
			}
				
			else 
				continue;
		}
		TempConfFileFin.close();
	}
	if (flag) return configPara;
	else return ConfigPara();
}

vector<string> StandardVideo::getAllCutter()
{
	ifstream TempConfFileFin(templatePath + "\\" + templateFileName);
	vector<string> cutters;
	string buffer;
	bool flag = false;//��¼�Ƿ��ȡ�ɹ�
					  //TemplatePara templatePara;
	if (TempConfFileFin.is_open())
	{
		while (!TempConfFileFin.eof())
		{
			getline(TempConfFileFin, buffer);
			if (configPara.getConfigParaByString(buffer))
			{
				cutters.push_back(configPara.cutterType);
			}
			else
				continue;
		}
		TempConfFileFin.close();
	}

	//ȥ�ز���
	sort(cutters.begin(), cutters.end());
	cutters.erase(unique(cutters.begin(), cutters.end()), cutters.end());

	return cutters;
}

vector<string> StandardVideo::getAllPart(string cutter)
{
	ifstream TempConfFileFin(templatePath + "\\" + templateFileName);
	vector<string> parts;
	string buffer;
	bool flag = false;//��¼�Ƿ��ȡ�ɹ�
					  //TemplatePara templatePara;
	if (TempConfFileFin.is_open())
	{
		while (!TempConfFileFin.eof())
		{
			getline(TempConfFileFin, buffer);
			if (configPara.getConfigParaByString(buffer)&& configPara.cutterType==cutter)
			{
				parts.push_back(configPara.partType);
			}
			else
				continue;
		}
		TempConfFileFin.close();
	}
	sort(parts.begin(), parts.end());
	parts.erase(unique(parts.begin(), parts.end()), parts.end());
	return parts;
}

void StandardVideo::makeTrainData(vector<string> files)
{
	string time = getTime();
	string dirName = dataDir + "\\" + time;
	if (!createDirectory(dirName)) return;//���������ļ���ʧ�ܣ���ֱ�ӷ���
	VideoCapture capture;
	long currentFrame = 1;
	string frameImage;//ÿһ֡�Ĵ洢·��
	for (vector<string>::iterator iter = files.begin(); iter != files.end(); iter++)
	{
		capture.open(*iter);//����Ƶ�ļ�
		if (!capture.isOpened())//�ļ���ȡʧ�ܣ�ֱ������
			continue;
		//��ȡ����֡��  
		//long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
		capture.set(CV_CAP_PROP_POS_FRAMES, 0);//���ÿ�ʼ֡ 
		Mat frame;//��ȡ����ͼ��
		
		while (capture.read(frame))
		{
			frame = getCutterByTemplate(frame);
			//ÿһ֡д���ļ�
			frameImage = dirName + "\\" + time + "_" + to_string(currentFrame) + ".png";
			imwrite(frameImage, frame);
			currentFrame++;
		}
		//�ر���Ƶ�ļ�  
		capture.release();
	}
}


string StandardVideo::getTime()
{
	time_t rawtime = time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime(&rawtime);
	char timeNow[256];
	memset(timeNow, '\0', 256);
	//���ݵ�ǰʱ��õ��ļ���(һ����һ��log�ļ�)
	sprintf(timeNow, "%04d%02d%02d%02d%02d%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	return string(timeNow);
}


