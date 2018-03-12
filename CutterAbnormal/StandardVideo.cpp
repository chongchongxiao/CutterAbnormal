#include "StandardVideo.h"



StandardVideo::StandardVideo()
{
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

	if (access(templateImagePath.c_str(), 0) == -1) {
		if (!createDirectory(templateImagePath)) {
			return false;
		}
	}
	string filename = (pr.cutterType + pr.partType) + "." + getFileType(pr.stdVideoPath);
	if (!copyFileToDir(pr.stdVideoPath, templateImagePath + "//" + filename))
		return false;

	ifstream tempConfFileFin(configFilePath);
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


	ofstream tempConfFileFout(configFilePath);
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
		imwrite(templateImagePath + "//" + pr.confID + ".png", pr.templateImage);//ͼƬֱ��д�뼴�ɣ���� �Ѵ��ڣ���ֱ�Ӹ��ǵ�
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
	ifstream TempConfFileFin(configFilePath);
	string confID = cutterType + partType;//���ò�����ID�е������ͺ�������͵������ȷ��
	string buffer;
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
					cutterTemplateImage = imread(templateImagePath +"//"+ configPara.confID + ".png");
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
	ifstream TempConfFileFin(configFilePath);
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
	ifstream TempConfFileFin(configFilePath);
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


