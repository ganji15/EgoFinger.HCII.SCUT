#include "processor.h"


Processor::Processor()
{

}

Processor::~Processor()
{

}

//*****************
// ����ͼƬ
//*****************
void Processor::readImage(string path)
{
    src = imread(path);

    if(src.empty())
    {
        qDebug("error while reading image");
        return;
    }

    src.copyTo(dst);
    cv::resize(dst,dst,cv::Size(640,480));

    clickedTimes = 0;
    hasFinished = false;
	tlx = -1; tly = -1;
	brx = -1; bry = -1;
	iftx = -1; ifty = -1;
	ifjx = -1; ifjy = -1;
	tfjx = -1; tfjy = -1;
	btx = -1; bty = -1;
	bjx = -1; bjy = -1;
}

//*****************
// ÿ�������������õ�
//*****************
void Processor::addCircleInImage(int x, int y)
{
    clickedTimes++;
	cv::Rect rect;
    switch(clickedTimes)
    {
    case 1:
		tlx = x; tly = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), 3);
        break;
    case 2:
		brx = x; bry = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), 3);
		rect=cv::Rect_<int>(tlx, tly, brx - tlx, bry - tly);
		rectangle(dst, rect, cv::Scalar(0, 0, 0), 1);
        break;
    case 3:
		iftx = x; ifty = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 255, 255), 3);
		break;
	case 4:
		ifjx = x; ifjy = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 255, 255), 3);
        break;
	case 5:
		tfjx = x; tfjy = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(255, 0, 0), 3);
		break;
	case 6:
		btx = x; bty = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), 3);
		break;
	case 7:
		bjx = x; bjy = y;
		circle(dst, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), 3);
		hasFinished = true;
		break;
    }
}

//*****************
// ������ݵ��Ƿ�Խ��
//*****************
bool Processor::checkPoints()
{
    if(!hasFinished)
        return false;

	//check tl and br
    if(brx<tlx || bry<tly)
    {
		return false;
    }
	//keep ift inside
    if(iftx>brx || ifty>bry || iftx < tlx || ifty < tly)
    {
		return false;
    }
	//keep ifj inside
	if (ifjx>brx || ifjy>bry || ifjx < tlx || ifjy < tly)
	{
		return false;
	}
	//keep ifj inside
	if (ifjx>brx || ifjy>bry || ifjx < tlx || ifjy < tly)
	{
		return false;
	}
	//keep tfj inside
	if (tfjx>brx || tfjy>bry || tfjx < tlx || tfjy < tly)
	{
		return false;
	}


	return true;
}

//*****************
// �����ǰͼ�񣨼ӵ��У�
//*****************
Mat Processor::showDstImage()
{
    return dst;
}

//*****************
// ����ͼ�񣨶����������ݣ�
//*****************
Mat Processor::showRecordedImage(vector<float> v)
{
	Mat recordedDst;

	tlx = v[0]; tly = v[1];
	brx = v[2]; bry = v[3];
	iftx = v[4]; ifty = v[5];
	ifjx = v[6]; ifjy = v[7];
	tfjx = v[8]; tfjy = v[9];
	btx = v[10]; bty = v[11];
	bjx = v[12]; bjy = v[13];

	src.copyTo(recordedDst);
	cv::circle(recordedDst, cv::Point(tlx*FRAME_WIDTH, tly*FRAME_HEIGHT), 3, Scalar(0, 0, 255), 3);
	cv::circle(recordedDst, cv::Point(brx*FRAME_WIDTH, bry*FRAME_HEIGHT), 3, Scalar(0, 0, 255), 3);
	cv::circle(recordedDst, cv::Point(iftx*FRAME_WIDTH, ifty*FRAME_HEIGHT), 3, Scalar(0, 255, 255), 3);
	cv::circle(recordedDst, cv::Point(ifjx*FRAME_WIDTH, ifjy*FRAME_HEIGHT), 3, Scalar(0, 255, 255), 3);
	cv::circle(recordedDst, cv::Point(tfjx*FRAME_WIDTH, tfjy*FRAME_HEIGHT), 3, Scalar(255, 0, 0), 3);
	cv::circle(recordedDst, cv::Point(btx*FRAME_WIDTH, bty*FRAME_HEIGHT), 3, Scalar(0, 255, 0), 3);
	cv::circle(recordedDst, cv::Point(bjx*FRAME_WIDTH, bjy*FRAME_HEIGHT), 3, Scalar(0, 255, 0), 3);
	cv::Rect rect(cv::Point(tlx*FRAME_WIDTH, tly*FRAME_HEIGHT), cv::Point(brx*FRAME_WIDTH, bry*FRAME_HEIGHT));
	cv::rectangle(recordedDst, rect, Scalar(0, 0, 0));

	return recordedDst;
}

#include "recorder.h"
//*****************
// ��Ƶ���ָ�Ϊ֡ͼ
//*****************
void Processor::splitVideo(QString videoPath, QString dir, QString baseName)
{
	VideoCapture cap;
	cv::String str = videoPath.toStdString().data();

	Recorder::writeLog(videoPath);

	cap.open(str);
	if (!cap.isOpened()){
		QMessageBox::information(NULL, "Path", "Didn't Select a Correct Video File.");
		return;
	}


	int totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < totalFrameNumber; i++)
	{
		Mat frame;
		cap.read(frame);
		cv::resize(frame, frame, cv::Size(640, 480));

		QString writeImgPath = dir + "/" + baseName + "_color_" + QString::number(i) + ".png";
		imwrite(writeImgPath.toStdString().data(), frame);
		imshow("show", frame);
		waitKey(10);
	}
	destroyAllWindows();
}