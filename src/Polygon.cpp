/*
 * Polygon.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "Polygon.h"
#include "GeometricOperations.h"

// Polygon definitions
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
	Vector perp = geops::perpendicular(vertices);
	norm = Normal(perp);
}

string Polygon::toString() const{
	string collectAll = "";
	std::for_each(vertices.begin(), vertices.end(), [&](Vertex vertex){
		string current = vertex.toString();
		collectAll += current;
	});
	return collectAll;
}

/*
void Polygon::transform(tMatrix transformation){
	std::vector<Vertex> newVertices;
	for(int i = 0; i < vertices.size(); i++){
		Vertex current = vertices.at(i);
		newVertices.push_back(Vertex(transformation * current));
	}
	vertices = newVertices;
	center = Point(transformation * center);
	norm = Normal(transformation * norm);
}
*/

Polygon::~Polygon() {
	// TODO Auto-generated destructor stub
}

