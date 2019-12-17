#include "pch.h"
#include "./headers/ColorBoundingRectangle.h"
#include <iostream>
#include <stdexcept>

ColorBoundingRectangle::ColorBoundingRectangle()
{
}


ColorBoundingRectangle::~ColorBoundingRectangle()
{
}

// Default values for translation 0, 0
ColorBoundingRectangle::ColorBoundingRectangle(int x, int y, int width, int height, std::string color, unsigned int translationX, unsigned int translationY) : cv::Rect(x, y, width, height)
{
	setColor(color);
	setTranslation(translationX, translationY);
	setLimits();
}

ColorBoundingRectangle::ColorBoundingRectangle(cv::Rect sourceRectangle, std::string color, unsigned int translationX, unsigned int translationY) : cv::Rect(sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height)
{
	setColor(color);
	setTranslation(translationX, translationY);
	setLimits();
}

ColorBoundingRectangle::ColorBoundingRectangle(int x, int y, int width, int height, cv::Scalar color, unsigned int translationX, unsigned int translationY) :cv::Rect(x, y, width, height)
{
	setColor(color);
	setTranslation(translationX, translationY);
	setLimits();
}

ColorBoundingRectangle::ColorBoundingRectangle(cv::Rect sourceRectangle, cv::Scalar color, unsigned int translationX, unsigned int translationY) : cv::Rect(sourceRectangle.x, sourceRectangle.y, sourceRectangle.width, sourceRectangle.height)
{
	setColor(color);
	setTranslation(translationX, translationY);
	setLimits();
}


const cv::Scalar ColorBoundingRectangle::getColor() const{
	return this->color;
}

///<summary>
///Set color function with string argument. Acceptable values: "red", "cyan".
///Used in the constructor
///</summary>
void ColorBoundingRectangle::setColor(std::string color) {
	if (color == "red")
		this->color = cv::Scalar(0, 0, 255);
	else if (color == "cyan")
		this->color = cv::Scalar(255, 255, 0);
	else {
		throw std::invalid_argument("The value is not \"red\" nor \"cyan\".");
	}
}

///<summary>
///Set color function with cv::Scalar argument. Acceptable values: cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0).
///Used in the constructor
///</summary>
void ColorBoundingRectangle::setColor(cv::Scalar color)
{
	if (color == cv::Scalar(0, 0, 255) || color == cv::Scalar(255, 255, 0))
		this->color = color;
	else
		throw std::invalid_argument("The value is not red [0, 0, 255] nor cyan [255, 255, 0]");
}
const ColorBoundingRectangle::Translation ColorBoundingRectangle::getTranslation() const{
	return this->translation;
}
int ColorBoundingRectangle::getMaxWidth()
{
	return this->maxWidth;
}
int ColorBoundingRectangle::getMaxHeight()
{
	return this->maxHeight;
}
int ColorBoundingRectangle::getStartX()
{
	return this->startX;
}
int ColorBoundingRectangle::getStartY()
{
	return this->startY;
}
int ColorBoundingRectangle::getMinTranslationX()
{
	return this->minTranslationX;
}
int ColorBoundingRectangle::getMinTranslationY()
{
	return this->minTranslationY;
}

void ColorBoundingRectangle::setTranslation(int translationX, int translationY) {
	this->translation.x = translationX;
	this->translation.y = translationY;
}

void ColorBoundingRectangle::setLimits()
{
	this->maxHeight = this->height;
	this->maxWidth = this->width;
	this->startX = this->x + this->translation.x;
	this->startY = this->y + this->translation.y;
	this->minTranslationX = this->translation.x;
	this->minTranslationY = this->translation.y;
}
