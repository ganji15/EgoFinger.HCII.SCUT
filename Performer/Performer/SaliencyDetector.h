#pragma once
#include "PathManager.h"

class SaliencyDetector
{
public:
	SaliencyDetector();
	~SaliencyDetector();

public:
	void displaySaliencySequence(QString diffImgDir);

private:
	Mat frameDiffImg;			// ֡��ͼ ��Ϊ ����
	Mat imgGray;				// �Ҷ�ͼ
	Mat hist;					// �Ҷ�ֱ��ͼ
	Mat colorDiffMap, distMap;	// �Ҷȱ�֡���
	int order;					// ֡��Ľ���

	void initMaps();
	void initHistogram();

	void setColorDiffMap();
	void setDistMap();

	void showHistogram();
	Mat  getDistMap();
	
};
