#include "PythonInterface.h"



PythonInterface::PythonInterface()
{
}


PythonInterface::~PythonInterface()
{
}

int PythonInterface::getInceptionV3()
{
	return 0;
}

void PythonInterface::test()
{
	Py_Initialize(); /*��ʼ��python������,���߱�����Ҫ�õ�python������*/
	PyRun_SimpleString("import helloworld"); /*����python�ļ�*/
	PyRun_SimpleString("helloworld.printHello()");/*����python�ļ��еĺ���*/
	Py_Finalize(); /*����python���������ͷ���Դ*/
	system("pause");
}
