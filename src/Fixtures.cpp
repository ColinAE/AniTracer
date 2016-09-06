/*
 * Fixtures.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: colin
 */

#include "Fixtures.h"

VectorTest::VectorTest(){
	left = Point(1, 2, 3);
	right = Point(4, 5, 6);
}

VerticeTest::VerticeTest(){
	poly = Polygon();
}

void VerticeTest::SetUp(){

	//Initialize vertices vector to points that make up a 1 x 1 square on the xy plane.
	first = Vertex(0, 0, 0);
	second = Vertex(1, 0, 0);
	third = Vertex(1, 1, 0);
	fourth = Vertex(0, 1, 0);
	fifth = Vertex(-1, .5, 0);
	std::vector<Vertex> all {first, second, third, fourth};
	vertices = all;

	poly = Polygon(vertices);
}

void MatrixTest::SetUp(){
	std::vector<double> one {1, 0, 0, 0};
	std::vector<double> two {0, 1, 0, 0};
	std::vector<double> three {0, 0, 1, 0};
	std::vector<double> four {0, 0, 0, 1};
	std::vector<std::vector<double>> ielms {one, two, three, four};
	imatrix = Matrix(ielms);

	std::vector<double> twoone {1, 2, 3, 4};
	std::vector<double> twotwo {5, 6, 7, 8};
	std::vector<double> twothree {9, 10, 11, 12};
	std::vector<double> twofour {13, 14, 15, 16};
	std::vector<std::vector<double>> nums {twoone, twotwo, twothree, twofour};
	nmatrix = Matrix(nums);
	nelms = nums;
}

void TransformTest::SetUp(){
	transformer = Transformer();
}

