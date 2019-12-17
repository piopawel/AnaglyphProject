#include "pch.h"
#include "./headers/ImageHandling.h"


int ImageHandling::imageNumber;

// Constructor preparing all the necessary things to run the program
ImageHandling::ImageHandling()
{
	this->findImageNames(".\\photos");
	this->createMainImage(this->imageNames.at(imageNumber));
	this->createGrayImage();
	this->leftBorder = 500;
	this->getROI(this->grayImage);
	ImageProcess::value = 0;
	this->createDisplayedImage();
	this->createAnaglyph(this->mainImage(this->redRectangle), this->mainImage(this->cyanRectangle));
}


ImageHandling::~ImageHandling()
{
}


cv::Mat_<cv::Vec3b> ImageHandling::getDisplayedImage()
{
	return this->displayedImage;
}

cv::Mat_<cv::Vec3b> ImageHandling::getAnaglyphImage()
{
	return this->anaglyphImage;
}

cv::Mat_<cv::Vec3b> ImageHandling::getMainImage()
{
	return this->mainImage;
}

cv::Mat_<uchar> ImageHandling::getGrayImage()
{
	return this->grayImage;
}

cv::Rect ImageHandling::getROILeft()
{
	return this->ROILeft;
}

cv::Rect ImageHandling::getROIRight()
{
	return this->ROIRight;
}

ColorBoundingRectangle ImageHandling::getRedRectangle()
{
	return this->redRectangle;
}

ColorBoundingRectangle ImageHandling::getCyanRectangle()
{
	return this->cyanRectangle;
}

/*
First the function copies the chosen part of Gray image to a new image. 
Then lightingChange object performs the proccess action and holds the new image in itself.
The output is overlayed on the main image (operator + overloaded)
Other images are created from the main image.
*/
void ImageHandling::lightenImage(cv::Rect ROI, int value)
{
	cv::Mat_ <uchar> copyOfGrayImage;
	grayImage(ROI).copyTo(copyOfGrayImage);
	LightingChange lightingChange(copyOfGrayImage, ROI, value);
	this->mainImage = lightingChange + this->mainImage;
	this->createGrayImage();
	this->createDisplayedImage();
	this->createAnaglyph(this->mainImage(this->redRectangle), this->mainImage(this->cyanRectangle));
}
/*
First the function copies the chosen part of Gray image to a new image.
Then filteredImage object performs the proccess action and holds the new image in itself.
The output is overlayed on the main image (operator + overloaded)
Other images are created from the main image.
*/
void ImageHandling::filterImage(cv::Rect ROI, cv::Size size, int type)
{
	cv::Mat_ <uchar> copyOfGrayImage;
	grayImage(ROI).copyTo(copyOfGrayImage);
	Filter filteredImage(copyOfGrayImage, ROI, size, type);
	this->mainImage = filteredImage + this->mainImage;
	this->createGrayImage();
	this->createDisplayedImage();
	this->createAnaglyph(this->mainImage(this->redRectangle), this->mainImage(this->cyanRectangle));
}

/*
The function checks whether it is possible to resize the color rectangles.
Checks if the change would not make them too small / too big.
If not, the red and cyan rectangles are resized.
displayed Image and anaglyph are recreated
*/
void ImageHandling::changeColorRectangleSize(int change)
{
	//theoritically the check should be for both rectangles, but their only change is in color. Both move in the same way
	try
	{
		ColorBoundingRectangle::Translation currentTranslation = this->redRectangle.getTranslation();
		if (this->redRectangle.width + change > 0 && this->redRectangle.height + change >0)
			if (this->redRectangle.width + change + currentTranslation.x < this->redRectangle.getMaxWidth() && this->redRectangle.height + change + currentTranslation.y < this->redRectangle.getMaxHeight()) {
				this->redRectangle.width = this->redRectangle.width + change;
				this->redRectangle.height = this->redRectangle.height + change;
				this->cyanRectangle.width = this->cyanRectangle.width + change;
				this->cyanRectangle.height = this->cyanRectangle.height + change;
			}
		createDisplayedImage();
		currentTranslation = this->redRectangle.getTranslation();
		cv::Rect newRedRectangle(redRectangle.x + currentTranslation.x, redRectangle.y + currentTranslation.y, redRectangle.width, redRectangle.height);
		cv::Rect newCyanRectangle(cyanRectangle.x + currentTranslation.x, cyanRectangle.y + currentTranslation.y, cyanRectangle.width, cyanRectangle.height);
		createAnaglyph(this->mainImage(newRedRectangle), this->mainImage(newCyanRectangle));
	}
	catch (const std::exception&)
	{
		printf("Rectangle out of boundaries!");
	}
	
}

/*
The function checks whether it is possible to move the color rectangles.
Checks if the change would not move them out of their primary boundaries.
If not, the red and cyan rectangles are moved.
displayed Image and anaglyph are recreated. 
As the redRectangle / cyanRectangle does not explicitly contain the translation,
it must be added to a temporary rectangle to see the effect immediately.
*/
void ImageHandling::changeColorRectangleTranslation(int translationX, int translationY)
{
	try
	{
		ColorBoundingRectangle::Translation currentTranslation = this->redRectangle.getTranslation();
		if (translationX + currentTranslation.x >= this->redRectangle.getMinTranslationX() && translationY + currentTranslation.y >= this->redRectangle.getMinTranslationY())
			if (translationX + currentTranslation.x + this->redRectangle.getStartX() + this->redRectangle.width < this->redRectangle.getMaxWidth() && translationY + currentTranslation.y + this->redRectangle.getStartY() + this->redRectangle.height - this->redRectangle.getStartY()< this->redRectangle.getMaxHeight()) {
				this->redRectangle.setTranslation(currentTranslation.x + translationX, currentTranslation.y + translationY);
				this->cyanRectangle.setTranslation(currentTranslation.x + translationX, currentTranslation.y + translationY);
			}
		createDisplayedImage();
		currentTranslation = this->redRectangle.getTranslation();
		cv::Rect newRedRectangle(redRectangle.x + currentTranslation.x, redRectangle.y + currentTranslation.y, redRectangle.width, redRectangle.height);
		cv::Rect newCyanRectangle(cyanRectangle.x + currentTranslation.x, cyanRectangle.y + currentTranslation.y, cyanRectangle.width, cyanRectangle.height);
		createAnaglyph(this->mainImage(newRedRectangle), this->mainImage(newCyanRectangle));
	}
	catch (const std::exception&)
	{
		printf("Rectangle out of boundaries!");
	}
	
}

template<class T>
inline void ImageHandling::displayImage(string windowName, cv::Mat_<T> image, int width, int height)
{
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, width, height);
	imshow(windowName, image);
}

void ImageHandling::displayImages()
{
	this->displayImage("firstWindow", this->displayedImage, 800, 300);
	this->displayImage("secondWindow", this->anaglyphImage, 400, 400);
}

// Function goes through a windows directory until it does not find any more jpg files.
void ImageHandling::findImageNames(const std::string & name)
{
	vector <string> imageNames;
	std::string pattern(name);
	pattern.append("\\*.jpg");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	// If there is a problem with patter in visual studio set properties:
	// Project -> Properties -> General -> Character Set option to Use Multi-Byte Character Set
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = data.cFileName;
			imageNames.push_back(name+ "\\" + filename);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
	this->imageNames = imageNames;
}

void ImageHandling::createMainImage(std::string imagePath)
{
	cv::Mat_<cv::Vec3b> mat = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);
	cv::resize(mat, this->mainImage, cv::Size(mat.cols / 4, mat.rows / 4));
}

void ImageHandling::createGrayImage()
{
	cv::cvtColor(this->mainImage, this ->grayImage, cv::COLOR_RGB2GRAY);
}
/*
Function is used to detect primary ROIs. 
Firstly it selects a ROI that does not contain top and bottom margins that all the images have.
Than performs a medianBlur on the image, to get rid of noise.
Then performs binarisation on a still threshold.
Uses findEdgeOfImage to detect the ends of ROIs from the middle part of the image.
Sets up ROI objects with given sizes.
*/
void ImageHandling::getROI(cv::Mat_<uchar> grayImage)
{
	cv::Mat_ <uchar> grayImageForROIDetection;
	cv::Mat_ <uchar> binaryImage;
	int yBasicTranslation = grayImage.rows / 25;
	cv::Rect ROI(0, yBasicTranslation, grayImage.cols, grayImage.rows / 20 * 18.2);

	grayImage(ROI).copyTo(grayImageForROIDetection);
	cv::medianBlur(grayImageForROIDetection, grayImageForROIDetection, 15);
	cv::threshold(grayImageForROIDetection, binaryImage, 40, 255, cv::THRESH_BINARY);

	int xCoordLeft = ImageHandling::findEdgeOfImage("left", binaryImage);
	int xCoordRight = ImageHandling::findEdgeOfImage("right", binaryImage);
	int roiWidth = ((xCoordLeft + grayImage.cols - xCoordRight) / 2 <  grayImage.cols - xCoordRight) ? (xCoordLeft + grayImage.cols - xCoordRight) / 2 : grayImage.cols - xCoordRight;
	int roiHeight = (grayImage.rows - 2 * yBasicTranslation);
	this->ROILeft = cv::Rect(0, yBasicTranslation, roiWidth, roiHeight);
	this->ROIRight = cv::Rect(xCoordRight, yBasicTranslation, roiWidth, roiHeight);
	this->redRectangle = ColorBoundingRectangle(ROILeft, "red"); //this->leftBorder
	this->cyanRectangle = ColorBoundingRectangle(ROIRight, "cyan");
}
/*
From the middle of an image that is blackthe function goes either left or right.
Whenever it sees a big change of white pixels it stops and sets it as the end / beginning of image.
It will stop also after 250 pixels if it did not find any big change.
It does not work perfectly - two images have noise in top / bottom part of the middle part, 
so the difference is found immediately. 
In one image the difference is not found, and the maximum 250 pixels condition is triggered.
*/
int ImageHandling::findEdgeOfImage(string type, cv::Mat_<uchar> image)
{
	int xCoord = image.cols / 2;
	int counterBuffer = 0;
	while (1) {
		int counter = 0;
		int previousCounter = counterBuffer;

		for (int i = 0; i < image.rows; i++) {
			if (image(i, xCoord) == 255)
				counter++;
		}
		counterBuffer = counter;
		if (counter - previousCounter > image.rows / 30 || counter > 250)
			break;
		else
			if (type == "left")
				xCoord--;
			else
				xCoord++;
	}
	return xCoord;
}
/*
Function prepares an image that will be displayed.
Moves the red rectangles towords right, so they still overlay the ROI.
Adds a black field on the left side, to display the informations for the user.
On the black field writes instructions
*/
void ImageHandling::createDisplayedImage()
{
	ColorBoundingRectangle::Translation translation= redRectangle.getTranslation();
	cv::Rect newRedRectangle(redRectangle.x + translation.x + this->leftBorder, redRectangle.y + translation.y, redRectangle.width, redRectangle.height);
	cv::Rect newCyanRectangle(cyanRectangle.x + translation.x + this->leftBorder, cyanRectangle.y + translation.y, cyanRectangle.width, cyanRectangle.height);
	cv::copyMakeBorder(this->mainImage, this->displayedImage, 0, 0, this->leftBorder, 0, cv::BORDER_CONSTANT);
	cv:: Mat_ <cv::Vec3b> mat = this->displayedImage;
	cv::rectangle(this->displayedImage, newRedRectangle, redRectangle.getColor(), 3);
	cv::rectangle(this->displayedImage, newCyanRectangle, cyanRectangle.getColor(), 3);
	int font = cv::FONT_HERSHEY_COMPLEX;
	int xStart = 25;
	int yInterval = 40;
	cv::putText(this->displayedImage, "Hold right mouse button &", cv::Point(xStart, yInterval), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "1-6 ->change image", cv::Point(xStart, yInterval*2), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "w/s/a/d - move rect", cv::Point(xStart, yInterval*3), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "q/e -> +/- size rect", cv::Point(xStart, yInterval*4), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "Hold left mouse button &", cv::Point(xStart, yInterval*6), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "1 - Left image", cv::Point(xStart, yInterval*7), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "2 - Right image", cv::Point(xStart, yInterval*8), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "Value + / -:", cv::Point(xStart, yInterval*9), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "l - Lighten Val<10 darken", cv::Point(xStart, yInterval*10), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "b - Blur", cv::Point(xStart, yInterval*11), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "g - Gaussian Blur", cv::Point(xStart, yInterval*12), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, "m - Median Filter", cv::Point(xStart, yInterval*13), font, 1, cv::Scalar(255, 255, 255), 2);
	cv::putText(this->displayedImage, std::to_string(ImageProcess::value), cv::Point(300, yInterval*9), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}
/*
Function creates an anaglyph out of two chosen ROIs.
Left image values are only left in red channel.
Right image values are left in green and blue channels.
In the end red and cyan images are equally added.
*/
void ImageHandling::createAnaglyph(cv::Mat_ <cv::Vec3b> leftImage, cv::Mat_ <cv::Vec3b> rightImage)
{
	cv::Mat_ <cv::Vec3b> redImage, cyanImage;
	leftImage.copyTo(redImage);
	rightImage.copyTo(cyanImage);
	setChannelToValue(redImage, 0, 0);
	setChannelToValue(redImage, 1, 0);
	setChannelToValue(cyanImage, 2, 0);
	cv::addWeighted(redImage, 0.5, cyanImage, 0.5, 0, this->anaglyphImage);
}

/*
Function uses the reshape function to set all values in image to a chosen value
*/
void ImageHandling::setChannelToValue(cv::Mat_ <cv::Vec3b> &mat, unsigned int channel, uchar value) {
	if (mat.isContinuous())
		mat.reshape(1, mat.rows*mat.cols).col(channel).setTo(cv::Scalar(value));
	else {
		for (int i = 0; i < mat.rows; i++)
			mat.row(i).reshape(1, mat.cols).col(channel).setTo(cv::Scalar(value));
	}
}


