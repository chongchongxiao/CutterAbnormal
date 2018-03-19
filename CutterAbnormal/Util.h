#pragma once
#include <iostream>
#include <vector>
#include <io.h> // _access
#include <direct.h> //_mkdir
#include <fstream> //ifstream ofstream

//若配置参数不存在时采用的默认值
#ifdef TEST//测试环境下的各种ip设置 
#define PATH_TEMPLATE	"E:\\VisualProjects\\template"//模板存放路径 
#define PATH_JUDGEIMAGE	"E:\\VisualProjects\\judgeImage"//待判断图片路径
#define PATH_DATA	    "E:\\VisualProjects\\video"
#else//工作环境下的各种ip设置
#define PATH_TEMPLATE	".\\template"//模板存放路径 
#define PATH_JUDGEIMAGE	".\\judgeImage"//待判断图片路径
#define PATH_DATA	    ".\\data"
#endif



using namespace std;
void SplitString(const string& s, vector<string>& v, const string& c);//对字符串s进行切割，以c为分隔符，切割后的数组保存在v中
bool createDirectory(string path);//创建文件夹path（必须绝对路径,例如E://VisualProjects//template）
bool copyFileToDir(string srcFilename,string dstFilename);//复制文件到指定目录

string getFileType(string filename);//获取文件后缀, 没有后缀返回""

string getPath(string type);//通过名字获取路径