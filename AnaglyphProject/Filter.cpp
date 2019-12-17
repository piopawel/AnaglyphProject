#include "pch.h"
#include "./headers/Filter.h"


Filter::Filter() 
{
}

Filter::Filter(cv::Mat_<uchar> image, cv::Rect roi, cv::Size size, int type) : ImageProcess(image, roi)
{
	setSize(size);
	setFilteringType(type);
	process();
}


Filter::~Filter()
{
}
/*
Main function of the class. Based on input type of filter it performs filtering.
There is an exception thrown if different input is forced.
*/
void Filter::process()
{
	switch (this->filteringType) {
		case 1: {
			cv::blur(this->originalImage, *this, this->size);
			break;
		}
		case 2: {
			cv::GaussianBlur(this->originalImage, *this, this->size, 0, 0);
			break;
		}
		case 3: {
			cv::medianBlur(this->originalImage, *this, this->size.width);
			break;
		} default: {
			throw std::invalid_argument("Incorrect argument. Possible values 1, 2, 3.");
			break;
		}
	}
}

void Filter::setSize(cv::Size size)
{
	this->size = size;
}

void Filter::setFilteringType(int type)
{
	this->filteringType = type;
}


