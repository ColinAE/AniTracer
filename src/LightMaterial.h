/*
 * LightMaterial.h
 *
 *  Created on: Sep 21, 2016
 *      Author: colin
 */

#ifndef LIGHTMATERIAL_H_
#define LIGHTMATERIAL_H_

#include "RGB.h"
#include "Singular.h"

class Light {
private:
	RGB rgb = RGB(-1, -1, -1);
	Point position = Point();
	bool notAmbient = true;
public:
	Light();
	Light(double red, double green, double blue, double x, double y, double z, bool notAmbient);
	RGB color() const { return rgb; }
	Point getPosition() const { return position; }
	bool hasPosition() const { return notAmbient; }
	RGB operator*(const double &scalar) const;
};

class Material {
private:
	int mindex, begin, end;
	double lambdaOne, lambdaTwo, lambdaThree;
	double k, alpha, translucence;
	bool null = false;
public:
	Material();
	Material(int, int, int, double, double, double, double, double, double);
	Material(const Material &);
	int model() const { return mindex; }
	int beginning() const { return begin; }
	int ending() const { return end; }
	double lone() const { return lambdaOne; }
	double ltwo() const { return lambdaTwo; }
	double lthree() const {return lambdaThree; }
	double rConst() const { return k; }
	double shininess() const { return alpha; }
	double translucency() const { return translucence; }
	bool isNull() { return null; }
};

#endif /* LIGHTMATERIAL_H_ */
