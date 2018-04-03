#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	UIO = new UIOperation();
	judgeTime = 1000;//Ĭ��1��
	srcLabel = new QLabel();
	cutterPara = new QComboBox();
	//cutterPara->addItem(QWidget::tr("please select cutter parameter"));
	cutterPara->addItem(QWidget::tr("A4mm"));
	cutterPara->addItem(QWidget::tr("A6mm"));
	cutterPara->addItem(QWidget::tr("A10mm"));

	//QRegExp regExp("^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$");������ʽһֱ�����⣬��ʱ����
	//QRegExpValidator *pRevalidotor = new QRegExpValidator(rx, this);
	timeDiffEdit = new QLineEdit(this);
	timeDiffEdit->setValidator(new QDoubleValidator(-100,100,2,this));//��Сֵ�����ֵ��С����λ��
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
	UIO->setVideoPath("E:\\VisualProjects\\video\\Camera Roll\\��׼��Ƶ.mp4");


	if (isStart && !isStop) return;
	isStart = true;	//��ʼ�ж�
	accumulateTime = 0;//��ʼ���ۼ�ʱ��

	

	if (timer == NULL)
	{
		timer = new QTimer(this);
		//timer->setSingleShot(true)
		connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
		//connect(judgeTimer, SIGNAL(timeout()), this, SLOT(threadEnd()));
		//connect(t, SIGNAL(finished()), this, SLOT(threadEnd()));
		//ʱ����
	}
	if (!timer->isActive())
	{
		timer->start(200);
	}

}
void MainWindow::stop()
{
	Mat tt = imread("E://VisualProjects//video//4mm//Z-5��ȫ����ȴ��//frame1.png");
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
	//UIO->setVideoPath("E://VisualProjects//video//Camera Roll//ԭʼ_�̵�1.mp4");
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
	//�����ļ��Ի�����  
	QFileDialog *fileDialog = new QFileDialog(this);
	//�����ļ��Ի������  
	fileDialog->setWindowTitle(tr("open file"));
	//����Ĭ���ļ�·��  
	fileDialog->setDirectory(".");
	//�����ļ�������  
	//fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//����ֻ��ѡ��һ���ļ�
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//������ͼģʽ  
	//fileDialog->setViewMode(QFileDialog::Detail);
	//��ӡ����ѡ����ļ���·��  
	QStringList fileNames;
	if (fileDialog->exec() == QDialog::Accepted) {//����û������ȷ��
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

	UIO->setVideoPath("E://VisualProjects//video//camera//����ȴҺ.avi");
	UIO->setConfigePara("A4mm", "B12");

	/*HistResult* histResult =(HistResult*)UIO->getJudgeResultTest(accumulateTime,HIST);
	bool abnormal = false;//true ��ʾ�����쳣
	Mat image = histResult->image;
	imshow("123", histResult->image);
	imshow("456", histResult->stdImage);
	imshow("hist1", histResult->hist);
	imshow("hist2", histResult->stdHist);
	qDebug() << histResult->sim;

	
	
	if (abnormal||image.empty())//��������쳣������Ƶ����,��ô������ʱ��
	{
		accumulateTime = 0;
		isStart = false;
		isStop = false;
		timer->stop();
		delete timer;
	}
	if (image.empty()) return;//������ؿ�ͼƬ����ô���ػ�
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

	pa.stdVideoPath = "E://VisualProjects//video//Camera Roll//��׼��Ƶ.mp4";
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
		string confID;//���ò�����Ψһ��ʶ�����õ��ߺ�����������ȷ��
		string cutterType;//��������
		string partType;//�������
		string stdVideoPath;//��׼��Ƶ��·��
		int isCutCutter;//�Ƿ�ü����ߣ�1��ʾ�ü�,���Ϊ0 �������������û������
		int distanceCenter;//ģ���͵��߿����ĵ�ľ���
		int cutterRectLength, cutterRectWide;//���߿�ĳ��Ϳ�
		double histThreshold;//ֱ��ͼ��ֵ
	}*/
}

void MainWindow::saveTemplate()
{
	//copyFileToDir("E://VisualProjects//template//A4mmB12.png", "E://VisualProjects//template//A4mffmB12.png");
}

void MainWindow::capture()
{
	UIO->setVideoPath("E://VisualProjects//video//Camera Roll//ԭʼ_�̵�1.mp4");
	vector<string> files;
	string file = "E:\\qq�ļ�\\569276205\\FileRecv\\3����.mp4";
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
	//���ݵ�ǰʱ��õ��ļ���(һ����һ��log�ļ�)
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
