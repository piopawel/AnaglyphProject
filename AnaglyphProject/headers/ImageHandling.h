#pragma once
#include <string>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include "ColorBoundingRectangle.h"
#include "LightingChange.h"
#include "Filter.h"

using namespace std;

///<summary>
///Main class for image connected processes
///</summary>
class ImageHandling
{
public:
	ImageHandling();
	~ImageHandling();
	static const int LEFT_IMAGE = 0;
	static const int RIGHT_IMAGE = 1;
	// Image number to be chosen from the list of images
	static int imageNumber;

	//Group of getters
	cv::Mat_<cv::Vec3b> getDisplayedImage();
	cv::Mat_<cv::Vec3b> getAnaglyphImage();
	cv::Mat_<cv::Vec3b> getMainImage();
	cv::Mat_<uchar> getGrayImage();
	cv::Rect getROILeft();
	cv::Rect getROIRight();
	ColorBoundingRectangle getRedRectangle();
	ColorBoundingRectangle getCyanRectangle();

	//Group of different purposes functions
	///<summary>
	/// Function used to perform lightening / shadowing the chosen image (Left or Right)
	/// It is defined by ROI. Value definies the strength of function.
	///</summary>
	void lightenImage(cv::Rect ROI, int value);
	///<summary>
	///Function used to perform filtering the chosen image (Left or Right)
	/// It is defined by ROI. Value definies the strength of function. Type definies the type of Filter.
	/// Available 3 types - BLUR, GAUSSIAN, MEDIAN (or 0, 1, 2)
	///</summary>
	void filterImage(cv::Rect ROI, cv::Size size, int type);
	///<summary>
	///Function used to change the size of color rectangle. The change value is verified if is possible to use.
	///</summary>
	void changeColorRectangleSize( int change);
	///<summary>
	///Function used to change the position of color rectangle. The translation values are verified if are possible to use.
	///</summary>
	void changeColorRectangleTranslation(int translationX, int translationY);
	///<summary>
	///Function used to display images. It is a template, because there may be a necessity to display only gray image
	///or binary image that are not cv::Mat_<cv::Vec3b> types.
	///</summary>
	template <class T> void displayImage(string windowName, cv::Mat_ <T> image, int width, int height);
	///<summary>
	///Function displaying both first and second windows.
	///</summary>
	void displayImages();

private:
	///<summary>
	///Function going through a directory "name", and setting all *jpg files in that directory.
	///</summary>
	void findImageNames(const std::string& name);
	///<summary>
	///Function creating main Image, from indicated imagePath.
	///</summary>
	void createMainImage(std::string imagePath);
	///<summary>
	///Function creating gray Image, from indicated image. (preferably mainImage)
	///</summary>
	void createGrayImage();
	///<summary>
	///Function creating gray Image, from indicated image. (preferably mainImage)
	///</summary>
	///<summary>
	///Function used in constructor to define primary ROIs and color rectangles.
	///</summary>
	void getROI(cv::Mat_ <uchar> grayImage);
	///<summary>
	///Function used by getROI function. Defines edge points of ROIs.
	///</summary>
	int findEdgeOfImage(string type, cv::Mat_<uchar> image);
	///<summary>
	///Function creating an image that will be displayed in first window.
	///</summary>
	void createDisplayedImage();
	///<summary>
	///Function creating an anaglyph that will be displayed in second window.
	///</summary>
	void createAnaglyph(cv::Mat_ <cv::Vec3b> leftImage, cv::Mat_ <cv::Vec3b> rightImage);
	///<summary>
	///Function used by createAnaglyph function. Sets a chosen channel of RGB image to a chosen value.
	///</summary>
	void setChannelToValue(cv::Mat_<cv::Vec3b>& mat, unsigned int channel, uchar value);

	vector<string> imageNames;

	cv::Mat_<cv::Vec3b> mainImage;
	cv::Mat_<uchar> grayImage;
	cv::Mat_<cv::Vec3b> displayedImage;
	cv::Mat_<cv::Vec3b> anaglyphImage;

	cv::Rect ROILeft;
	cv::Rect ROIRight;

	ColorBoundingRectangle redRectangle;
	ColorBoundingRectangle cyanRectangle;

	// A value indicating how will be the displayedImage moved right in comparison to mainImage.
	int leftBorder;

};


