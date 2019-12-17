#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>


class ColorBoundingRectangle :
	public cv::Rect

{
public:
	// x,y used to be unsigned ints, but I misunderstood the idea behind unsigned
	// changed back to int, so the rectangles behave properly (They could move out of left / top boundaries
	// because the values after getting below 0 got to maximum value of unsigned int
	struct Translation
	{
		int x;
		int y;
	};
	/*
	Constructors, with default 0,0 translation. There is a possibliity to choose another rectangle as a base,
	or to input dimensions directly.
	There is also a possibility to use constructors with string or Scalar as an input for color.
	Accepted values are only red / cyan / 255,255,0 / 0, 0, 255
	*/
	ColorBoundingRectangle();
	ColorBoundingRectangle(int x, int y, int width, int height, std::string color, unsigned int translationX = 0, unsigned int translationY = 0);
	ColorBoundingRectangle(cv::Rect sourceRectangle, std::string color, unsigned int translationX = 0, unsigned int translationY = 0);
	ColorBoundingRectangle(int x, int y, int width, int height, cv::Scalar color, unsigned int translationX = 0, unsigned int translationY = 0);
	ColorBoundingRectangle(cv::Rect sourceRectangle, cv::Scalar color, unsigned int translationX = 0, unsigned int translationY = 0);
	~ColorBoundingRectangle();

	const cv::Scalar getColor() const;
	const Translation getTranslation() const;
	void setTranslation(int translationX = 0, int translationY = 0);

	int getMaxWidth();
	int getMaxHeight();
	int getStartX();
	int getStartY();
	int getMinTranslationX();
	int getMinTranslationY();


private:
	void setColor(std::string color);
	void setColor(cv::Scalar color);
	///<summary>
	///Function used at the construction of the color rectangle to set up maximum size it can reach.
	///</summary>
	void setLimits();

	Translation translation;
	cv::Scalar color;
	int maxWidth;
	int maxHeight;
	int startX;
	int startY;
	int minTranslationX;
	int minTranslationY;
};