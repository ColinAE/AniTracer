/*
 * TraceTools.h
 *
 *  Created on: Sep 8, 2016
 *      Author: colin
 */

// Functions and classes directly supporting the ray tracing portion of the code.

#ifndef TRACETOOLS_H_
#define TRACETOOLS_H_

#include "LightMaterial.h"
#include <limits>

// A class that represents collisions between rays and objects.
// This class is simplifies the act of raytracing since it gets
// initialized with everything needed to represent a collision.
class Collision{
private:
	double travelDistance;
	Ray collidingRay;
	Normal surfaceNorm; // Surface of the normal that the ray hit.
	Material mat; // Material of the face that was hit.
	Point position; // Exact 3-D position of the collision on the face.
public:
	Collision();
	Collision(const Ray &collided, double distance, const Normal &norm,
			const Material &material);
	bool operator>(const Collision &other) const; // See if this object has greater travel distance.
	double getDistance() const { return travelDistance; }
	Material collisionMaterial() const { return mat; }
	Point getPosition() const { return position; }
	Normal getNormal() const { return surfaceNorm; }
	Vector getV() const { return collidingRay.getDirection() * -1; }
	Ray reflect();
	Ray refract();
};

#endif /* TRACETOOLS_H_ */
