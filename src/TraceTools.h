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

namespace rayTracingTools{

// Detects collision between a ray and a triangle.
double detectTriangleCollision(const Ray &incoming, const std::vector<Vertex> &vertex);

// TODO: needs explanation.

inline double calcM(const Point &c1, const Point &c2, const Point &D){
	double one = c1.X() * ((c2.Y() * D.Z()) - (D.Y() * c2.Z()));
	double two = c1.Y() * ((D.X() * c2.Z()) - (c2.X() * D.Z()));
	double three = c1.Z() * ((c2.X() * D.Y()) - (c2.Y() * D.X()));
	return one + two + three;
}

inline double calcBeta(const Point &c2, const Point &D, const Point &aug, double M){
	double one = aug.X() * ((c2.Y() * D.Z()) - (D.Y() * c2.Z()));
	double two = aug.Y() * ((D.X() * c2.Z()) - (c2.X() * D.Z()));
	double three = aug.Z() * ((c2.X() * D.Y()) - (c2.Y() * D.X()));
	double sum = one + two + three;
	return sum / M;
}

inline double calcGamma(const Point &c1, const Point &D, const Point &aug, double M){
	double one = D.Z() * ((c1.X() * aug.Y()) - (aug.X() * c1.Y()));
	double two = D.Y() * ((aug.X() * c1.Z()) - (c1.X() * aug.Z()));
	double three = D.X() * ((c1.Y() * aug.Z()) - (aug.Y() * c1.Z()));
	double sum = one + two + three;
	return sum / M;
}

inline double calcTau(const Point &c1, const Point &c2, const Point &aug, double M){
	double one = c2.Z() * ((c1.X() * aug.Y()) - (aug.X() * c1.Y()));
	double two = c2.Y() * ((aug.X() * c1.Z()) - (c1.X() * aug.Z()));
	double three = c2.X() * ((c1.Y() * aug.Z()) - (aug.Y() * c1.Z()));
	double sum = one + two + three;
	return -(sum / M);
}

}


#endif /* TRACETOOLS_H_ */
