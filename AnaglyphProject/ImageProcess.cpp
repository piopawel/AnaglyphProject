#include "pch.h"
#include "./headers/ImageProcess.h"


cv::Rect ImageProcess::chosenROI;
unsigned int ImageProcess::value;

ImageProcess::ImageProcess()
{
}

ImageProcess::ImageProcess(cv::Mat_<uchar> image, cv::Rect roi) : cv::Mat_<uchar>(image)
{
	setOriginalImage(image);
	setROI(roi);
}


ImageProcess::~ImageProcess()
{
}

cv::Mat_<uchar> ImageProcess::getOriginalImage()
{
	return cv::Mat_<uchar>();
}

/*
Function overloads the + operator. On a main matrix there is overlayed matrix that is inside this object.
(Of a child class, because ImageProcess class is an abstract class)
The main image is overwriten k times, because this class derives from <uchar> type matrix,
and main image is a Vec3b matrix.
*/
cv::Mat_<cv::Vec3b> ImageProcess::operator+(const cv::Mat_<cv::Vec3b> & mainImage) 
{
	int startX = this->ROI.x;
	int startY = this->ROI.y;
	int endX = startX + this->ROI.width;
	int endY = startY + this->ROI.height;
	cv::Mat_<cv::Vec3b> displayedImage;
	mainImage.copyTo(displayedImage);
	for (int i = startX; i < endX; i++) {
		for (int j = startY; j < endY; j++) {
			for(int k = 0; k < displayedImage.channels(); k++)
			displayedImage(j, i)[k] = this->at<uchar>(j - startY, i - startX);
		}
	}
	return displayedImage;
}

void ImageProcess::setOriginalImage(cv::Mat_<uchar> image)
{
	this->originalImage = image;
}

void ImageProcess::setROI(cv::Rect roi)
{
	this->ROI = roi;
}







