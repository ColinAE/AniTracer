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
	Point lookat; //Point the camera is looking at
	Vector vup; //View Plane Normal
	double focalLength; //Length from focal point to screen
	Screen screen;

	double calcMidzoneOffset(const Vector &, const Vector &, const Vector &);
public:
	Camera();
	Camera(Point, Point, Vector, double, Screen);
	Camera(const Camera &);
	Point getFocalPoint() const { return focalPoint; }
	Point getLookat() const { return lookat; }
	Vector getVUP() const { return vup; }
	double getFocalLength() const { return focalLength; }
	Screen getScreen() const { return screen; }

	//Generate vector of rays for each pixel on the screen.
	//Rays have origins on the pixel/screen plane
	std::vector<Ray> shootAll();
};

#endif /* CAMERA_H_ */
