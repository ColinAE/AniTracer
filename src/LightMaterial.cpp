/*
 * LightMaterial.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: colin
 */

#include "LightMaterial.h"

Light::Light(){

}

Light::Light(double red, double green, double blue, double x, double y, double z, bool notAmbient){
	rgb = RGB(red, green, blue);
	position = Point(x, y, z);
	this->notAmbient = notAmbient;
}

RGB Light::operator*(const double &scalar) const{
	return rgb * scalar;
}

Material::Material(){
	begin = std::numeric_limits<double>::min();
	end = std::numeric_limits<double>::max();
	mindex = lambdaOne = lambdaTwo = lambdaThree = 1;
	k = alpha = translucence = 1;
	null = true;
}

Material::Material(int mindex, int begin, int end, double lambdaOne, double lambdaTwo, double lambdaThree, double reflectance, double shininess, double translucence){
	this->mindex = mindex;
	this->begin = begin;
	this->end = end;
	this->lambdaOne = lambdaOne;
	this->lambdaTwo = lambdaTwo;
	this->lambdaThree = lambdaThree;
	k = reflectance;
	alpha = shininess;
	this->translucence = translucence;
}

Material::Material(const Material &other){
	mindex = other.model();
	begin = other.beginning();
	end = other.ending();
	lambdaOne = other.lone();
	lambdaTwo = other.ltwo();
	lambdaThree = other.lthree();
	k = other.rConst();
	alpha = other.shininess();
	translucence = other.translucency();
}

