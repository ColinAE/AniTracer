/*
 * transformer.h
 *
 *  Created on: Mar 19, 2016
 *      Author: colin
 */

//TODO: Error checking for transformation matrix types

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Singular.h"
#include <vector>

class Matrix;

namespace lops{
	std::vector<double> vmultiply(const Matrix &, const std::vector<double> &);

	template<class matrixType>
	matrixType multiply(const Matrix &, const Matrix &);

	template<class matrixType>
	matrixType transpose(const Matrix &);
}

class Matrix{
protected:
	std::vector< std::vector<double> > elements;
	int rows, columns;
public:
	Matrix();
	Matrix(const std::vector< std::vector<double> > &);
	Matrix(const Matrix &);
	bool operator==(const Matrix &);
	double at(int, int) const;
	Matrix multiply(const Matrix &) const;
	Matrix transpose() const;
	std::vector<double> vmultiply(const std::vector<double> &) const;
	Vector vmultiply(const Vector &) const;
	std::vector<double> row(int rowIndex) const { return elements.at(rowIndex); }
	std::vector< std::vector<double> > getElms(){ return elements; }
	int rowCount() const { return rows; }
	int colCount() const { return columns; }
};

class tMatrix : public Matrix{
public:
	tMatrix();
	tMatrix(const Matrix &other) :
		Matrix(other) {};
	tMatrix(const tMatrix &other) :
		Matrix(other) {};
	tMatrix multiply(const Matrix &) const;
	tMatrix transpose() const;
};

class Xrotation : public tMatrix{
public:
	Xrotation(double theta);
};

class Yrotation : public tMatrix{
public:
	Yrotation(double theta);
};

class Zrotation : public tMatrix{
public:
	Zrotation(double theta);
};

class Translate : public tMatrix{
public:
	Translate(double x, double y, double z);
};

class Scale : public tMatrix{
public:
	Scale(double x, double y, double z);
};

class Identity : public tMatrix{
public:
	Identity();
};

class Axisangle : public tMatrix{
public:
	Axisangle(double, double, double);
};

class Transformer {
private:
	tMatrix tcomposition;
public:
	Transformer() { tcomposition = Identity(); }
	void scale(double, double, double);
	void translate(double, double, double);
	void axis_angle(double, double, double, double);
	void update(const tMatrix &);
	tMatrix transformationMatrix() { return tcomposition; }
};

#endif
