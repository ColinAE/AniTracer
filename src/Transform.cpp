/*
 * transform.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: colin
 */

//TODO: More error checking for matrix operations

#include "Transform.h"

#include <iostream>

namespace lops{


std::vector<double> vmultiply(const Matrix &matrix, const std::vector<double> &vec){
	int rows = matrix.rowCount();
	int columns = matrix.colCount();
	std::vector<double> newVec;
	for(int i = 0; i < rows; i++){
		double accumulator = 0;
		for(int j = 0; j < columns; j++){
			accumulator += matrix.at(i, j) * vec[j];
		}
		newVec.push_back(accumulator);
	}
	return newVec;
}

template <class matrixType>
matrixType multiply(const Matrix &left, const Matrix &right){
	//std::cout << left.toString() << std::endl;
	//std::cout << right.toString() << std::endl;
	Matrix matrans = right.transpose();
	int rowCount = right.rowCount();
	std::vector<std::vector<double>> finalVecs(rowCount);
	for(int i = 0; i < rowCount; i++){
		finalVecs[i] = lops::vmultiply(left, matrans.row(i));
	}

	Matrix final = Matrix(finalVecs).transpose();
	//std::cout << final.toString() << std::endl;

	return matrixType(Matrix(finalVecs).transpose());
}

template <class matrixType>
matrixType transpose(const Matrix &matrix){
	int columns = matrix.colCount();
	int rows = matrix.rowCount();
	std::vector<std::vector<double>> trans(columns);
	for(int i = 0; i < columns; i++){
		trans[i] = std::vector<double>(rows);
	}

	for(int i = 0; i < columns; i++){
		for(int j = 0; j < rows; j++){
			trans[i][j] = matrix.at(j, i);
		}
	}
	matrixType ret = Matrix(trans);
	return ret;
}
}

// Matrix class
//~
Matrix::Matrix(){
	elements = std::vector<std::vector<double>>(0);
	rows = 0;
	columns = 0;
}

Matrix::Matrix(const std::vector< std::vector<double> > &elms){
	int overlen = elms.size();
	int lastlen = 0;
	int nextlen = 0;
	//Make sure the elements are triangular
	for(int i = 1; i < overlen; i++){
		lastlen = elms[i - 1].size();
		nextlen = elms[i].size();
		if(lastlen != nextlen){
			//TODO: throw error
		}
	}
	elements = elms;
	rows = elms.size();
	columns = elms[0].size();
}

Matrix::Matrix(const Matrix &other){
	rows = other.rowCount();
	columns = other.colCount();
	elements = std::vector<std::vector<double>>(rows);
	for(int i = 0; i < rows; i++){
		elements[i] = std::vector<double>(columns);
		for(int j = 0; j < columns; j++){
			elements[i][j] = other.at(i, j);
		}
	}
}

bool Matrix::operator==(const Matrix &other){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			if(elements[i][j] != other.at(i, j))
				return false;
		}
	}
	return true;
}

double Matrix::at(int i, int j) const{
	return elements[i][j];
}

/*
Point Matrix::vmultiply(const Point &point) const{
	std::vector<double> vec = {point.X(), point.Y(), point.Z(), 1};
	std::vector<double> ret = this * vec;
	return Point(ret[0], ret[1], ret[2]);
}
*/

Matrix Matrix::multiply(const Matrix &matrix) const{
	return lops::multiply<Matrix>(*this, matrix);
}

Matrix Matrix::transpose() const{
	return lops::transpose<Matrix>(*this);
}

std::vector<double> Matrix::vmultiply(const std::vector<double> &vec) const{
	return lops::vmultiply(*this, vec);
}

Vector Matrix::vmultiply(const Vector &vec) const{
	std::vector<double> convertedVec = {vec.X(), vec.Y(), vec.Z()};
	convertedVec = lops::vmultiply(*this, convertedVec);
	Vector ret = Vector(convertedVec.at(0), convertedVec.at(1), convertedVec.at(2));
	return ret;
}

string Matrix::toString() const{
	string all;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			all += dtos(elements[i][j]) + " ";
		}
		all += "\n";
	}
	return all;
}

// Transformation types
//TODO Check the dimensions of the matrices that are passed the non-default constructor
tMatrix::tMatrix() :
	Matrix(){
	rows = 4;
	columns = 4;
	elements = std::vector<std::vector<double>>(rows);
	for(int i = 0; i < 4; i++){
		elements[i] = std::vector<double>(columns);
	}
}

tMatrix tMatrix::multiply(const Matrix &other) const{
	return lops::multiply<tMatrix>(*this, other);
}

tMatrix tMatrix::transpose() const{
	return lops::transpose<tMatrix>(*this);
}

Xrotation::Xrotation(double theta)
	: tMatrix(){

	elements[0][0] = 1;
	elements[1][1] = cos(theta);
	elements[1][2] = -sin(theta);
	elements[2][1] = sin(theta);
	elements[2][2] = cos(theta);
	elements[3][3] = 1;
}

Yrotation::Yrotation(double theta)
	: tMatrix(){

	elements[0][0] = cos(theta);
	elements[0][2] = -sin(theta);
	elements[1][1] = 1;
	elements[2][0] = sin(theta);
	elements[2][2] = cos(theta);
	elements[3][3] = 1;
}

Zrotation::Zrotation(double theta)
	: tMatrix(){

	elements[0][0] = cos(theta);
	elements[0][1] = -sin(theta);
	elements[1][0] = sin(theta);
	elements[1][1] = cos(theta);
	elements[2][2] = 1;
	elements[3][3] = 1;
}

Translate::Translate(double x, double y, double z)
	: tMatrix(){

	elements[0][0] = 1;
	elements[1][1] = 1;
	elements[2][2] = 1;
	elements[3][3] = 1;

	elements[0][3] = x;
	elements[1][3] = y;
	elements[2][3] = z;
}

Scale::Scale(double x, double y, double z)
	: tMatrix(){

	elements[0][0] = x;
	elements[1][1] = y;
	elements[2][2] = z;
	elements[3][3] = 1;
}

Identity::Identity()
	: tMatrix(){

	elements[0][0] = 1;
	elements[1][1] = 1;
	elements[2][2] = 1;
	elements[3][3] = 1;
}

Axisangle::Axisangle(double x, double y, double z)
	: tMatrix(){
	std::cout << "axis " << std::endl;

	//The axis of rotation.
	Vector axis(x, y, z);

	//Normalize the axis of rotation to acquire 'w' row.
	Normal w(axis);
	x = w.X();
	y = w.Y();
	z = w.Z();
	std::cout << "W: " << x << " " << y << " " << z << std::endl;

	//Acquire 'u' row using the lowest-equals-one-renormalize-then-cross-with-w heuristic.
	Normal u;
	if((x < y || x == y)  && (x < z || x == z)){
		Normal m(1, y, z);
		std::cout << "m: " << m.X() << " " << m.Y() << " " << m.Z() << std::endl;
		u = w.cross(m);
	}
	else if((y < x || y == x) && (y < z || y == z)){
		u = w.cross(Normal(x, 1, z));
	}
	else{
		u = w.cross(Normal(x, y, 1));
	}
	std::cout << "u: " << u.X() << " " << u.Y() << " " << u.Z() << " " << std::endl;

	//Acquire 'v' row by crossing 'w' with 'u'. Could also swap 'w' and 'u' in the cross product.
	Normal v = w.cross(u);
	std::cout << "v: " <<  v.X() << " " << v.Y() << " " << v.Z() << " " << std::endl;

	std::vector<double> one {u.X(), u.Y(), u.Z(), 0};
	std::vector<double> two {v.X(), v.Y(), v.Z(), 0};
	std::vector<double> three {w.X(), w.Y(), w.Z(), 0};
	std::vector<double> four {0, 0, 0, 1};
	std::vector<std::vector<double>> all {one, two, three, four};
	elements = all;
	rows = 4;
	columns = 4;
}

void Transformer::scale(double x, double y, double z){
	update(Scale(x, y, z));
}

void Transformer::translate(double x, double y, double z){
	update(Translate(x, y, z));
}

void Transformer::axis_angle(double x, double y, double z, double theta){
	//Final axis angle matrix, z rotation matrix. Transpose the bases matrix to acquire inverse.
	Axisangle basesMatrix(x, y, z);
	std::cout << basesMatrix.toString() << std::endl;
	Zrotation rotationMatrix(theta);
	std::cout << rotationMatrix.toString() << std::endl;
	tMatrix invBases = basesMatrix.transpose();
	std::cout << invBases.toString() << std::endl;

	//Update the transformation matrix with the required axis angle matrices.
	tMatrix composed = invBases.multiply(rotationMatrix.multiply(basesMatrix));
	update(composed);
}

void Transformer::update(const tMatrix &next){
	tcomposition = next.multiply(tcomposition);
}

string Transformer::toString() const{
	return tcomposition.toString();
}








