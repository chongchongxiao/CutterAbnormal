#include "PythonInterface.h"



PythonInterface::PythonInterface()
{
}


PythonInterface::~PythonInterface()
{
}

int PythonInterface::getInceptionV3()
{
	Py_Initialize();//��ʼ��python���л���
	PyObject* pModule = PyImport_ImportModule("Classify_V3");//py�ļ���
	if (!pModule)//��ȡģ��ʧ��
	{
		return -1;
	}
	PyObject* pv = PyObject_GetAttrString(pModule, "classify");
	PyObject* args = PyTuple_New(0);
	//string path = "E:\\VisualProjects\\video\\Camera Roll\\��׼��ƵCutter\\��׼��Ƶframe0.png";
	//PyObject* arg1 = PyByteArray_FromStringAndSize(path.c_str(), path.size());

	//��arg1����Ϊarg����ĵ�һ������  
	//PyTuple_SetItem(args, 0, arg1);
	PyObject*t = PyObject_CallObject(pv, args);
	int a = _PyLong_AsInt(t);
	Py_Finalize(); /*����python���������ͷ���Դ*/
	if (a == 0) return 1;  //����һ��1��0�����ս��1��ʾ�쳣��0��ʾ����
	else if (a == 1) return 0;
	else return a;
	
}

void PythonInterface::test()
{
}
