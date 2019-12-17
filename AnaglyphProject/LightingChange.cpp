#include "stdafx.h"
#include "LightingChange.h"


LightingChange::LightingChange()
{
}

LightingChange::LightingChange(cv::Mat_<uchar> image, cv::Rect roi, int value) : ImageProcess(image, roi)
{
	this->setValue(value);
	this->process();
}


LightingChange::~LightingChange()
{
}

/*
Main function of the class.
Adds a value to each pixel of chosen image.
If the output value is bigger than 255, it is set to 255.
If the output value is less than 0, it is set to 0.
*/
void LightingChange::process()
{
	int width = this->rows;
	int height = this->cols;
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			if (originalImage.at<uchar>(j, i) + this->value > 255)
				this->at<uchar>(j, i) = 255;
			else if (originalImage.at<uchar>(j, i) + this->value < 0)
				this->at<uchar>(j, i) = 0;
			else
				this->at<uchar>(j, i) = originalImage.at<uchar>(j, i) + this->value;
		}		
	}
}

void LightingChange::setValue(int value)
{
	this->value = value;
}


