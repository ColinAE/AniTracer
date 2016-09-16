/*
 * Polygon.h
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Singular.h"
#include "Transform.h"
#include <vector>
#include <string>
#include <algorithm>

using std::string;

// Polygon object, which is made up of three or more vertices (hopefully three
// if it is a polygon within a model).
// Contains a centroid and a surface normal.
// Supports ray-TRIANGLE!!!! intersection.
class Polygon{
private:
	std::vector<Vertex> vertices;
	int vertexCount;
	Point center; //centroid of polygon
	Normal norm;

public:
	Polygon();
	Polygon(const std::vector<Vertex> &vertices);
	~Polygon();
	int getVertexCount() const { return vertexCount; }
	Point centroid() const { return center; }
	Vertex operator[](int index) const { return vertices[index]; }
	Normal normal() const { return norm; }
	string toString() const;
	void update(const tMatrix trans);
	double collide(const Ray &);
};

#endif /* POLYGON_H_ */
