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

class Polygon{
private:
	std::vector<Vertex> vertices;
	int vertexCount;
	Point center;
	Normal norm;

	Collision Polygon::detectTriangleCollision(const Ray &, const std::vector<Vertex> &);
public:
	Polygon();
	Polygon(const std::vector<Vertex> &vertices);
	~Polygon();
	int getVertexCount() const { return vertexCount; }
	Point centroid() const {return center; }
	Vertex operator[](int index) const { return vertices[index]; }
	Normal normal() const { return norm; }
	string toString() const;
	void update(tMatrix trans);
};

#endif /* POLYGON_H_ */
