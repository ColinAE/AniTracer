/*
 * RGB.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef RGB_H_
#define RGB_H_

#include <algorithm>


// Unscaled RGB color object used for the intermediate stages of color calculation.
class RGB{
protected:
	double r, g, b;
public:
	RGB();
	RGB(double red, double green, double blue);
	double red() const { return r; }
	double green() const { return g; }
	double blue() const { return b; }
	RGB operator+(const RGB &other) const;
	RGB operator*(const double &scalar) const;
};

// Scaled RGB color object.
// TODO: r g b variables need error checking so that they stay in-range
class sRGB : public RGB{
private:

	// Helper that checks if a value is between 0-255.
	bool inrange(int rgb);

public:
	sRGB(double red, double green, double blue) :
		RGB(red, green, blue) {};
	sRGB(const RGB &urgbs);

};

namespace colors{
	int normalize(double oldval, double min, double max);
	std::vector<sRGB> scale(const std::vector<RGB> &urgbs);
};

#endif
