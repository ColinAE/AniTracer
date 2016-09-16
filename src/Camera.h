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

// Camera object
// Contains everything required of a ray-tracing camera, including the screen.
class Camera{
private:
	Point focalPoint;
	Point lookat; //Point the camera is looking at
	Vector vup; //View Plane Normal
	double focalLength; //Length from focal point to screen
	Screen screen;

	double calcMidzoneOffset(const Vector &u, const Vector &v, const Vector &f);
public:
	Camera();
	Camera(Point focalPoint, Point lookat, Vector v, double focalLength, Screen scr);
	Camera(const Camera &other);
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
