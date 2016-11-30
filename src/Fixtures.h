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

	/* These three functions commented out to serve as a reminder of the other
	 * test figure methods.
	void SetUp();
	void TearDown();
	~VectorTest();
	*/

	Vector left, right;
};

class VerticeTest: public ::testing::Test{
public:
	VerticeTest();
	void SetUp();

	Vertex first, second, third, fourth, fifth;
	std::vector<Vertex> vertices;
	Polygon poly;
};

class MatrixTest: public ::testing::Test{
public:
	void SetUp();

	Matrix imatrix;
	std::vector<std::vector<double>> nelms;
	Matrix nmatrix;
};

class TransformTest: public ::testing::Test{
public:
	void SetUp();

	Transformer transformer;
};

class ColorTest: public ::testing::Test{
public:
	ColorTest();

	Collision collision;
	Light light;
};

class CollisionTest: public ::testing::Test{
public:
	CollisionTest();

	Ray ray;
	Polygon face;

};


#endif
