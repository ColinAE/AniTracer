/*
 * Polygon.cpp
 *
 *  Created on: Jul 5, 2016
 *      Author: colin
 */

#include "Polygon.h"
#include "GeometricOperations.h"
#include "TraceTools.h"
#include <iostream>

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
	double distance = 0;
	if(vertexCount == 3)
		distance = detectTriangleCollision(incoming, *this);
	return distance;
}

// TODO: needs explanation.
double Polygon::detectTriangleCollision(const Ray &incoming, const Polygon &face){
	Vector a = vertices.at(0);
	Vector b = vertices.at(1);
	Vector c = vertices.at(2);

	Vector c1 = a - b;
	Vector c2 = a - c;
	Normal D = incoming.getDirection();
	Vector aug = a - incoming.getOrigin();

	double maxlen = std::numeric_limits<double>::max(); //TODO: what does this do?

	double M = calcM(c1, c2, D);
	double tau = calcTau(c1, c2, aug, M);

	const double epsilon = .000000000001;

	if(tau + epsilon < 0 || tau - epsilon > maxlen){
		return maxlen;
	}
	double gamma = calcGamma(c1, D, aug, M);
	if(gamma + epsilon < 0 || gamma - epsilon > 1){
		return maxlen;
	}
	double beta = calcBeta(c2, D, aug, M);
	if(beta + epsilon < 0 || beta - epsilon > 1 - gamma)
		return maxlen;
	return tau;

}

Polygon::~Polygon() {
	// TODO Auto-generated destructor stub
}

