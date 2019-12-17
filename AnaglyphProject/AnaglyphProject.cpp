// AnaglyphProject.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "./headers/ImageHandling.h"


void startingFunction(int imageNumber);
void mouseEvent(int event, int flags, int x, int y, void* param);

int main()
{
	// Main initializes the first appearance of windows.
	// Most of the processes hapen in the imageHandling object.
	startingFunction(0);
	return 0;
}

void startingFunction(int imageNumber)
{
	ImageHandling::imageNumber = ImageHandling::imageNumber = imageNumber;
	ImageHandling imageHandling = ImageHandling();
	imageHandling.displayImages();
	cv::setMouseCallback("firstWindow", mouseEvent, &imageHandling);
	cv::waitKey(0);;
}

/*
All events that are occuring are in this function. The events are invoked by mouse event,
then the type of keyboard key pressed defines the output.
Possibilities:
LeftMouseKey - this group is about processing the image
1 - choose left image to process
2 - choose right image to process
+ - increase the value / size of kernel
- - decrease the value / size of kernel
l - perform change of lightness of image
b - perform blur filtering
g - perform gaussian filtering
m - perform median filtering

RightMouseKey - change image and manipulate color rectangles
1-6 - change of image
(The signs are logical in international keyboard - in french it may be unconfortable)
q - increase size of color rectangles
e - decrease size of color rectangles
w - move up of color rectangles
s - move down of color rectangles
a - move left of color rectangles
d - move right of color rectangles

*/
void mouseEvent(int event, int flags, int x, int y, void* param)
{
	ImageHandling* ptrImageHandling = (ImageHandling*)param;
	cv::Mat_ <cv::Vec3b> displayedImage = (ptrImageHandling->getDisplayedImage());
	cv::Mat_ <cv::Vec3b> anaglyphImage = (ptrImageHandling->getAnaglyphImage());
	cv::Mat_ <uchar> grayImage = (ptrImageHandling->getGrayImage());
	cv::Rect roiLeft = (ptrImageHandling->getROILeft());
	cv::Rect roiRight = (ptrImageHandling->getROIRight());
	ColorBoundingRectangle redRectangle = ptrImageHandling->getRedRectangle();
	ColorBoundingRectangle cyanRectangle = ptrImageHandling->getCyanRectangle();
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN: {
		while (cv::waitKey(0) != 27) {
			int key = cv::waitKey(0);
			switch (key) {
			case '1':
			{
				// The rectangle indicates which image will be processed.
				cv::Rect rectangle1(15, 255, 300, 35);
				cv::rectangle(displayedImage, rectangle1, cv::Scalar(60, 170, 0), 2);
				cv::Rect rectangle2(15, 295, 300, 35);
				cv::rectangle(displayedImage, rectangle2, cv::Scalar(0, 0, 255), 2);
				ImageProcess::chosenROI = roiLeft;
				ptrImageHandling->displayImage("firstWindow", displayedImage, 800, 300);
				break;
			}
			case '2':
			{
				cv::Rect rectangle1(15, 255, 300, 35);
				cv::rectangle(displayedImage, rectangle1, cv::Scalar(0, 0, 255), 2);
				cv::Rect rectangle2(15, 295, 300, 35);
				cv::rectangle(displayedImage, rectangle2, cv::Scalar(60, 170, 0), 2);
				ImageProcess::chosenROI = roiRight;
				ptrImageHandling->displayImage("firstWindow", displayedImage, 800, 300);
				break;
			}
			case '+':
			{
				// This rectangle erases the previos number
				cv::Rect rectangle(285, 333, 80, 40);
				cv::rectangle(displayedImage, rectangle, cv::Scalar(0, 0, 0), CV_FILLED);
				// Limit
				if (ImageProcess::value < 20)
					ImageProcess::value++;
				cv::putText(displayedImage, std::to_string(ImageProcess::value), cv::Point(300, 360), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);
				ptrImageHandling->displayImage("firstWindow", displayedImage, 800, 300);
				break;
			}
			case '-':
			{
				cv::Rect rectangle(285, 333, 80, 40);
				cv::rectangle(displayedImage, rectangle, cv::Scalar(0, 0, 0), CV_FILLED);
				if (ImageProcess::value > 0)
					ImageProcess::value--;
				cv::putText(displayedImage, std::to_string(ImageProcess::value), cv::Point(300, 360), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);
				ptrImageHandling->displayImage("firstWindow", displayedImage, 800, 300);
				break;
			}
			case 'l':
			{
				//In order to catch possible exceptions while performing the process there is a try/catch
				try
				{
					ptrImageHandling->lightenImage(ImageProcess::chosenROI, ImageProcess::value - 10);
					ptrImageHandling->displayImages();
				}
				catch (const std::exception&)
				{
					printf("There was an exception.");
				}
				break;
			}
			case 'b':
			{
				try
				{
					ptrImageHandling->filterImage(ImageProcess::chosenROI, cv::Size(ImageProcess::value, ImageProcess::value), Filter::BLUR);
					ptrImageHandling->displayImages();
				}
				catch (const std::exception&)
				{
					printf("There was an exception.");
				}
				break;
			}
			case 'g':
			{
				try
				{
					ptrImageHandling->filterImage(ImageProcess::chosenROI, cv::Size(ImageProcess::value, ImageProcess::value), Filter::GAUSSIAN);
					ptrImageHandling->displayImages();
				}
				catch (const std::exception&)
				{
					printf("There was an exception.");
				}
				break;
			}
			case 'm':
			{
				try
				{
					ptrImageHandling->filterImage(ImageProcess::chosenROI, cv::Size(ImageProcess::value, ImageProcess::value), Filter::MEDIAN);
					ptrImageHandling->displayImages();

				}
				catch (const std::exception&)
				{
					printf("There was an exception.");
				}
				break;
			}
			}
		}
	}
	case CV_EVENT_RBUTTONDOWN: {
		while (cv::waitKey(0) != 27) {
			int key = cv::waitKey(0);
			switch (key) {
			case '1': {
				// Performs the same steps as in main()
				cv::destroyAllWindows();
				startingFunction(0);
				break;
			}
			case '2': {
				cv::destroyAllWindows();
				startingFunction(1);
				break;
			}
			case '3': {
				cv::destroyAllWindows();
				startingFunction(2);
				break;
			}
			case '4': {
				cv::destroyAllWindows();
				startingFunction(3);
				break;
			}
			case '5': {
				cv::destroyAllWindows();
				startingFunction(4);
				break;
			}
			case '6': {
				cv::destroyAllWindows();
				startingFunction(5);
				break;
			}
					// increase size of color rectangles
			case 'q': {
				ptrImageHandling->changeColorRectangleSize(5);
				ptrImageHandling->displayImages();
				break;
			}
					// decrease size of color rectangles
			case 'e': {
				ptrImageHandling->changeColorRectangleSize(-5);
				ptrImageHandling->displayImages();
				break;
			}
					//move color rectangles up
			case 'w': {
				ptrImageHandling->changeColorRectangleTranslation(0, -5);
				ptrImageHandling->displayImages();
				break;
			}
					//move color rectangles down
			case 's': {
				ptrImageHandling->changeColorRectangleTranslation(0, 5);
				ptrImageHandling->displayImages();
				break;
			}
					//move color rectangles left
			case 'a': {
				ptrImageHandling->changeColorRectangleTranslation(-5, 0);
				ptrImageHandling->displayImages();
				break;
			}
					//move color rectangles right
			case 'd': {
				ptrImageHandling->changeColorRectangleTranslation(5, 0);
				ptrImageHandling->displayImages();
				break;
			}
			}
		}
	}
	}
}


