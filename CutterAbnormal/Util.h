#pragma once
#include <iostream>
#include <vector>
#include <io.h> // _access
#include <direct.h> //_mkdir
#include <fstream> //ifstream ofstream

//�����ò���������ʱ���õ�Ĭ��ֵ
#ifdef TEST//���Ի����µĸ���ip���� 
#define PATH_TEMPLATE	"E:\\VisualProjects\\template"//ģ����·�� 
#define PATH_JUDGEIMAGE	"E:\\VisualProjects\\judgeImage"//���ж�ͼƬ·��
#define PATH_DATA	    "E:\\VisualProjects\\video"
#else//���������µĸ���ip����
#define PATH_TEMPLATE	".\\template"//ģ����·�� 
#define PATH_JUDGEIMAGE	".\\judgeImage"//���ж�ͼƬ·��
#define PATH_DATA	    ".\\data"
#endif



using namespace std;
void SplitString(const string& s, vector<string>& v, const string& c);//���ַ���s�����и��cΪ�ָ������и������鱣����v��
bool createDirectory(string path);//�����ļ���path���������·��,����E://VisualProjects//template��
bool copyFileToDir(string srcFilename,string dstFilename);//�����ļ���ָ��Ŀ¼

string getFileType(string filename);//��ȡ�ļ���׺, û�к�׺����""

string getPath(string type);//ͨ�����ֻ�ȡ·��