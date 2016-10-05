/*
 * TraceTools.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: colin
 */

#include "TraceTools.h"

const double epsilon = .000000000001;

Collision::Collision(){
	travelDistance = -1;
	collidingRay = Ray();
	surfaceNorm = Normal();
	mat = Material();
	position = Point();
}

Collision::Collision(const Ray &collided, double distance, const Normal &norm, const Material &material){
	travelDistance = distance;
	collidingRay = collided;
	surfaceNorm = norm;
	Material mat = material;
	Point position = (collided.getDirection() * distance + Vector(collided.getOrigin())).destination();
}

// Returns true if this object has a larger travel distance than the other object.
// False otherwise.
bool Collision::operator>(const Collision &other) const {
	return travelDistance > other.getDistance();
}

Ray Collision::reflect(){
	return collidingRay.reflect(travelDistance, surfaceNorm);
}

Ray Collision::refract(){
	return collidingRay.refract(travelDistance, surfaceNorm);
}

// TODO: needs explanation.
double rayTracingTools::detectTriangleCollision(const Ray &incoming,
		const std::vector<Vertex> &vertices){
	Vector a = vertices.at(0);
	Vector b = vertices.at(1);
	Vector c = vertices.at(2);

	Vector c1 = a - b;
	Vector c2 = a - c;
	Normal D = incoming.getDirection();
	Vector aug = a - incoming.getOrigin();

	double maxlen = std::numeric_limits<double>::max(); //TODO: what does this do?

	double M = rayTracingTools::calcM(c1, c2, D);
	double tau = rayTracingTools::calcTau(c1, c2, aug, M);

	if(tau + epsilon < 0 || tau - epsilon > maxlen){
		return maxlen;
	}
	double gamma = rayTracingTools::calcGamma(c1, D, aug, M);
	if(gamma + epsilon < 0 || gamma - epsilon > 1){
		return maxlen;
	}
	double beta = rayTracingTools::calcBeta(c2, D, aug, M);
	if(beta + epsilon < 0 || beta - epsilon > 1 - gamma)
		return maxlen;
	return tau;

}

/* FOR REFERENCE
//Check if the ray collides with a given triangle
bool collideTriangle(const Ray &ray, const vector<Model> &models,
					int index, int mindex, Tracker &tracker){

	double maxlen = -1;
	for(int i = 0; i < 2; i++){
		double cur = pDistance(ray.getOrigin(), models[mindex].boundingBox(i));
		if(cur > maxlen) maxlen = cur;
	}

	Polygon face = models[mindex].getFace(index);
	vector<int> points = face.getPoints();
	Point A = models[mindex].getVertex(points[0]);
	Point B = models[mindex].getVertex(points[1]);
	Point C = models[mindex].getVertex(points[2]);
	Point D = ray.getDirection().getDirection();
	Point c1 = pSubtract(A, B);
	Point c2 = pSubtract(A, C);

	Point aug = pSubtract(A, ray.getOrigin());

	double M = calcM(c1, c2, D);
	//NOTE: this tau comparison might be a bug
	double tau = calcTau(c1, c2, aug, M);
	//cout << "\nTAU: " << tau << endl;
	if(tau + epsilon < 0 || tau - epsilon > maxlen)
		return false;
	double gamma = calcGamma(c1, D, aug, M);
	//cout << "GAMMA: " << gamma << endl;
	if(gamma + epsilon < 0 || gamma - epsilon > 1)
		return false;
	double beta = calcBeta(c2, D, aug, M);
	//cout << "BETA: " << beta << endl;
	if(beta + epsilon < 0 || beta - epsilon > 1 - gamma)
		return false;
	if(tau < tracker.T()){
		tracker.setT(tau);
		tracker.setMindex(mindex);
		tracker.setIndex(index);
	}
	//cout << "true" << endl;
	return true;
}

*/
