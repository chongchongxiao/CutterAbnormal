#include "Cutter.h"


Cutter::Cutter()
{

	histThreshold = 0.7;
	judgeImagePath = getPath("judgeImage");//��ȡ·��
	pi = new PythonInterface();
}


Cutter::~Cutter()
{
	delete pi;
}


double Cutter::histCompare(const Mat&image1, const Mat&image2)
{
	//imshow("pic1", pic1);
	//imshow("pic2", pic2);
	//�������ƶ�
	Mat pic1, pic2;
	//���ﲻcopyһ�ݵĻ������ԭ������Ӱ��
	image1.copyTo(pic1);
	image2.copyTo(pic2);

	cvtColor(pic1, pic1, CV_BGR2GRAY);//��ֱ��ͼ�Ƚ�ǰ��ȫ��תΪ�Ҷ�ͼ��
	cvtColor(pic2, pic2, CV_BGR2GRAY);

	if (pic2.channels() == 1) {//��ͨ��ʱ��
		int channels[1] = { 0 };
		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true;
		bool accumulate = false;

		Mat hist1, hist2;
		Mat hist1Img, hist2Img;

		//����ֱ��ͼ����ʾ
		calcHist(&pic1, 1, { 0 }, Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate);
		normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
		hist1Img = getHistImg(hist1);
		//imshow("hist1", hist1Img);

		calcHist(&pic2, 1, 0, Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate);
		normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
		hist2Img = getHistImg(hist2);
		//imshow("hist2", hist2Img);

		double dSimilarity = compareHist(hist1, hist2, CV_COMP_CORREL);//,CV_COMP_CHISQR,CV_COMP_INTERSECT,CV_COMP_BHATTACHARYYA    CV_COMP_CORREL

		histResult.hist = hist1Img;//������ʾ����Ҫ��ʾֱ��ͼ
		histResult.stdHist = hist2Img;
		histResult.sim = dSimilarity;

		return dSimilarity;
			//cout << "similarity = " << dSimilarity << endl;
	}
	else {//��ͨ��ʱ
		cvtColor(pic1, pic1, COLOR_BGR2HSV);
		cvtColor(pic2, pic2, COLOR_BGR2HSV);

		int h_bins = 50, s_bins = 60;
		int histSize[] = { h_bins, s_bins };
		float h_ranges[] = { 0, 180 };
		float s_ranges[] = { 0, 256 };
		const float* ranges[] = { h_ranges, s_ranges };
		int channels[] = { 0, 1 };

		MatND hist1, hist2;
		//Mat hist1Img, hist2Img;

		calcHist(&pic1, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false);
		//hist1Img = getHistImg(hist1);
		//imshow("hist1", hist1Img);
		normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&pic2, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
		//hist2Img = getHistImg(hist2);
		//imshow("hist2", hist2Img);
		normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());

		double dSimilarity = compareHist(hist1, hist2, CV_COMP_CORREL); //,CV_COMP_CHISQR,CV_COMP_INTERSECT,CV_COMP_BHATTACHARYYA  CV_COMP_CORREL

		return dSimilarity;
	}
}

/*bool Cutter::compareImage(const Mat&image1, const Mat& image2, int method)
{
	double sim;
	switch (method)
	{
	case HIST:

		sim = histCompare(image1, image2);
		return sim > histThreshold ? false : true;
		break;//���Ӧ��û�ð�

	default:
		return false;
		break;
	}
}*/

int Cutter::compareImage(const Mat & image1, const Mat & image2, int method)
{
	if (image1.empty() || image2.empty())
	{
		return -1;
	}

	Mat img1, img2;

	img1 = image1;
	img2 = image2;


	switch (method)
	{
	case HIST:
	{
		double sim = histCompare(img1, img2);//����ͼƬ���ƶ�
		int result = sim < histThreshold ? 1 : 0;//������ƶ�С���쳣�����ʾ�쳣��1�쳣��0����
		return result;
		break;//���Ӧ��û�ð�
	}
	default:
		return -1;
		break;
	}
}

void* Cutter::compareImageTest(const Mat&image1, const Mat& image2, int method)
{
	Mat img1, img2;

	img1 = image1;
	img2 = image2;


	switch (method)
	{
	case HIST:
		histResult.image = image1;
		histResult.stdImage = image2;
		histResult.sim = histCompare(img1, img2);
		histResult.abnormal = histResult.sim < histThreshold;//�Ƿ��쳣��true��ʾ�쳣
		return &histResult;
		break;//���Ӧ��û�ð�
	default:
		return NULL;
		break;
	}
}

int Cutter::judgeImage(const Mat & image, int method)
{
	switch (method)
	{
	case INCEPTION_V3:
	{
		int result = getInceptionV3(image);
		return result;
		break;
	}
	default:
		return NULL;
		break;
	}
}

bool Cutter::endJudgeImage(int method)
{
	return pi->endPython(method);
}

void Cutter::setHistThreshold(double tr)
{
	histThreshold = tr;
}





Mat Cutter::getHistImg(const MatND& hist)
{
	double maxVal = 0;
	double minVal = 0;

	//�ҵ�ֱ��ͼ�е����ֵ����Сֵ
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	int histSize = hist.rows;//histΪrows*1��������
	Mat histImg(histSize, histSize, CV_8U, Scalar(255));
	// ��������ֵΪͼ��߶ȵ�90%
	int hpt = static_cast<int>(0.9*histSize);

	/*MatConstIterator_<uchar> grayit, grayend;
	for (grayit = hist.begin<uchar>(), grayend = hist.end<uchar>(); grayit != grayend; grayit++)
	{
	float binVal = *grayit;
	int intensity = binVal*hpt / maxVal;
	line(histImg, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
	}*/

	for (int h = 0; h<histSize; h++)
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal*hpt / maxVal);//��ÿ�����ص�ĸ߶ȱ�Ϊԭ����90% 
		line(histImg, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
	}
	return histImg;
}

int Cutter::getInceptionV3(const Mat & image)
{
	if (!createDirectory(judgeImagePath)) return -1;
	imwrite(judgeImagePath+"\\"+inceptionImage,image);
	return pi->getInceptionV3();
}


