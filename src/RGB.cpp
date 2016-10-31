/*
 * RGB.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "RGB.h"
#include <iostream>

//RGB definitions

int colors::normalize(double oldval, double min, double max) {
	return ((oldval - min) / (max - min)) * 255;
}

std::vector<sRGB> colors::scale(const std::vector<RGB> &urgbs) {

	double min = std::numeric_limits<double>::max();
	double max = std::numeric_limits<double>::min();

	std::for_each(urgbs.begin(), urgbs.end(), [&](const RGB &urgb) {
		if (urgb.red() < min) {
			min = urgb.red();
		} else if (urgb.red() > max) {
			max = urgb.red();
		}

		if (urgb.green() < min) {
			min = urgb.green();
		} else if (urgb.green() > max) {
			max = urgb.green();
		}

		if (urgb.blue() < min) {
			min = urgb.blue();
		} else if (urgb.blue() > max) {
			max = urgb.blue();
		}
	});
	std::cout << "min: " << min << std::endl;
	std::cout << "max: " << max << std::endl;
	std::vector<sRGB> scaled;
	std::for_each(urgbs.begin(), urgbs.end(), [&](const RGB &urgb) {
		scaled.push_back(sRGB(colors::normalize(urgb.red(), min, max),
						colors::normalize(urgb.green(), min, max),
						colors::normalize(urgb.blue(), min, max)));
	});
	return scaled;
}

RGB::RGB() {
	r = 0;
	g = 0;
	b = 0;
}

RGB::RGB(double red, double green, double blue) {
	r = red;
	g = green;
	b = blue;
}

RGB RGB::operator+(const RGB &other) const {
	double red = r + other.red();
	double green = g + other.green();
	double blue = b + other.blue();
	return RGB(red, green, blue);
}

RGB RGB::operator*(const double &scalar) const {
	return RGB(scalar * r, scalar * g, scalar * b);
}

//sRGB definitions

sRGB::sRGB(const RGB &urgb) :
		RGB(-1, -1, -1) {

	if (!inrange(urgb.red()) || !inrange(urgb.green()) || !inrange(urgb.blue())) {
		//TODO: throw error
	}
	r = urgb.red();
	g = urgb.green();
	b = urgb.blue();
}

bool sRGB::inrange(int rgb) {
	if (rgb >= 0 && rgb <= 255) {
		return true;
	} else {
		return false;
	}
}
