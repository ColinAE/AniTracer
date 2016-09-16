/*
 * LinearOperations.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: colin
 */

#include "GeometricOperations.h"
#include <iostream>

// Computes the geometric center of a collection of points.
// Does not take a Polygon object for same reasons as the computeNormal function.
Point geops::centroid(const std::vector<Point> &points){
	double x, y, z;
	x = y = z = 0;
	int len = points.size();
	for(int i = 0; i < len; i++){
		Point point = points[i];
		x += point.X();
		y += point.Y();
		z += point.Z();
	}
	x /= len; y /= len;	z /= len;

	return Point(x, y, z);
}

Point geops::centroid(const std::vector<Vertex> &points){
	return centroid(points);
}

std::vector<Polygon> geops::triangularize(const Polygon &face){
	int count = face.getVertexCount();
	std::vector<Polygon> triangles;

	// If the face is already a triangle, this adds it to the vector.
	if(count == 3){
		triangles.push_back(face);

	// If face is a quadrilateral, this splits it into two triangles without computing a centroid.
	} else if(count == 4){
		std::vector<Vertex> triangle;

		triangle.push_back(face[0]);
		triangle.push_back(face[1]);
		triangle.push_back(face[3]);
		triangles.push_back(Polygon(triangle));

		triangle.clear();
		triangle.push_back(face[1]);
		triangle.push_back(face[2]);
		triangle.push_back(face[3]);
		triangles.push_back(Polygon(triangle));

	// If face is a polygon with five or greater vertices, calculate centroid and divide into
	// number of triangles equal to the number of vertices in the original face.
	// All resulting faces contain two points from the edge of the polygon as well as the centroid.
	// This can potentially result in very thin triangles if the polygon has many vertices.
	// Assumes faces are convex.
	} else {
		Point centroid = face.centroid();
		for(int i = 0; i < count; i++){
			std::vector<Vertex> triangle;
			triangle.push_back(face[i]);
			triangle.push_back(centroid);
			triangle.push_back(face[(i + 1) % count]);
			triangles.push_back(Polygon(triangle));
		}
	}

	return triangles;
}

// Since this function is used to compute the surface normal within the Polygon constructor,
// the parameter is an "unconstrained polygon" instead of a Polygon object. That is, it is a vector of vertices
// that should define a polygon. Does not perform checking whether or not the vertices actually
// define a polygon. One could define a function that takes a Polygon object as well, but since
// the surface normals within the Polygons are pre-computed using this function, that would be
// redundant.
Normal geops::computeNormal(const std::vector<Vertex> &unconstrainedPolygon){

	// Grabs each point within the polygon;
	Point one = unconstrainedPolygon[0];
	Point two = unconstrainedPolygon[1];
	Point three = unconstrainedPolygon[2];

	// Forms two vectors from the triangle points.
	Vector left = Vector(one, two);
	Vector right = Vector(two, three);

	// Computes the vector that is perpendicular to the polygon.
	Vector perpendicularDirection = left.cross(right);
	double x = perpendicularDirection.X();
	double y = perpendicularDirection.Y();
	double z = perpendicularDirection.Z();
	return Normal(x, y, z);
}
