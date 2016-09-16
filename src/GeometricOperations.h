/*
 * LinearOperations.h
 *
 *  Created on: Mar 18, 2016
 *      Author: colin
 */

// Miscellaneous operations involved with geometry.

#ifndef GEOMETRICOPERATIONS_H
#define GEOMETRICOPERATIONS_H

#include "Polygon.h"
#include "Singular.h"
#include <vector>

// Namespace geops contains several functions that involve geometry.
// I did not take into account the strictest definition of "geometric operation" when
// naming this namespace. I could therefore be persuaded to change the name.
namespace geops{

	// Cuts polygon into triangles. All convex polygons can be cut into multiple triangles.
	std::vector<Polygon> triangularize(const Polygon &points);

	// Calculates centroid of polygon that is made up of a vector of vertices.
	Point centroid(const std::vector<Point> &points);
	Point centroid(const std::vector<Vertex> &face);

	// Computes a surface normal.
	// Assumes polygon vertices vector contains three points / is a triangle.
	// unconstrainedPolygon - Vector of vertices that should make up a polygon.
	// TODO: should still work with more than three points. Test this possibility.
	Normal computeNormal(const std::vector<Vertex> &unconstrainedPolygon);

	/*
	double distanceShell(const Shell &, const Shell &);
	vector<Point> boundingBox(const vector<Point> &);
	vector<Material> getMaterials(int, int, const vector<Material>);
	double shellRadius(const Polygon &, const Point &, const Model &);
	Point midpointShell(const Shell &, const Shell &);
	*/
}

#endif
