#include "StandardVideo.h"



StandardVideo::StandardVideo()
{
}


StandardVideo::~StandardVideo()
{
}


Mat StandardVideo::getCutterByTemplate(const Mat & image_source)
{
	if (configPara.isCutCutter == 0) //如果用户在添加视频的时候没有添加裁剪参数
		return image_source;
	Mat image_matched;
	//模板匹配  
	matchTemplate(image_source, cutterTemplateImage, image_matched, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	Point templateRectCenter;//模板框中心点
	Point cutterRectCenter;//刀具框的中心点
						   //寻找最佳匹配位置  
	minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);

	Mat image_result = image_source;
	//cvtColor(image_source, image_color, CV_GRAY2BGR);

	//模板框中心
	templateRectCenter.x = maxLoc.x + cutterTemplateImage.cols / 2;
	templateRectCenter.y = maxLoc.y + cutterTemplateImage.rows / 2;

	//刀具框
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
	string confID = pr.cutterType + pr.partType;//配置参数的ID有刀具类型和零件类型的组合来确定
	string buffer;
	map<string, ConfigPara> confs;//存储参数
	ConfigPara para;//读取文件的临时存放位置
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
	if (confs.count(confID) == 0)//源配置文件中不含有需要保存的刀具类型
	{
		confs.insert(pair<string, ConfigPara>(confID, pr));
	}else //源配置文件中含有需要保存的刀具类型，那么更新需要保存的内容
	{
		iter = confs.find(confID);
		//iter->second = pr;
		iter->second.setPara(pr);//更新参数设置
	}
	if (pr.isCutCutter == 1)//只有裁剪之后才更新模板
	{
		imwrite(templateImagePath + "//" + pr.confID + ".png", pr.templateImage);//图片直接写入即可，如果 已存在，会直接覆盖掉
	}
	//循环遍历重新保存到文件中
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
	string confID = cutterType + partType;//配置参数的ID有刀具类型和零件类型的组合来确定
	string buffer;
	bool flag = false;//记录是否读取成功
	//TemplatePara templatePara;
	if (TempConfFileFin.is_open())
	{
		while (!TempConfFileFin.eof())     
		{
			getline(TempConfFileFin, buffer);
			if (configPara.getConfigParaByString(buffer)&& configPara.confID == confID)
			{
				if(configPara.isCutCutter==1)//必须用户制作模板之后才需要读取模板图片
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
	bool flag = false;//记录是否读取成功
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

	//去重操作
	sort(cutters.begin(), cutters.end());
	cutters.erase(unique(cutters.begin(), cutters.end()), cutters.end());

	return cutters;
}

vector<string> StandardVideo::getAllPart(string cutter)
{
	ifstream TempConfFileFin(configFilePath);
	vector<string> parts;
	string buffer;
	bool flag = false;//记录是否读取成功
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


