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

void Polygon::update(tMatrix trans){
	std::for_each(vertices.begin(), vertices.end(), [&](Vertex &vertex){
		vertex = trans.vmultiply(vertex);
	});
}

Collision Polygon::triangleCollision(const Ray &ray, const std::vector<Vertex> &vertices){
	Vector a = vertices.at(0);
	Vector b = vertices.at(1);
	Vector c = vertices.at(2);

	Vector c1 = a - b;
	Vector c2 = a - c;
	Normal D = ray.direction();
	Vector aug = a - ray.origin();

	double maxlen = -1; //TODO: what does this do?

	double M = calcM(c1, c2, D);
	double tau = calcTau(c1, c2, aug, M);

	if(tau + epsilon < 0 || tau - epsilon > maxlen){
		return false;
	}
	double gamma = calcGamma(c1, D, aug, M);
	//cout << "GAMMA: " << gamma << endl;
	if(gamma + epsilon < 0 || gamma - epsilon > 1){
		return false;
	}
	double beta = calcBeta(c2, D, aug, M);
	//cout << "BETA: " << beta << endl;
	if(beta + epsilon < 0 || beta - epsilon > 1 - gamma)
		return false;

}

Polygon::~Polygon() {
	// TODO Auto-generated destructor stub
}

