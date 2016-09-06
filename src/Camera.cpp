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

std::vector<Ray> Camera::shootAll(){
	Vector focal(focalPoint);
	Vector look(lookat);
	Vector direction(lookat - focalPoint);
	Normal vpn(direction);
}
