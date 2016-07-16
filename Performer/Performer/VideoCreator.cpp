#include "VideoCreator.h"


VideoCreator::VideoCreator()
{
	fps = DEFAULT_FPS;					//֡��
	imgWidth = IMAGE_DEFAULT_WIDTH;		//ͼƬ��ʽͳһ
	imgHeight = IMAGE_DEFAULT_HEIGHT;	//ͼƬ��ʽͳһ
}


VideoCreator::~VideoCreator()
{
}


/*
�˴���Ҫ�Ż���Ӧ�ö���һ���ļ���֮��ֱ�Ӱ���˳��Ͷ��룬��Ӧ��Ҫ��̶����ļ�����ʽ
Ŀǰ������Ŀ���ļ��б�������������ִ�0��ʼ��pngͼƬ��
*/
int VideoCreator::writeVideo(string targetDir)
{
	Size frameSize(static_cast<int>(imgWidth), static_cast<int>(imgHeight));
	VideoWriter videoWriter("FrameDifference.avi", CV_FOURCC('D', 'I', 'V', 'X'), fps, frameSize, 1);
	if (!videoWriter.isOpened())
	{
		qDebug() << "Fail to Initialize Video Writer, Pls Check Frame Size or Decoder��" << endl;
		return -1;
	}

	int i = 0;
	while (true)
	{
		string targetPath = targetDir + "/" + i + ".png";
		Mat img = imread(targetPath);
		if (img.empty())
		{
			qDebug()<< "Read Image Ends" << endl;
			break;
		}
		videoWriter.write(img);
		qDebug() << "д���" << i << "��֡�ɹ�" << endl;
		i++;
	}
}

