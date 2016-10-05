/*
 * GeoTypes.h
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

// Contains Point, types that are derived from it, and types that are closely
// related to Point in some way. All types are immutable.

#ifndef GEOTYPES_H
#define GEOTYPES_H

#include "Utility.h"
#include <math.h>
#include <string>
#include <vector>

using std::string;

class Point;
class Vector;
class Normal;
class Vertex;

// A class defining a 3-dimensional point.
class Point{
protected:
	double x, y, z;
public:
	Point();
	Point(double x, double y, double z);
	Point(const Point &);
	double X() const { return x; }
	double Y() const { return y; }
	double Z() const { return z; }
	bool operator==(const Point & other) const;
};

// Geometric vector. Defined in terms of the point class, since a vector is
// defined by a single point.
class Vector : public Point{
public:
	Vector()
		: Point() {};
	Vector(double x, double y, double z)
		: Point(x, y, z) {};
	Vector(const Point &point)
		: Point(point) {};
	Vector(const Vector &other) :
		Point(other.X(), other.Y(), other.Z()) {};
	Vector(const Point &origin, const Point &direction);
	Vector operator+(const Vector &other) const;
	Vector operator-(const Vector &other) const;
	Vector operator*(double scalar) const;
	Vector operator/(double scalar) const;
	double magnitude() const;
	Vector cross(const Vector &other) const; // Compute cross product.
	double dot(const Vector &other) const; // Compute dot product.
	Point destination() const { return Point(x, y, z); }
};

// A class that makes unit-length vectors. All constructors of this class
// normalize their arguments.
// You may think that this class is redundant, since we could just add a normalization method
// to the Vector class. However, having this class guarantees that any object declared Normal
// will be guaranteed to be unit-length. If Vector were mutable with a normalization method,
// it could introduce some very difficult to find bugs.
class Normal : public Vector{
private:
	Vector normalize(double x, double y, double z);
public:
	Normal() :
		Vector(1, 0, 0) {};
	Normal(double x, double y, double z);
	Normal(const Vector &other);
	Normal(const std::vector<Vertex> &unconstrainedPolygon);
};

// A class of points meant only to be used in the context of polygons.
// Has a to-String methd, which Point lacks.
// TODO: I will probably choose to consolidate this class with Point.
class Vertex : public Point{
public:
	Vertex() :
		Point() {};
	Vertex(double x, double y, double z) :
		Point(x, y, z) {};
	Vertex(const Point &point) :
		Point(point) {};
	Vertex(const Vertex &vertex) :
		Point(vertex.X(), vertex.Y(), vertex.Z()) {};
	string toString() const;
};

// Represents rays of light.
class Ray{
private:
	Point origin; // The origin of the ray.
	Normal direction; // The unit-length vector that points in the direction of the ray's travels
	int limit; // The number of maximum bounces a ray can perform.
public:
	Ray();
	Ray(const Point &center, const Vector &direction, int limit);
	Normal getDirection() const { return direction; }
	Point getOrigin() const { return origin; }
	bool reachedLimit() const { return (limit <= 0); } // Checks if ray has bounced to its limit.
	int getLimit() const { return limit; }
	Ray reflect(const double &distance, const Normal &N) const;
	Ray refract(const double &distance, const Normal &N) const;
};

#endif
