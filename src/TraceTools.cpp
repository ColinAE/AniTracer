/*
 * TraceTools.cpp
 *
 *  Created on: Sep 8, 2016
 *      Author: colin
 */

#include "TraceTools.h"

const double epsilon = .000000000001;


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
