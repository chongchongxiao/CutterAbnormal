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
	void makeTmp();//制作模板
	void saveTemplate();
	void capture();
	void savetmp();

private:
	Ui::MainWindowClass ui;
	Mat srcImage;//OpenCV传回的待显示的图片
	QImage qImage;//待显示的图片
	QLabel *srcLabel;//显示图片的label
	QComboBox *cutterPara;//刀具参数下拉选择框
	QLineEdit *timeDiffEdit;//基准视频和待测视频时差输入框
	UIOperation *UIO;
	QTimer *timer;//定时器,
	int judgeTime;//隔judgeTime个时间，就需要重新判断一次，单位ms
	int accumulateTime=0;//开始累计时间
	bool isStop=false;
	bool isStart = false;
};
