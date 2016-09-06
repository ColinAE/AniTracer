/*
 * GeoTypes.h
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

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
	bool operator==(const Point &) const;
};

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
	Vector(const Point &, const Point &);
	Vector operator+(const Vector &) const;
	Vector operator-(const Vector &) const;
	Vector operator*(double) const;
	Vector operator/(double) const;
	double magnitude() const;
	Vector cross(const Vector &) const;
	double dot(const Vector &) const;
};

class Normal : public Vector{
private:
	Vector normalize(double, double, double);
public:
	Normal() :
		Vector(1, 0, 0) {};
	Normal(double, double, double);
	Normal(const Vector &);
	Normal(const std::vector<Vertex> &);
};

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

class Ray{
private:
	Point center;
	Normal direction;
public:
	Ray();
	Ray(const Point &, const Vector &);
};

#endif
