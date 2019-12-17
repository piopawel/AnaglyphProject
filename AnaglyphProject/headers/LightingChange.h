#pragma once
#include "ImageProcess.h"

///<summary>
///Function inheriting from ImageProcess.
///Responsible for enlightening / darkening chosen image.
///</summary>
class LightingChange :
	public ImageProcess
{
public:
	LightingChange();
	LightingChange(cv::Mat_<uchar> image, cv::Rect roi, int value);
	~LightingChange();
private:
	void process();
	void setValue(int value);


	int value;
};

