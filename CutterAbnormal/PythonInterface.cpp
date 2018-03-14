#include "PythonInterface.h"
#include<windows.h>//
//这里把Windows.h头文件放在这里，主要是这个头文件中的命名与OpenCV相互冲突，放在这里就不会报错
//error C2872: “ACCESS_MASK”: 不明确的符号]的解决方法


PythonInterface::PythonInterface()
{
}


PythonInterface::~PythonInterface()
{
}

LPCWSTR stringToLPCWSTR(std::string orig)//string 转LPCWSTR
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}


LPWSTR ConvertCharToLPWSTR(const char * szString)//char* 转 LPWSTR
{
	int dwLen = strlen(szString) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//算出合适的长度
	LPWSTR lpszPath = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
	return lpszPath;
}


BOOL system_hide(const char* CommandLine)//执行cmd命令不弹窗
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
	//关键步骤，CreateProcess函数参数意义请查阅MSDN 
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
		//buffer中就是执行的结果，可以保存到文本，也可以直接输出     
		//printf(buffer);//这行注释掉就可以了     
		Sleep(100);
	}
	return   TRUE;
}



int PythonInterface::getInceptionV3()
{
	
	//char cmdexe[100];
	//strcpy_s(cmdexe, cmd.c_str());
	bool res = system_hide(cmdInception.c_str());
	if (!res)//返回值不为true，表示执行错误
	{
		return -1;
	}
	ifstream fin(resultInception);
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

void PythonInterface::test()
{
}

