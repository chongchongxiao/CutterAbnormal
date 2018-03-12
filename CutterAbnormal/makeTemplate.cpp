#include"makeTemplate.h"


ImageTemplate IMAGETEMPLATE = ImageTemplate();

void makeDoubleRect(const Mat& image)//制作两个模板框
{
	Point templateRectCenter;//模板框中心点
	Point cutterRectCenter;//刀具框的中心点
	Mat tempImage;//临时图片
	templateRectCenter.x = (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x) / 2;
	templateRectCenter.y = (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y) / 2;
	cutterRectCenter.x = templateRectCenter.x;
	cutterRectCenter.y = templateRectCenter.y + IMAGETEMPLATE.distanceCenter;

	image.copyTo(tempImage);//复制一份到另外一个存储位置，防止数据共享影响模板图片
	rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 255, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
	rectangle(tempImage, Point(cutterRectCenter.x - IMAGETEMPLATE.cutterRectWide / 2, cutterRectCenter.y - IMAGETEMPLATE.cutterRectLength / 2),
		Point(cutterRectCenter.x + IMAGETEMPLATE.cutterRectWide / 2, cutterRectCenter.y + IMAGETEMPLATE.cutterRectLength / 2), Scalar(0, 0, 255));
	//rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 0, 255, 0), 1, 8, 0);
	line(tempImage, templateRectCenter, cutterRectCenter, Scalar(255, 255, 0));
	imshow("SrcImage", tempImage);

	//waitKey();
}

void onMouseTmp(int event, int x, int y, int flags, void *ustc)//鼠标事件回调函数，鼠标点击后执行的内容应在此
{

	Mat image = *(Mat*)ustc;
	Mat tempImage;
	image.copyTo(tempImage);
	if (event == CV_EVENT_LBUTTONDOWN && !IMAGETEMPLATE.drawedTemplate)//鼠标左键第一次按下事件，开始绘制模板框
	{
		IMAGETEMPLATE.drawingTemplate = true;//标志在画模板框
		IMAGETEMPLATE.templageRectStart.x = x;//记录矩形的开始的点
		IMAGETEMPLATE.templageRectStart.y = y;
	}
	else if (event == CV_EVENT_LBUTTONDOWN && IMAGETEMPLATE.drawedTemplate)//非第一次按左键
	{
		if (x >= IMAGETEMPLATE.templageRectStart.x&&x <= IMAGETEMPLATE.templageRectEnd.x
			&&y >= IMAGETEMPLATE.templageRectStart.y&&y <= IMAGETEMPLATE.templageRectEnd.y)//如果选中模板框,如果同时选中模板框和刀具框的共同部分，默认选中模板框
		{
			IMAGETEMPLATE.isTemplateMove = true;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
		}
		else if (x >= (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x - IMAGETEMPLATE.cutterRectWide) / 2 //选中刀具框
			&& x <= (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x + IMAGETEMPLATE.cutterRectWide) / 2
			&& y >= (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y - IMAGETEMPLATE.cutterRectLength) / 2 + IMAGETEMPLATE.distanceCenter
			&& y <= (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y + IMAGETEMPLATE.cutterRectLength) / 2 + IMAGETEMPLATE.distanceCenter)
		{
			IMAGETEMPLATE.isCutterMove = true;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
		}

	}
	else if (event == CV_EVENT_LBUTTONUP)//鼠标左键松开事件
	{
		if (!IMAGETEMPLATE.drawedTemplate&&IMAGETEMPLATE.drawingTemplate)//左键第一次松开，表示画完模板框
		{

			IMAGETEMPLATE.drawingTemplate = false;
			IMAGETEMPLATE.drawedTemplate = true;//左键第一次松开，表示画完模板框
			IMAGETEMPLATE.distanceCenter = (IMAGETEMPLATE.templageRectEnd.y - IMAGETEMPLATE.templageRectStart.y) / 2 + IMAGETEMPLATE.cutterRectLength / 2;
			makeDoubleRect(tempImage);
		}
		else if (IMAGETEMPLATE.isTemplateMove)
		{
			IMAGETEMPLATE.isTemplateMove = false;
			int xDiff = x - IMAGETEMPLATE.lastLocation.x;
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.templageRectStart.x += xDiff;//移动模板框的位置
			IMAGETEMPLATE.templageRectEnd.x += xDiff;
			IMAGETEMPLATE.templageRectStart.y += yDiff;
			IMAGETEMPLATE.templageRectEnd.y += yDiff;
			makeDoubleRect(tempImage);
		}
		else if (IMAGETEMPLATE.isCutterMove)
		{
			IMAGETEMPLATE.isCutterMove = false;
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.distanceCenter += yDiff;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
			makeDoubleRect(tempImage);
		}
	}
	else if (event == CV_EVENT_MOUSEMOVE && flags == CV_EVENT_FLAG_LBUTTON)//按住拖动
	{
		if (IMAGETEMPLATE.drawingTemplate)//绘制模板框
		{
			IMAGETEMPLATE.templageRectEnd.x = x;
			IMAGETEMPLATE.templageRectEnd.y = y;
			rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
			imshow("SrcImage", tempImage);
		}
		else if (IMAGETEMPLATE.isTemplateMove)//拖动模板框
		{
			int xDiff = x - IMAGETEMPLATE.lastLocation.x;
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.templageRectStart.x += xDiff;//移动模板框的位置
			IMAGETEMPLATE.templageRectEnd.x += xDiff;
			IMAGETEMPLATE.templageRectStart.y += yDiff;
			IMAGETEMPLATE.templageRectEnd.y += yDiff;
			IMAGETEMPLATE.lastLocation.x = x;//更新上一个点的位置
			IMAGETEMPLATE.lastLocation.y = y;
			makeDoubleRect(tempImage);
		}
		else if (IMAGETEMPLATE.isCutterMove)//拖动刀具框，只可以上下拖动，保证在两个框中心在同一个垂直位置
		{
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.distanceCenter += yDiff;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
			makeDoubleRect(tempImage);
		}
	}
}


Mat resizeAdaptive(const Mat&src)//自适应调整图片大小
{
	int maxRows = 800;//行和列的最大值
	int maxCols = 1000;
	double f_row = 1, f_col = 1;//行和列的缩放比例
	int row = src.rows, col = src.cols;
	Mat dst;//缩放之后的图像
	while (row > maxRows)
	{
		row /= 2;
		f_row /= 2;
	}
	while (col > maxCols)
	{
		col /= 2;
		f_col /= 2;
	}
	resize(src, dst, Size(0, 0), f_col, f_row, INTER_LINEAR);//双线性插值缩放
	return dst;

}


Mat makeTemplate(Mat& image)
{

	if (image.data == 0)
	{
		cout << "error, the src image is not built!" << endl;
		return Mat();
	}

	Mat src = resizeAdaptive(image);

	double f_row = image.rows / src.rows;//缩放比例
	double f_col = image.cols / src.cols;


	IMAGETEMPLATE.cutterRectWide /= f_col; //对刀具的模板框按照同比例缩放
	IMAGETEMPLATE.cutterRectLength /= f_row;

	IMAGETEMPLATE.init();//对变量初始化
	namedWindow("SrcImage");
	imshow("SrcImage", src);
	setMouseCallback("SrcImage", onMouseTmp, &src);//对鼠标操作进行监听，从而实现抠图
	if (waitKey() == 13)//制作完成之后，回车或者关闭制作窗口结束制作
	{
		destroyWindow("SrcImage");//只有结束掉监听窗口，才会结束对鼠标的监听
	}

	//Mat imageTemplate = src(Rect(IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd));//最后生成的模板框

	Point start,end; //对制作的模板按照模板还原到原始图像中
	start.x = IMAGETEMPLATE.templageRectStart.x*f_col;
	start.y = IMAGETEMPLATE.templageRectStart.y*f_row;
	end.x = IMAGETEMPLATE.templageRectEnd.x*f_col;
	end.y = IMAGETEMPLATE.templageRectEnd.y*f_row;

	IMAGETEMPLATE.distanceCenter *= f_row;//对中心点的距离按照同比例缩放

	Mat srcImageTemplate = image(Rect(start, end));
	waitKey();

	return srcImageTemplate;
}
