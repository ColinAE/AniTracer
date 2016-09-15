/*
 * Camera.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

// Camera method definitions

#include "Camera.h"

//TODO figure out what to do with Camera default constructor
Camera::Camera(){}

Camera::Camera(Point focalPoint, Point lookat, Vector v, double focalLength, Screen scr){
	this->focalPoint = focalPoint;
	this->lookat = lookat;
	this->vup = v;
	this->focalLength = focalLength;
	this->screen = scr;
}

Camera::Camera(const Camera &other){
	focalPoint = other.focalPoint;
	lookat = other.lookat;
	vup = other.vup;
	focalLength = other.focalLength;
	screen = other.screen;
}

//TODO: keep or eliminate this method
double Camera::calcMidzoneOffset(const Vector &u, const Vector &v, const Vector &f){
	double fmag = f.magnitude();
	double vmag = v.magnitude();
	double vertPythag = sqrt((fmag * fmag) + (vmag * vmag));
	double umag = u.magnitude();
	double horiPythag = sqrt((vertPythag * vertPythag) + (umag * umag));
	return horiPythag;
}

std::vector<Ray> Camera::shootAll(){
	Vector focal(focalPoint);
	Vector look(lookat);
	Normal vpn(look - focal); //View plane normal
	Vector focalVec = vpn * focalLength;
	Vector toScreenCenter = focal + focalVec;
	Normal unorm = vpn.cross(vup); // Horizontal pixel position normal
	Normal vnorm = unorm.cross(vpn); // Vertical pixel position normal

	std::vector<Ray> rays;
	int lowu = screen.getlowu();
	int lowv = screen.getlowv();
	int highu = screen.gethighu();
	int highv = screen.gethighv();

	// Compute all rays
	// Outer loop iterates through screen rows from top to bottom.
	for(int i = highv; i > lowv; i--){
		Vector v = vnorm * i;

		// Inner loop iterates through columns from left to right.
		for(int j = lowu; j < highu; j++){
			Vector u = unorm * j;
			Vector uvPosition = u + v; // Pixel through which ray will be shot.
			Vector pixelPosition3D = uvPosition + toScreenCenter;
			Normal rayDirection = pixelPosition3D - focal;
			Point rayOrigin = Point(pixelPosition3D.X(), pixelPosition3D.Y(), pixelPosition3D.Z());
			rays.push_back(Ray(rayOrigin, rayDirection, 20)); // last number in Ray constructor is countdown
		}
	}

	return rays;
}
