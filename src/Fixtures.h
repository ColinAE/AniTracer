/*
 * Test.h
 *
 *  Created on: Mar 26, 2016
 *      Author: colin
 */

#ifndef TEST_H
#define TEST_H

#include <gtest/gtest.h>

#include "Transform.h"
#include "RGB.h"
#include "Scene.h"
#include "Singular.h"
#include "Polygon.h"
#include "IO.h"
#include <vector>

using std::string;

class VectorTest: public ::testing::Test{
public:
	VectorTest();
	void SetUp();
	void TearDown();
	~VectorTest();

	Vector left, right;
};

class VerticeTest: public ::testing::Test{
public:
	VerticeTest();
	void SetUp();
	void TearDown();
	~VerticeTest();

	Vertex first, second, third, fourth, fifth;
	std::vector<Vertex> vertices;
	Polygon poly;
};

class MatrixTest: public ::testing::Test{
public:
	MatrixTest();
	void SetUp();
	void TearDown();
	~MatrixTest();

	Matrix imatrix;
	std::vector<std::vector<double>> nelms;
	Matrix nmatrix;
};

class TransformTest: public ::testing::Test{
public:
	TransformTest();
	void SetUp();
	void TearDown();
	~TransformTest();

	Transformer transformer;
};


#endif
