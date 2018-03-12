#pragma once
#include <iostream>
#include <vector>
#include <io.h> // _access
#include <direct.h> //_mkdir
#include <fstream> //ifstream ofstream

using namespace std;
void SplitString(const string& s, vector<string>& v, const string& c);//对字符串s进行切割，以c为分隔符，切割后的数组保存在v中
bool createDirectory(string path);//创建文件夹path（必须绝对路径,例如E://VisualProjects//template）
bool copyFileToDir(string srcFilename,string dstFilename);//复制文件到指定目录

string getFileType(string filename);//获取文件后缀, 没有后缀返回""