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
	SplitString(path, paths, "//");//分割路径，逐一创建
	for (vector<string>::iterator iter = paths.begin(); iter != paths.end(); iter++)
	{
		dirPath += (*iter);
		if (_access(dirPath.c_str(), 0) == -1)//文件夹不存在
		{
			if (_mkdir(dirPath.c_str()) == -1)//创建失败
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
	in.open(srcFilename, ios::binary);//打开源文件
	if (in.fail())//打开源文件失败
	{
		//cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		return false;
	}
	out.open(dstFilename, ios::binary);//创建目标文件
	if (out.fail())//创建文件失败
	{
		//cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		return false;
	}
	char buf[2048];
	while (in)
	{
		//read从in流中读取2048字节，放入buf数组中，同时文件指针向后移动2048字节
		//若不足2048字节遇到文件结尾，则以实际提取字节读取。
		in.read(buf, 2048);
		//gcount()用来提取读取的字节数，write将buf中的内容写入out流。
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



