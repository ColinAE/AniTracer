/*
 * Polygon.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "Polygon.h"
#include "GeometricOperations.h"
#include "TraceTools.h"

Polygon::Polygon(){
	vertices.push_back(Vertex());
	vertices.push_back(Vertex());
	vertices.push_back(Vertex());
	vertexCount = vertices.size();
	center = geops::centroid(vertices);
	norm = Normal(vertices);
}


Polygon::Polygon(const std::vector<Vertex> &vertices){
	this->vertices = vertices;
	vertexCount = vertices.size();
	center = geops::centroid(vertices);
	norm = geops::computeNormal(vertices);
}

// Format: X Y Z
// One vertex on each line.
string Polygon::toString() const{
	string collectAll = "";
	std::for_each(vertices.begin(), vertices.end(), [&](Vertex vertex){
		string current = vertex.toString();
		collectAll += current + "\n";
	});
	return collectAll;
}

void Polygon::update(const tMatrix trans){
	std::for_each(vertices.begin(), vertices.end(), [&](Vertex &vertex){
		vertex = trans.vmultiply(vertex);
	});
}

double Polygon::collide(const Ray &incoming){
	double distance = rayTracingTools::detectTriangleCollision(incoming, vertices);
	return distance;
}

Polygon::~Polygon() {
	// TODO Auto-generated destructor stub
}

