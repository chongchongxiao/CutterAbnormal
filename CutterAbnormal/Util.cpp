#include"Util.h"
void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

bool createDirectory(string path) {
	vector<string> paths;
	string dirPath;
	SplitString(path, paths, "//");//�ָ�·������һ����
	for (vector<string>::iterator iter = paths.begin(); iter != paths.end(); iter++)
	{
		dirPath += (*iter);
		if (_access(dirPath.c_str(), 0) == -1)//�ļ��в�����
		{
			if (_mkdir(dirPath.c_str()) == -1)//����ʧ��
			{
				break;
				return false;
			}
		}
		dirPath += "//";
	}
	return true;
}

bool copyFileToDir(string srcFilename, string dstFilename)
{
	

	ifstream in;
	ofstream out;
	in.open(srcFilename, ios::binary);//��Դ�ļ�
	if (in.fail())//��Դ�ļ�ʧ��
	{
		//cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		return false;
	}
	out.open(dstFilename, ios::binary);//����Ŀ���ļ�
	if (out.fail())//�����ļ�ʧ��
	{
		//cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		return false;
	}
	char buf[2048];
	while (in)
	{
		//read��in���ж�ȡ2048�ֽڣ�����buf�����У�ͬʱ�ļ�ָ������ƶ�2048�ֽ�
		//������2048�ֽ������ļ���β������ʵ����ȡ�ֽڶ�ȡ��
		in.read(buf, 2048);
		//gcount()������ȡ��ȡ���ֽ�����write��buf�е�����д��out����
		out.write(buf, in.gcount());
	}

	out.close();
	in.close();
	return true;
}

string getFileType(string filename)
{
	int pos = filename.find(".");
	if (pos == string::npos)
	{
		return string();
	}
	return filename.substr(pos);
}



