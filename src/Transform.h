/*
 * transformer.h
 *
 *  Created on: Mar 19, 2016
 *      Author: colin
 */

//TODO: Error checking for transformation matrix types

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "Singular.h"
#include "Utility.h"

class Matrix;

// Linear operations, like multiplication between matrices and vectors, as well as transposition.
namespace lops{

	// Matrix-vector multiplication.
	std::vector<double> vmultiply(const Matrix &matrix, const std::vector<double> &vec);

	// Matrix-matrix multiplication.
	template<class matrixType>
	matrixType multiply(const Matrix &left, const Matrix &right);

	// Matrix transpose.
	template<class matrixType>
	matrixType transpose(const Matrix &matrix);
}

// Matrix base class that is only intended to be instantiated as 4 by 4.
// tMatrix and Matrix are separate since this class could be used beyond 4 by 4.
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
	string toString() const;
};

// Transformation matrix classes.
// Operations between tMatrix and its child classes produce a basic tMatrix type.

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

// IMPORTANT NOTE:
// The scale class of transformation matrices can do
// both uniform and non-uniform scaling. Nowhere in the Scale code is it assumed
// which kind of scaling is being performed. It is all up to the user/calling
// code.
class Scale : public tMatrix{
public:
	Scale(double x, double y, double z);
};

class Identity : public tMatrix{
public:
	Identity();
};

// Axisangle never takes in the rotation value.
// It creates only the basis matrix, not the entire axis-angle rotation composition.
// Transformer uses Axisangle to create whole composition.
class Axisangle : public tMatrix{
public:
	Axisangle(double x, double y, double z);
};

// Composites transformation matrices together.
class Transformer {
private:
	tMatrix tcomposition;
public:
	Transformer() { tcomposition = Identity(); }
	void scale(double x, double y, double z);
	void translate(double x, double y, double z);
	void axis_angle(double x, double y, double z, double theta);
	void update(const tMatrix &next);
	tMatrix transformationMatrix() { return tcomposition; }
	string toString() const;
};

#endif
