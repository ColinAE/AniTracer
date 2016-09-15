/*
 * RGB.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef RGB_H_
#define RGB_H_

//Unscaled RGB color object used for the intermediate stages of color calculation
class RGB{
protected:
	double r, g, b;
public:
	RGB();
	RGB(double, double, double);
	double red() const { return r; }
	double green() const { return g; }
	double blue() const { return b; }
	RGB operator+(const RGB &);
	RGB operator*(const double &);
};

//Red-Green-Blue color object defined by three integers 0-255 range
//TODO: r g b variables need error checking so that they stay in-range
class sRGB : public RGB{
private:
	bool inrange(int);
public:
	sRGB(int, int, int);
};

#endif
