#pragma once
#include "ImageProcess.h"
class Filter :
	public ImageProcess
{
public:
	Filter();
	Filter(cv::Mat_<uchar> image, cv::Rect roi, cv::Size size, int type);
	~Filter();
	
	//Three possibilities of filters.
	const static int BLUR = 1;
	const static int GAUSSIAN = 2;
	const static int MEDIAN = 3;
	
private:
	void process();
	void setSize(cv::Size size);
	void setFilteringType(int type);
	int filteringType;
	cv::Size size;
};

