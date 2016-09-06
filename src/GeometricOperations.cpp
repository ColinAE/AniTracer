/*
 * LinearOperations.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: colin
 */

#include "GeometricOperations.h"
#include <iostream>

namespace geops{

// Calculate centroids
Point centroid(const std::vector<Point> &points){
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

Point centroid(const std::vector<Vertex> &points){
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

std::vector<Polygon> triangularize(const Polygon &face){
	int count = face.getVertexCount();
	std::vector<Polygon> triangles;
	// If the face is already a triangle, make a polygon
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
	// number of triangles equal to the number of vertices in the original face
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

Vector perpendicular(const std::vector<Vertex> &unconstrainedPolygon){
	Point one = unconstrainedPolygon[0];
	Point two = unconstrainedPolygon[1];
	Point three = unconstrainedPolygon[2];
	Vector left = Vector(one, two);
	Vector right = Vector(two, three);
	Vector normalDirection = left.cross(right);
	double x = normalDirection.X();
	double y = normalDirection.Y();
	double z = normalDirection.Z();
	return Vector(x, y, z);
}

}
