#pragma once
#include <iostream>
#include <vector>
#include <io.h> // _access
#include <direct.h> //_mkdir
#include <fstream> //ifstream ofstream

using namespace std;
void SplitString(const string& s, vector<string>& v, const string& c);//���ַ���s�����и��cΪ�ָ������и������鱣����v��
bool createDirectory(string path);//�����ļ���path���������·��,����E://VisualProjects//template��
bool copyFileToDir(string srcFilename,string dstFilename);//�����ļ���ָ��Ŀ¼

string getFileType(string filename);//��ȡ�ļ���׺, û�к�׺����""