#include "PythonInterface.h"
#include<windows.h>//CreateProcess
//这里把Windows.h头文件放在这里，主要是这个头文件中的命名与OpenCV相互冲突，放在这里就不会报错
//error C2872: “ACCESS_MASK”: 不明确的符号]的解决方法
#include<tlhelp32.h>//PROCESSENTRY32  CreateToolhelp32Snapshot  TH32CS_SNAPPROCESS  Process32First Process32Next

PythonInterface::PythonInterface()
{
	resultPath = getPath("judgeImage");
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


std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)//WCHAR 转 string
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



BOOL system_hide(const char* CommandLine)//执行cmd命令不弹窗，不能声明为类的内部函数，因为window.h与OpenCV冲突
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


BOOL FindProcess(string strProc) //判断一个进程（QQ.exe）是否在运行，
{
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "创建进程快照句柄失败!" << endl;
		return false;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		string strTemp = WcharToChar(pe32.szExeFile);//WCHAR转string
		//这里还是使用stricmp函数最方便，否则还要统一转换成大写或者小写
		if (0 == _stricmp(strProc.c_str(), strTemp.c_str()))//忽略大小写的字符串比较
		{
			i += 1;  //找到    
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


//线程函数只能是普通函数或是类的静态函数
void startInceptionThread() 
{
	string cmd = "python E:\\VisualProjects\\judgeImage\\Classify_V3.py";
	//string cmd = "python .\\judgeImage\\Classify_V3.py";
	//string cmd = pythonName + " " + resultPath + "\\" + inceptionPyName;
	system_hide(cmd.c_str());
}


int PythonInterface::getInceptionV3()
{
	//如果检测到python程序已经在运行，但是inception程序没有启动，那么可能是其他方法或者程序占用了，
	//所以要把python程序先结束掉
	if (checkPython() && !isStartInception)
	{
		endPython();
	}
	if (!isStartInception)
	{
		isStartInception = true;
	}

	if (!checkPython())//如果线程不存在
	{
		thread inceptionThread(startInceptionThread);
		inceptionThread.detach();//把线程移交系统管理，主进程结束不会影响子线程的运行
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
	if (!checkPython())//如果python脚本程序已经结束掉
	{
		isStartInception = false;
		return true;
	}
	if (destroyPython()&& !checkPython())//如果结束python程序成功
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
	if (res)//返回值不为true，表示执行错误
	{
		return true;
	}
	return false;
}

