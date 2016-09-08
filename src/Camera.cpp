/*
 * Camera.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "Camera.h"

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

double calcMidzoneOffset(const Vector &u, const Vector &v, const Vector &f){
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
	Vector direction(look - focal);
	Normal vpn(direction);
	Vector f = vpn * focalLength;
	Vector center = f + focal;
	Normal unorm = vpn.cross(vup);
	Normal vnorm = unorm.cross(vpn);

	std::vector<Ray> rays;
	int lowu = screen.getlowu();
	int lowv = screen.getlowv();
	int highu = screen.gethighu();
	int highv = screen.gethighv();
	for(int i = highv; i > lowv; i--){
		Vector v = vnorm * i;
		for(int j = lowu; j < highu; j++){
			Vector u = unorm * j;
			Vector uvPosition = u + v;
			Vector position3D = uvPosition + center;
			Normal angle = position3D - focal;
			Point xyz = Point(position3D.X(), position3D.Y(), position3D.Z());
			rays.push_back(Ray(center, angle));
		}
	}

	return rays;
}
