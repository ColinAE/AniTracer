/*
 * LinearOperations.h
 *
 *  Created on: Mar 18, 2016
 *      Author: colin
 */

#ifndef GEOMETRICOPERATIONS_H
#define GEOMETRICOPERATIONS_H

#include "Polygon.h"
#include "Singular.h"
#include <vector>

namespace geops{

	// Cut polygon into triangles. All convex polygons can be cut into multiple triangles.
	std::vector<Polygon> triangularize(const Polygon &);

	// Calculate centroid of polygon that is made up of a vector of vertices.
	Point centroid(const std::vector<Point> &);
	Point centroid(const std::vector<Vertex> &);

	// Compute vector that is perpendicular to a polygon point vector.
	// Assumes polygon vertices vector contains three points / is a triangle.
	// TODO: should still work with more than three points. Test this possibility.
	Normal perpendicular(const std::vector<Vertex> &);

	/*
	double distanceShell(const Shell &, const Shell &);
	vector<Point> boundingBox(const vector<Point> &);
	vector<Material> getMaterials(int, int, const vector<Material>);
	double shellRadius(const Polygon &, const Point &, const Model &);
	Point midpointShell(const Shell &, const Shell &);
	*/
}

#endif
