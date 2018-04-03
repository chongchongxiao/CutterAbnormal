#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	UIO = new UIOperation();
	judgeTime = 1000;//默认1秒
	srcLabel = new QLabel();
	cutterPara = new QComboBox();
	//cutterPara->addItem(QWidget::tr("please select cutter parameter"));
	cutterPara->addItem(QWidget::tr("A4mm"));
	cutterPara->addItem(QWidget::tr("A6mm"));
	cutterPara->addItem(QWidget::tr("A10mm"));

	//QRegExp regExp("^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$");正则表达式一直有问题，暂时放弃
	//QRegExpValidator *pRevalidotor = new QRegExpValidator(rx, this);
	timeDiffEdit = new QLineEdit(this);
	timeDiffEdit->setValidator(new QDoubleValidator(-100,100,2,this));//最小值，最大值，小数点位数
	QPushButton *confirmTimeDiff = new QPushButton(QString("&que ren"));

	//srcLabel->setPixmap();
	//radar = new Radar;
	//radar->setBaseSize(600, 600);
	//radar->setMinimumSize(300, 300);
	QPushButton *start = new QPushButton(QString("&start scan"));
	QPushButton *stop = new QPushButton(QString("&stop scan"));
	QPushButton *end = new QPushButton(QString("&end"));
	QPushButton *selectVideo = new QPushButton(QString("&select file"));
	QPushButton *makeTemplate = new QPushButton(QString("&make template"));
	QPushButton *saveTemplate = new QPushButton(QString("&save template"));
	QPushButton *capture = new QPushButton(QString("&capture"));
	QPushButton *savetmp = new QPushButton(QString("&savetmp"));
	QWidget* mainWidget = new QWidget;
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(srcLabel);
	layout->addWidget(start);
	layout->addWidget(stop);
	layout->addWidget(end);
	layout->addWidget(cutterPara);
	layout->addWidget(timeDiffEdit);
	layout->addWidget(confirmTimeDiff);
	layout->addWidget(selectVideo);
	layout->addWidget(makeTemplate);
	layout->addWidget(saveTemplate);
	layout->addWidget(capture);
	layout->addWidget(savetmp);
	mainWidget->setLayout(layout);
	this->setCentralWidget(mainWidget);
	connect(start, SIGNAL(clicked(bool)), this, SLOT(start()));
	connect(stop, SIGNAL(clicked(bool)), this, SLOT(stop()));
	connect(end, SIGNAL(clicked(bool)), this, SLOT(end()));
	connect(selectVideo, SIGNAL(clicked(bool)), this, SLOT(selectFile()));
	connect(makeTemplate, SIGNAL(clicked(bool)), this, SLOT(makeTmp())); 
	connect(saveTemplate, SIGNAL(clicked(bool)), this, SLOT(saveTemplate()));
	connect(cutterPara, SIGNAL(currentIndexChanged(int)), this, SLOT(selectCutterType()));
	connect(confirmTimeDiff, SIGNAL(clicked(bool)), this, SLOT(confirmTimeDiff()));
	connect(capture, SIGNAL(clicked(bool)), this, SLOT(capture()));
	connect(savetmp, SIGNAL(clicked(bool)), this, SLOT(savetmp()));
}



void MainWindow::start() 
{
	UIO->setConfigePara("AC", "BD");
	UIO->setVideoPath("E:\\VisualProjects\\video\\Camera Roll\\基准视频.mp4");


	if (isStart && !isStop) return;
	isStart = true;	//开始判断
	accumulateTime = 0;//初始化累计时间

	

	if (timer == NULL)
	{
		timer = new QTimer(this);
		//timer->setSingleShot(true)
		connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
		//connect(judgeTimer, SIGNAL(timeout()), this, SLOT(threadEnd()));
		//connect(t, SIGNAL(finished()), this, SLOT(threadEnd()));
		//时间间隔
	}
	if (!timer->isActive())
	{
		timer->start(200);
	}

}
void MainWindow::stop()
{
	Mat tt = imread("E://VisualProjects//video//4mm//Z-5（全程冷却）//frame1.png");
	cvtColor(tt, tt, CV_BGR2RGB);
	if (tt.empty())
	{
		cout << "123" << endl;
		qDebug() << "456" << endl;
		return;
	}
	qImage = QImage((const unsigned char*)(tt.data), tt.cols, tt.rows, tt.cols*tt.channels(), QImage::Format_RGB888);
	srcLabel->setPixmap(QPixmap::fromImage(qImage));
	//srcLabel->resize(srcLabel->pixmap()->size());
	srcLabel->update();
	//srcLabel->show();
	qDebug() << "123" << endl;
}
void MainWindow::end() {}

void MainWindow::selectCutterType()
{
	

}

void MainWindow::confirmTimeDiff()
{
	//UIO->setVideoPath("E://VisualProjects//video//Camera Roll//原始_短刀1.mp4");
	UIO->setConfigePara("AC", "BD");
	vector<string> files;
	string file1 = "E:\\VisualProjects\\video\\20180316video\\WIN_20180316_16_58_21_Pro.mp4";
	string file2 = "E:\\VisualProjects\\video\\20180316video\\WIN_20180316_16_59_11_Pro.mp4";
	files.push_back(file1);
	files.push_back(file2);
	UIO->makeTrainData(files);

}

void MainWindow::selectFile()
{
	qDebug() << "select file" << endl;
	//定义文件对话框类  
	QFileDialog *fileDialog = new QFileDialog(this);
	//定义文件对话框标题  
	fileDialog->setWindowTitle(tr("open file"));
	//设置默认文件路径  
	fileDialog->setDirectory(".");
	//设置文件过滤器  
	//fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置只能选择一个文件
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//设置视图模式  
	//fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径  
	QStringList fileNames;
	if (fileDialog->exec() == QDialog::Accepted) {//如果用户点击了确定
		QString path = fileDialog->selectedFiles()[0];
		UIO->setVideoPath(path.toStdString());
	}
	else {
		//QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
	}
}

void MainWindow::judge()
{
	accumulateTime += judgeTime;

	UIO->setVideoPath("E://VisualProjects//video//camera//无冷却液.avi");
	UIO->setConfigePara("A4mm", "B12");

	/*HistResult* histResult =(HistResult*)UIO->getJudgeResultTest(accumulateTime,HIST);
	bool abnormal = false;//true 表示出现异常
	Mat image = histResult->image;
	imshow("123", histResult->image);
	imshow("456", histResult->stdImage);
	imshow("hist1", histResult->hist);
	imshow("hist2", histResult->stdHist);
	qDebug() << histResult->sim;

	
	
	if (abnormal||image.empty())//如果出现异常或者视频读完,那么结束定时器
	{
		accumulateTime = 0;
		isStart = false;
		isStop = false;
		timer->stop();
		delete timer;
	}
	if (image.empty()) return;//如果返回空图片，那么不重绘
	cvtColor(image, image, CV_BGR2RGB);
	qImage = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_RGB888);
	srcLabel->setPixmap(QPixmap::fromImage(qImage));
	//srcLabel->resize(srcLabel->pixmap()->size());
	srcLabel->update();*/

	
}

void MainWindow::makeTmp()
{
	ConfigPara pa;
	
	pa.cutterType = "AB";
	pa.partType = "CD";
	pa.confID = pa.cutterType;//cutterType+partType
	pa.confID += pa.partType;

	pa.stdVideoPath = "E://VisualProjects//video//Camera Roll//基准视频.mp4";
	pa.isCutCutter = 1;
	TemplatePara templatePara = UIO->makeTmp(pa.stdVideoPath, 5000,150,150);

	pa.distanceCenter = templatePara.distanceCenter;
	pa.cutterRectLength = templatePara.cutterRectLength;
	pa.cutterRectWide = templatePara.cutterRectWide;
	pa.templateImage = templatePara.templateImage;
	pa.histThreshold = 0.8;

	if (UIO->addStdVideo(pa))
	{
		qDebug() << "test suc" << endl;
	}
	/*struct ConfigPara
	{
		string confID;//配置参数的唯一标识符，用刀具和零件的组合来确定
		string cutterType;//刀具类型
		string partType;//零件类型
		string stdVideoPath;//基准视频的路径
		int isCutCutter;//是否裁剪刀具，1表示裁剪,如果为0 则后面三个参数没有意义
		int distanceCenter;//模板框和刀具框中心点的距离
		int cutterRectLength, cutterRectWide;//刀具框的长和宽
		double histThreshold;//直方图阈值
	}*/
}

void MainWindow::saveTemplate()
{
	//copyFileToDir("E://VisualProjects//template//A4mmB12.png", "E://VisualProjects//template//A4mffmB12.png");
}

void MainWindow::capture()
{
	UIO->setVideoPath("E://VisualProjects//video//Camera Roll//原始_短刀1.mp4");
	vector<string> files;
	string file = "E:\\qq文件\\569276205\\FileRecv\\3正常.mp4";
	files.push_back(file);
	UIO->setConfigePara("A", "1");
	//HistResult* histResult = (HistResult*)UIO->getJudgeResultTest(10000, HIST); 
	UIO->makeTrainData(files);
	//int a = UIO->getJudgeResult(5000, INCEPTION_V3);
	//cout << a << endl;
}

void MainWindow::savetmp()
{
	qDebug() << "save" << endl;
	QTimer *t = new QTimer();
	QThread *thread = new QThread(this);
	connect(t, SIGNAL(timeout()), this, SLOT(updateImage()),Qt::UniqueConnection);
	t->start(100);
}


string getTime()
{
	time_t rawtime = time(NULL);
	struct tm * timeinfo;
	timeinfo = localtime(&rawtime);
	char timeNow[256];
	memset(timeNow, '\0', 256);
	//根据当前时间得到文件名(一个月一个log文件)
	sprintf(timeNow, "%04d%02d%02d%02d%02d%02d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	return string(timeNow);
}


void MainWindow::updateImage()
{
	_sleep(1000);
	string ttt = getTime();
	qDebug() << "update" << QString::fromStdString(ttt) << endl;
}

void MainWindow::threadEnd()
{
	qDebug() << 123456 << endl;
}
