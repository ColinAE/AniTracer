/*
 * RGB.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "RGB.h"

//RGB definitions
RGB::RGB(double red, double green, double blue){
	r = red;
	g = green;
	b = blue;
}

RGB RGB::operator+(const RGB &other){
	double red = r + other.red();
	double green = g + other.green();
	double blue = b + other.blue();
	return RGB(red, green, blue);
}

//sRGB definitions
sRGB::sRGB(int red, int green, int blue) :
	RGB(-1, -1, -1){
	if(!inrange(red) || !inrange(green) || !inrange(blue)){
		//TODO: throw error
	}
	r = red;
	g = green;
	b = blue;
}

bool sRGB::inrange(int rgb){
	if(rgb >= 0 && rgb <= 255){
		return true;
	} else {
		return false;
	}
}

