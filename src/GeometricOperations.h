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

	std::vector<Polygon> triangularize(const Polygon &);
	Point centroid(const std::vector<Point> &);
	Point centroid(const std::vector<Vertex> &);
	Vector perpendicular(const std::vector<Vertex> &);
	/*
	double distanceShell(const Shell &, const Shell &);
	vector<Point> boundingBox(const vector<Point> &);
	vector<Material> getMaterials(int, int, const vector<Material>);
	double shellRadius(const Polygon &, const Point &, const Model &);
	Point midpointShell(const Shell &, const Shell &);
	*/
}

#endif
