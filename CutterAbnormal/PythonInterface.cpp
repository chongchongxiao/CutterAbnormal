#include "PythonInterface.h"
#include<windows.h>//CreateProcess
//�����Windows.hͷ�ļ����������Ҫ�����ͷ�ļ��е�������OpenCV�໥��ͻ����������Ͳ��ᱨ��
//error C2872: ��ACCESS_MASK��: ����ȷ�ķ���]�Ľ������
#include<tlhelp32.h>//PROCESSENTRY32  CreateToolhelp32Snapshot  TH32CS_SNAPPROCESS  Process32First Process32Next

PythonInterface::PythonInterface()
{
	resultPath = getPath("judgeImage");
}


PythonInterface::~PythonInterface()
{
}

LPCWSTR stringToLPCWSTR(std::string orig)//string תLPCWSTR
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}


LPWSTR ConvertCharToLPWSTR(const char * szString)//char* ת LPWSTR
{
	int dwLen = strlen(szString) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//������ʵĳ���
	LPWSTR lpszPath = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
	return lpszPath;
}


std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)//WCHAR ת string
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char    *m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}



BOOL system_hide(const char* CommandLine)//ִ��cmd�����������������Ϊ����ڲ���������Ϊwindow.h��OpenCV��ͻ
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return   FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//�ؼ����裬CreateProcess�����������������MSDN 
	string aa="ff";
	if (!CreateProcess(NULL, ConvertCharToLPWSTR(CommandLine), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		return   FALSE;
	}
	CloseHandle(hWrite);

	char   buffer[4096] = { 0 };
	DWORD   bytesRead;
	while (true)
	{
		memset(buffer, 0, strlen(buffer));
		if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)
			break;
		//buffer�о���ִ�еĽ�������Ա��浽�ı���Ҳ����ֱ�����     
		//printf(buffer);//����ע�͵��Ϳ�����     
		Sleep(100);
	}
	return   TRUE;
}


BOOL FindProcess(string strProc) //�ж�һ�����̣�QQ.exe���Ƿ������У�
{
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "�������̿��վ��ʧ��!" << endl;
		return false;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		string strTemp = WcharToChar(pe32.szExeFile);//WCHARתstring
		//���ﻹ��ʹ��stricmp������㣬����Ҫͳһת���ɴ�д����Сд
		if (0 == _stricmp(strProc.c_str(), strTemp.c_str()))//���Դ�Сд���ַ����Ƚ�
		{
			i += 1;  //�ҵ�    
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	if (i >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//�̺߳���ֻ������ͨ����������ľ�̬����
void startInceptionThread() 
{
	string cmd = "python E:\\VisualProjects\\judgeImage\\Classify_V3.py";
	//string cmd = "python .\\judgeImage\\Classify_V3.py";
	//string cmd = pythonName + " " + resultPath + "\\" + inceptionPyName;
	system_hide(cmd.c_str());
}


int PythonInterface::getInceptionV3()
{
	//�����⵽python�����Ѿ������У�����inception����û����������ô�����������������߳���ռ���ˣ�
	//����Ҫ��python�����Ƚ�����
	if (checkPython() && !isStartInception)
	{
		endPython();
	}
	if (!isStartInception)
	{
		isStartInception = true;
	}

	if (!checkPython())//����̲߳�����
	{
		thread inceptionThread(startInceptionThread);
		inceptionThread.detach();//���߳��ƽ�ϵͳ���������̽�������Ӱ�����̵߳�����
	}

	ifstream fin(resultPath+"\\"+ resultInceptionName);
	string buffer;
	int result = -1;
	if (fin.is_open())
	{
		getline(fin, buffer);
		result = atoi(buffer.c_str());
		fin.close();
	}
	return result;
}

bool PythonInterface::endPython()
{
	if (!checkPython())//���python�ű������Ѿ�������
	{
		isStartInception = false;
		return true;
	}
	if (destroyPython()&& !checkPython())//�������python����ɹ�
	{
		isStartInception = false;
		return true;
	}
	return false;
	

}




bool PythonInterface::checkPython()
{
	if (FindProcess(pythonName+".exe"))
	{
		return true;
	}
	return false;
}

bool PythonInterface::destroyPython()
{
	string cmd = "taskkill /f /t /im " + pythonName + ".exe";
	bool res = system_hide(cmd.c_str());
	if (res)//����ֵ��Ϊtrue����ʾִ�д���
	{
		return true;
	}
	return false;
}

