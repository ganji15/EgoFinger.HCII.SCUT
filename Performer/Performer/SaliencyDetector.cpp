#include "SaliencyDetector.h"


SaliencyDetector::SaliencyDetector()
{
}


SaliencyDetector::~SaliencyDetector()
{
}


void SaliencyDetector::displaySaliencySequence(QString diffImgDir)
{
	int i = 1;
	while (true)
	{
		string path = (diffImgDir + i + ".png").toStdString().data();
		Mat frameDiffImg = imread(path);
		if (frameDiffImg.empty())
			break;
		Mat frameDiffGrayImg;
		cvtColor(frameDiffImg, frameDiffGrayImg, CV_BGR2GRAY);

		initMaps();		//init of maps, which is of performance enhancement
		initHistogram();	//init of histogram, same for enhencing performance

		setColorDiffMap(); //calculate mapping relationship of each value 0-255
		//for performance of lc saliency method
		setDistMap();

		imshow("saliency", getDistMap());
		waitKey(30);
	}
}


//****************
// The followings are private methods
//****************

void SaliencyDetector::initMaps()
{
	//8λ����1ͨ��,��ʼ��colorDiffMap
	colorDiffMap.create(256, 1, CV_8UC1);

	//8λ����1ͨ��,��ʼ��distMap
	distMap.create(imgGray.rows, imgGray.cols, CV_8UC1);
}


void SaliencyDetector::initHistogram()
{
	int narrays = 1; 
	int channels = 0; 
	int dims = 1; 
	int size = 256;
	float hranges[] = { 0, 255 };   
	const float *ranges[] = { hranges };
	calcHist(&frameDiffImg, narrays, &channels, Mat(), hist, dims, &size, ranges);
}

//����0-255��ÿ���Ҷ�ֵ�ĵ��֡����Ϊ�Ҷȱ�����֡������ظ�����
void SaliencyDetector::setColorDiffMap()
{
	uchar value;
	for (int i = 0; i<256; i++)
	{
		value = 0;
		//.ptr<>�����õ�һ�е�ָ�룬Ȼ����[]����������ĳһ�е�����ֵ��ֱ��ͼ��256��1�е�Mat�ṹ
		uchar* histPtr = hist.ptr<uchar>(i);
		uchar* colorDiffMapP = colorDiffMap.ptr<uchar>(i);
		for (int j = 0; j<256; j++)
		{
			value = abs(i - j)*histPtr[1];
		}
		colorDiffMapP[1] = value;
	}
}

void SaliencyDetector::setDistMap()
{
	for (int i = 0; i<imgGray.rows; i++)
	{
		//.ptr<>�����õ�һ�е�ָ�룬Ȼ����[]����������ĳһ�е�����ֵ��rows��cols��
		uchar* imgGrayP = imgGray.ptr<uchar>(i);
		//distMapӦ����imgGray�ȴ�
		uchar* distMapP = distMap.ptr<uchar>(i);
		for (int j = 0; j < imgGray.cols; j++)
		{
			//colorDiffMapΪ256��1��,��ȡimgGray(i,j)�ĻҶȵó���������colorDiffMap��һ���в��ó�distMap
			uchar* colorDiffMapP = colorDiffMap.ptr<uchar>(imgGrayP[j]);
			distMapP[j] = colorDiffMapP[1];
		}

		imshow("distMap", distMap);
	}
}

//��ʾ�Ҷ�ֱ��ͼ
void SaliencyDetector::showHistogram()
{
	int size = 256;
	int scale = 1;
	Mat imageShow(size * scale, size, CV_8U, Scalar(0));
	// ��ȡ���ֵ����Сֵ  
	double minVal = 0;
	double maxVal = 0;
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	//��ʾֱ��ͼ��ͼ��  
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		//hist����float���� 
		float value = hist.at<float>(i);
		int realValue = saturate_cast<int>(value * hpt / maxVal);
		rectangle(imageShow, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
	}
	namedWindow("showGrayImage");
	imshow("showGrayImage", imageShow);
	waitKey(0);
}

Mat SaliencyDetector::getDistMap()
{
	return distMap;
}