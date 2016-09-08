/*
 * TraceTools.h
 *
 *  Created on: Sep 8, 2016
 *      Author: colin
 */

#ifndef TRACETOOLS_H_
#define TRACETOOLS_H_

class Collision{
private:
public:
	Collision(double, );
};

inline double calcM(const Point &c1, const Point &c2, const Point &D){
	double one = c1.X() * ((c2.Y() * D.Z()) - (D.Y() * c2.Z()));
	double two = c1.Y() * ((D.X() * c2.Z()) - (c2.X() * D.Z()));
	double three = c1.Z() * ((c2.X() * D.Y()) - (c2.Y() * D.X()));
	return one + two + three;
}

inline double calcBeta(const Point &c2, const Point &D, const Point &aug, double M){
	double one = aug.X() * ((c2.Y() * D.Z()) - (D.Y() * c2.Z()));
	double two = aug.Y() * ((D.X() * c2.Z()) - (c2.X() * D.Z()));
	double three = aug.Z() * ((c2.X() * D.Y()) - (c2.Y() * D.X()));
	double sum = one + two + three;
	return sum / M;
}

inline double calcGamma(const Point &c1, const Point &D, const Point &aug, double M){
	double one = D.Z() * ((c1.X() * aug.Y()) - (aug.X() * c1.Y()));
	double two = D.Y() * ((aug.X() * c1.Z()) - (c1.X() * aug.Z()));
	double three = D.X() * ((c1.Y() * aug.Z()) - (aug.Y() * c1.Z()));
	double sum = one + two + three;
	return sum / M;
}

inline double calcTau(const Point &c1, const Point &c2, const Point &aug, double M){
	double one = c2.Z() * ((c1.X() * aug.Y()) - (aug.X() * c1.Y()));
	double two = c2.Y() * ((aug.X() * c1.Z()) - (c1.X() * aug.Z()));
	double three = c2.X() * ((c1.Y() * aug.Z()) - (aug.Y() * c1.Z()));
	double sum = one + two + three;
	return -(sum / M);
}


#endif /* TRACETOOLS_H_ */
