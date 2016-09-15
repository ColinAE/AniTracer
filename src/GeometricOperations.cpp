/*
 * LinearOperations.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: colin
 */

// Miscellaneous operations involved with geometry.
// Namespace geops contains several functions that involve geometry.
// I did not take into account the strictest definition of "geometric operation" when
// naming this namespace. I could therefore be persuaded to change the name.

#include "GeometricOperations.h"
#include <iostream>

// Calculate centroids of polygon that is a vector of points
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
	// If the face is already a triangle, add it to the data structure
	if(count == 3){
		triangles.push_back(face);

	// If face is a quadrilateral, split into two triangles
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

Normal geops::perpendicular(const std::vector<Vertex> &unconstrainedPolygon){

	// Grab each point within the polygon;
	Point one = unconstrainedPolygon[0];
	Point two = unconstrainedPolygon[1];
	Point three = unconstrainedPolygon[2];

	// Form two vectors from the triangle points.
	Vector left = Vector(one, two);
	Vector right = Vector(two, three);

	// Compute the vector that is perpendicular to the polygon.
	Vector perpendicularDirection = left.cross(right);
	double x = perpendicularDirection.X();
	double y = perpendicularDirection.Y();
	double z = perpendicularDirection.Z();
	return Normal(x, y, z);
}
