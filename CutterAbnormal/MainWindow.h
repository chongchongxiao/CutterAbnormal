#pragma once
#include <iostream>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <qDebug>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QTimer>
#include<opencv2/opencv.hpp>
#include "UIOperation.h"
using namespace std;
using namespace cv;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private slots :
	void start();
	void stop();
	void end();
	void selectCutterType();
	void confirmTimeDiff();
	void selectFile();
	void judge();
	void makeTmp();//����ģ��
	void saveTemplate();
	void capture();
	void savetmp();

private:
	Ui::MainWindowClass ui;
	Mat srcImage;//OpenCV���صĴ���ʾ��ͼƬ
	QImage qImage;//����ʾ��ͼƬ
	QLabel *srcLabel;//��ʾͼƬ��label
	QComboBox *cutterPara;//���߲�������ѡ���
	QLineEdit *timeDiffEdit;//��׼��Ƶ�ʹ�����Ƶʱ�������
	UIOperation *UIO;
	QTimer *timer;//��ʱ��,
	int judgeTime;//��judgeTime��ʱ�䣬����Ҫ�����ж�һ�Σ���λms
	int accumulateTime=0;//��ʼ�ۼ�ʱ��
	bool isStop=false;
	bool isStart = false;
};
