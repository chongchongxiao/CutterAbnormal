#include"makeTemplate.h"


ImageTemplate IMAGETEMPLATE = ImageTemplate();

void makeDoubleRect(const Mat& image)//��������ģ���
{
	Point templateRectCenter;//ģ������ĵ�
	Point cutterRectCenter;//���߿�����ĵ�
	Mat tempImage;//��ʱͼƬ
	templateRectCenter.x = (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x) / 2;
	templateRectCenter.y = (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y) / 2;
	cutterRectCenter.x = templateRectCenter.x;
	cutterRectCenter.y = templateRectCenter.y + IMAGETEMPLATE.distanceCenter;

	image.copyTo(tempImage);//����һ�ݵ�����һ���洢λ�ã���ֹ���ݹ���Ӱ��ģ��ͼƬ
	rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 255, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
	rectangle(tempImage, Point(cutterRectCenter.x - IMAGETEMPLATE.cutterRectWide / 2, cutterRectCenter.y - IMAGETEMPLATE.cutterRectLength / 2),
		Point(cutterRectCenter.x + IMAGETEMPLATE.cutterRectWide / 2, cutterRectCenter.y + IMAGETEMPLATE.cutterRectLength / 2), Scalar(0, 0, 255));
	//rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 0, 255, 0), 1, 8, 0);
	line(tempImage, templateRectCenter, cutterRectCenter, Scalar(255, 255, 0));
	imshow("SrcImage", tempImage);

	//waitKey();
}

void onMouseTmp(int event, int x, int y, int flags, void *ustc)//����¼��ص��������������ִ�е�����Ӧ�ڴ�
{

	Mat image = *(Mat*)ustc;
	Mat tempImage;
	image.copyTo(tempImage);
	if (event == CV_EVENT_LBUTTONDOWN && !IMAGETEMPLATE.drawedTemplate)//��������һ�ΰ����¼�����ʼ����ģ���
	{
		IMAGETEMPLATE.drawingTemplate = true;//��־�ڻ�ģ���
		IMAGETEMPLATE.templageRectStart.x = x;//��¼���εĿ�ʼ�ĵ�
		IMAGETEMPLATE.templageRectStart.y = y;
	}
	else if (event == CV_EVENT_LBUTTONDOWN && IMAGETEMPLATE.drawedTemplate)//�ǵ�һ�ΰ����
	{
		if (x >= IMAGETEMPLATE.templageRectStart.x&&x <= IMAGETEMPLATE.templageRectEnd.x
			&&y >= IMAGETEMPLATE.templageRectStart.y&&y <= IMAGETEMPLATE.templageRectEnd.y)//���ѡ��ģ���,���ͬʱѡ��ģ���͵��߿�Ĺ�ͬ���֣�Ĭ��ѡ��ģ���
		{
			IMAGETEMPLATE.isTemplateMove = true;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
		}
		else if (x >= (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x - IMAGETEMPLATE.cutterRectWide) / 2 //ѡ�е��߿�
			&& x <= (IMAGETEMPLATE.templageRectStart.x + IMAGETEMPLATE.templageRectEnd.x + IMAGETEMPLATE.cutterRectWide) / 2
			&& y >= (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y - IMAGETEMPLATE.cutterRectLength) / 2 + IMAGETEMPLATE.distanceCenter
			&& y <= (IMAGETEMPLATE.templageRectStart.y + IMAGETEMPLATE.templageRectEnd.y + IMAGETEMPLATE.cutterRectLength) / 2 + IMAGETEMPLATE.distanceCenter)
		{
			IMAGETEMPLATE.isCutterMove = true;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
		}

	}
	else if (event == CV_EVENT_LBUTTONUP)//�������ɿ��¼�
	{
		if (!IMAGETEMPLATE.drawedTemplate&&IMAGETEMPLATE.drawingTemplate)//�����һ���ɿ�����ʾ����ģ���
		{

			IMAGETEMPLATE.drawingTemplate = false;
			IMAGETEMPLATE.drawedTemplate = true;//�����һ���ɿ�����ʾ����ģ���
			IMAGETEMPLATE.distanceCenter = (IMAGETEMPLATE.templageRectEnd.y - IMAGETEMPLATE.templageRectStart.y) / 2 + IMAGETEMPLATE.cutterRectLength / 2;
			makeDoubleRect(tempImage);
		}
		else if (IMAGETEMPLATE.isTemplateMove)
		{
			IMAGETEMPLATE.isTemplateMove = false;
			int xDiff = x - IMAGETEMPLATE.lastLocation.x;
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.templageRectStart.x += xDiff;//�ƶ�ģ����λ��
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
	else if (event == CV_EVENT_MOUSEMOVE && flags == CV_EVENT_FLAG_LBUTTON)//��ס�϶�
	{
		if (IMAGETEMPLATE.drawingTemplate)//����ģ���
		{
			IMAGETEMPLATE.templageRectEnd.x = x;
			IMAGETEMPLATE.templageRectEnd.y = y;
			rectangle(tempImage, IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd, Scalar(0, 255, 0, 0), 1, 8, 0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
			imshow("SrcImage", tempImage);
		}
		else if (IMAGETEMPLATE.isTemplateMove)//�϶�ģ���
		{
			int xDiff = x - IMAGETEMPLATE.lastLocation.x;
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.templageRectStart.x += xDiff;//�ƶ�ģ����λ��
			IMAGETEMPLATE.templageRectEnd.x += xDiff;
			IMAGETEMPLATE.templageRectStart.y += yDiff;
			IMAGETEMPLATE.templageRectEnd.y += yDiff;
			IMAGETEMPLATE.lastLocation.x = x;//������һ�����λ��
			IMAGETEMPLATE.lastLocation.y = y;
			makeDoubleRect(tempImage);
		}
		else if (IMAGETEMPLATE.isCutterMove)//�϶����߿�ֻ���������϶�����֤��������������ͬһ����ֱλ��
		{
			int yDiff = y - IMAGETEMPLATE.lastLocation.y;
			IMAGETEMPLATE.distanceCenter += yDiff;
			IMAGETEMPLATE.lastLocation.x = x;
			IMAGETEMPLATE.lastLocation.y = y;
			makeDoubleRect(tempImage);
		}
	}
}


Mat resizeAdaptive(const Mat&src)//����Ӧ����ͼƬ��С
{
	int maxRows = 800;//�к��е����ֵ
	int maxCols = 1000;
	double f_row = 1, f_col = 1;//�к��е����ű���
	int row = src.rows, col = src.cols;
	Mat dst;//����֮���ͼ��
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
	resize(src, dst, Size(0, 0), f_col, f_row, INTER_LINEAR);//˫���Բ�ֵ����
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

	double f_row = image.rows / src.rows;//���ű���
	double f_col = image.cols / src.cols;


	IMAGETEMPLATE.cutterRectWide /= f_col; //�Ե��ߵ�ģ�����ͬ��������
	IMAGETEMPLATE.cutterRectLength /= f_row;

	IMAGETEMPLATE.init();//�Ա�����ʼ��
	namedWindow("SrcImage");
	imshow("SrcImage", src);
	setMouseCallback("SrcImage", onMouseTmp, &src);//�����������м������Ӷ�ʵ�ֿ�ͼ
	if (waitKey() == 13)//�������֮�󣬻س����߹ر��������ڽ�������
	{
		destroyWindow("SrcImage");//ֻ�н������������ڣ��Ż���������ļ���
	}

	//Mat imageTemplate = src(Rect(IMAGETEMPLATE.templageRectStart, IMAGETEMPLATE.templageRectEnd));//������ɵ�ģ���

	Point start,end; //��������ģ�尴��ģ�廹ԭ��ԭʼͼ����
	start.x = IMAGETEMPLATE.templageRectStart.x*f_col;
	start.y = IMAGETEMPLATE.templageRectStart.y*f_row;
	end.x = IMAGETEMPLATE.templageRectEnd.x*f_col;
	end.y = IMAGETEMPLATE.templageRectEnd.y*f_row;

	IMAGETEMPLATE.distanceCenter *= f_row;//�����ĵ�ľ��밴��ͬ��������

	Mat srcImageTemplate = image(Rect(start, end));
	waitKey();

	return srcImageTemplate;
}
