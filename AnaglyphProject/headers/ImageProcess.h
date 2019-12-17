#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

///<summary>
///An abstract class that is a base for lightingChange and Filter classes
///It inherits from Mat_<uchar> class itself
///</summary>
class ImageProcess :
	public cv::Mat_<uchar>
{
public:
	ImageProcess();
	ImageProcess(cv::Mat_<uchar> image, cv::Rect roi);
	~ImageProcess();
	static cv::Rect chosenROI;
	static unsigned int value;

	cv::Mat_<uchar> getOriginalImage();
	///<summary>
	///Operator + is overloaded in this class.
	///A matrix that is inside an object of children classes is put over the main image.
	///</summary>
	cv::Mat_ <cv::Vec3b> operator+ (const cv::Mat_<cv::Vec3b>& mainImage);

protected:
	// Pure virtual method.
	virtual void process() = 0;

	cv::Mat_<uchar> originalImage;
	cv::Rect ROI;
	std::string processName;

private:
	void setOriginalImage(cv::Mat_<uchar> image);
	void setROI(cv::Rect roi);
};

