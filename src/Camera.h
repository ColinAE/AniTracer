/*
 * Camera.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Singular.h"
#include "Screen.h"

class Camera{
private:
	Point focalPoint;
	Point lookat;
	Vector vup;
	double focalLength;
	Screen screen;
public:
	Camera();
	Camera(Point, Point, Vector, double, Screen);
	Camera(const Camera &);
	Point getFocalPoint() const { return focalPoint; }
	Point getLookat() const { return lookat; }
	Vector getVUP() const { return vup; }
	double getFocalLength() const { return focalLength; }
	Screen getScreen() const { return screen; }
	std::vector<Ray> shootAll();
};

#endif /* CAMERA_H_ */
