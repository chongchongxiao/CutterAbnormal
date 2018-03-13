#include "PythonInterface.h"



PythonInterface::PythonInterface()
{
}


PythonInterface::~PythonInterface()
{
}

int PythonInterface::getInceptionV3()
{
	Py_Initialize();//初始化python运行环境
	PyObject* pModule = PyImport_ImportModule("Classify_V3");//py文件名
	if (!pModule)//读取模型失败
	{
		return -1;
	}
	PyObject* pv = PyObject_GetAttrString(pModule, "classify");
	PyObject* args = PyTuple_New(0);
	//string path = "E:\\VisualProjects\\video\\Camera Roll\\基准视频Cutter\\基准视频frame0.png";
	//PyObject* arg1 = PyByteArray_FromStringAndSize(path.c_str(), path.size());

	//将arg1配置为arg带入的第一个参数  
	//PyTuple_SetItem(args, 0, arg1);
	PyObject*t = PyObject_CallObject(pv, args);
	int a = _PyLong_AsInt(t);
	Py_Finalize(); /*结束python解释器，释放资源*/
	if (a == 0) return 1;  //交换一下1和0，最终结果1表示异常，0表示正常
	else if (a == 1) return 0;
	else return a;
	
}

void PythonInterface::test()
{
}
