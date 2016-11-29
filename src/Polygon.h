/*
 * Polygon.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>
#include <string>
#include <algorithm>

#include "Singular.h"
#include "Transform.h"

using std::string;

// Polygon object, which is made up of three or more vertices (strictly three
// if it is a polygon within a model that has been loaded by the IO functions).
// Contains a centroid and a surface normal.
// Supports ray-TRIANGLE!!!! intersection.
class Polygon{
private:
	std::vector<Vertex> vertices;
	int vertexCount;
	Point center; //centroid of polygon
	Normal norm;

	// Detects collision between a ray and a triangle.
	double detectTriangleCollision(const Ray &incoming, const Polygon &face);

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

public:
	Polygon();
	Polygon(const std::vector<Vertex> &vertices);
	~Polygon();
	int getVertexCount() const { return vertexCount; }
	Point centroid() const { return center; }
	Vertex operator[](int index) const { return vertices[index]; }
	Vertex at(int index) const {return vertices.at(index); }
	Normal normal() const { return norm; }
	string toString() const;
	void update(const tMatrix trans);
	double collide(const Ray &);
};

#endif /* POLYGON_H_ */
