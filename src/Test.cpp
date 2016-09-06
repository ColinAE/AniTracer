/*
 * Test.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: colin
 */

#include <iostream>
#include <cmath>
#include "Fixtures.h"

const double tolerance = .00000001;

bool tolerates(double left, double right){
	if(abs(left - right) < tolerance){
		return true;
	} else {
		return false;
	}
}

//Test all Point class functionality.
TEST(PointTest, PointBasic){
	Point def = Point();
	Point tpoint = Point(1, 2, 3);
	Point cpoint = Point(tpoint);

	//Test default constructor.
	EXPECT_EQ(0, def.X());
	EXPECT_EQ(0, def.Y());
	EXPECT_EQ(0, def.Z());

	//Test both non-default constructors and x, y, and z getters.
	EXPECT_TRUE(tolerates(tpoint.X(), cpoint.X()));
	EXPECT_TRUE(tolerates(tpoint.Y(), cpoint.Y()));
	EXPECT_TRUE(tolerates(tpoint.Z(), cpoint.Z()));

	//Test == operator.
	EXPECT_TRUE(tpoint == cpoint);
}

//Test Vector constructors.
TEST(VectorTest, Constructors){
	Vector def = Vector();
	Vector tvec = Vector(1, 2, 3);
	Vector cvec = Vector(tvec);
	Vector pvec = Vector(Point(1, 2, 3));

	//Test default constructor
	EXPECT_EQ(0, def.X());
	EXPECT_EQ(0, def.Y());
	EXPECT_EQ(0, def.Z());

	//Test both coordinate constructor and copy constructor.
	EXPECT_TRUE(tolerates(tvec.X(), cvec.X()));
	EXPECT_TRUE(tolerates(tvec.X(), cvec.X()));
	EXPECT_TRUE(tolerates(tvec.Z(), cvec.Z()));

	//Test construct-with-Point constructor
	EXPECT_TRUE(tolerates(1, pvec.X()));
	EXPECT_TRUE(tolerates(2, pvec.Y()));
	EXPECT_TRUE(tolerates(3, pvec.Z()));

}


//Test Vector addition.
TEST_F(VectorTest, Addition){
	Vector examine = left + right;
	EXPECT_TRUE(tolerates(5, examine.X()));
	EXPECT_TRUE(tolerates(7, examine.Y()));
	EXPECT_TRUE(tolerates(8, examine.Z()));
}

//Test Vector subtraction.
TEST_F(VectorTest, Subtraction){
	Vector examine = left - right;
	EXPECT_TRUE(tolerates(-3, examine.X()));
	EXPECT_TRUE(tolerates(-3, examine.Y()));
	EXPECT_TRUE(tolerates(-3, examine.Z()));
}

//Test Vector multiplication.
TEST_F(VectorTest, Multiplication){
	Vector examine = left * 3;
	EXPECT_TRUE(tolerates(3, examine.X()));
	EXPECT_TRUE(tolerates(6, examine.Y()));
	EXPECT_TRUE(tolerates(9, examine.Z()));
	examine = right * 6;
	EXPECT_TRUE(tolerates(24, examine.X()));
	EXPECT_TRUE(tolerates(30, examine.Y()));
	EXPECT_TRUE(tolerates(36, examine.Z()));
}

//Test Vector division.
TEST_F(VectorTest, Division){
	Vector examine = left / 3;
	EXPECT_TRUE(tolerates(.3333, examine.X()));
	EXPECT_TRUE(tolerates(.6666, examine.Y()));
	EXPECT_TRUE(tolerates(1, examine.Z()));
	examine = right / 4;
	EXPECT_TRUE(tolerates(1, examine.X()));
	EXPECT_TRUE(tolerates(1.25, examine.Y()));
	EXPECT_TRUE(tolerates(1.5, examine.Z()));
}

//Test Vector magnitude method.
TEST_F(VectorTest, Magnitude){
	double examine = left.magnitude();
	EXPECT_TRUE(tolerates(3.7416, examine));
	examine = right.magnitude();
	EXPECT_TRUE(tolerates(3.7416, examine));
}

//Test Vector cross product method.
TEST_F(VectorTest, CrossProduct){
	Vector crossed = left.cross(right);
	EXPECT_EQ(-3, crossed.X());
	EXPECT_EQ(6, crossed.Y());
	EXPECT_EQ(-3, crossed.Z());
}

//Test Vector dot product method.
TEST_F(VectorTest, DotProduct){
	double examine = left.dot(right);
	EXPECT_TRUE(tolerates(32, examine));
}

//
//Test Normal constructors.
//The Normal constructors are more complex than other singular types.
//The Normal constructors compute the normalized vector of an argument Vector and set their own fields
//accordingly.
TEST(NormalTest, Constructors){
	Normal def = Normal(); //Construct with default.
	Normal copy = Normal(Vector(1, 2, 3));

	EXPECT_TRUE(tolerates(1, def.X()));
	EXPECT_TRUE(tolerates(0, def.Y()));
	EXPECT_TRUE(tolerates(0, def.Z()));

	EXPECT_TRUE(tolerates(1/sqrt(14), copy.X()));
	EXPECT_TRUE(tolerates(sqrt(2/7), copy.Y()));
	EXPECT_TRUE(tolerates(3/sqrt(14), copy.Z()));
}

//Test Vertex constructors.
/*
 * TODO: Vertex constructors are defined in terms of Point constructors. I will return to complete this test case later.
 */

//

//Test Screen functionality.
TEST(ScreenTest, All){
	Screen def = Screen();
	Screen gar(1, 2, 3, 4);
	Screen copy = gar;

	//Test default constructor.
	EXPECT_EQ(0, gar.getlowu());
	EXPECT_EQ(0, gar.getlowv());
	EXPECT_EQ(0, gar.gethighu());
	EXPECT_EQ(0, gar.gethighv());

	//Test constructor.
	EXPECT_EQ(1, gar.getlowu());
	EXPECT_EQ(2, gar.getlowv());
	EXPECT_EQ(3, gar.gethighu());
	EXPECT_EQ(4, gar.gethighv());

	//Test copy constructor.
	EXPECT_EQ(1, copy.getlowu());
	EXPECT_EQ(2, copy.getlowv());
	EXPECT_EQ(3, copy.gethighu());
	EXPECT_EQ(4, copy.gethighv());
}

//Test Camera constructors.
TEST(CameraTest, Constructors){
	Camera def = Camera();
	Camera cam = Camera(Point(), Point(1, 1, 1), Vector(2, 2, 2), 0, Screen());

	EXPECT_TRUE(Point() == cam.getFocalPoint());
	EXPECT_TRUE(Point(1, 1, 1) == cam.getLookat());
	EXPECT_TRUE(Vector(2, 2, 2) == cam.getVUP());
	EXPECT_TRUE(0 == cam.getFocalLength());
	EXPECT_TRUE(Screen().getlowu() == cam.getScreen().getlowu());
}

TEST_F(VerticeTest, Centroid){
	Point centroid = geops::centroid(vertices);

	EXPECT_TRUE(tolerates(.5, centroid.X()));
	EXPECT_TRUE(tolerates(.5, centroid.Y()));
	EXPECT_TRUE(tolerates(0, centroid.Z()));
}

TEST_F(VerticeTest, PolygonConstructors){
	EXPECT_EQ(4, poly.getVertexCount());
}

TEST_F(VerticeTest, PolygonAccessors){
	EXPECT_TRUE(geops::centroid(vertices) == poly.centroid());
	EXPECT_TRUE(first == poly[0]);
	EXPECT_TRUE(second == poly[1]);
	EXPECT_TRUE(third == poly[2]);
	EXPECT_TRUE(fourth == poly[3]);
}

TEST_F(VerticeTest, PolygonNormal){
	Normal pnorm = poly.normal();

	EXPECT_EQ(0, pnorm.X());
	EXPECT_EQ(0, pnorm.Y());
	EXPECT_EQ(1, pnorm.Z());
}

TEST_F(VerticeTest, TriangularizeSquare){
	std::vector<Polygon> triangles = geops::triangularize(poly);
	EXPECT_EQ(2, triangles.size());
	Polygon one = triangles.at(0);
	Polygon two = triangles.at(1);

	EXPECT_TRUE(first == one[0]);
	EXPECT_TRUE(second == one[1]);
	EXPECT_TRUE(third == one[2]);

	EXPECT_TRUE(second == one[0]);
	EXPECT_TRUE(third == one[1]);
	EXPECT_TRUE(fourth == one[2]);
}

TEST_F(VerticeTest, TriangularizePolygon){
	vertices.push_back(fifth);
	poly = Polygon(vertices);
	Point centroid = poly.centroid();
	std::vector<Polygon> triangles = geops::triangularize(poly);

	EXPECT_EQ(5, triangles.size());
	Polygon one = triangles.at(0);
	Polygon two = triangles.at(1);
	Polygon three = triangles.at(2);
	Polygon four = triangles.at(3);
	Polygon five = triangles.at(4);

	EXPECT_TRUE(first == one[0]);
	EXPECT_TRUE(second == one[1]);
	EXPECT_TRUE(centroid == one[2]);

	EXPECT_TRUE(second == two[0]);
	EXPECT_TRUE(third == two[1]);
	EXPECT_TRUE(centroid == two[2]);

	EXPECT_TRUE(third == three[0]);
	EXPECT_TRUE(fourth == three[1]);
	EXPECT_TRUE(centroid == three[2]);

	EXPECT_TRUE(fourth == four[0]);
	EXPECT_TRUE(fifth == four[1]);
	EXPECT_TRUE(centroid == four[2]);

	EXPECT_TRUE(fifth == five[0]);
	EXPECT_TRUE(first == five[1]);
	EXPECT_TRUE(centroid == five[2]);
}

TEST(RGBTest, All){
	RGB one(1, 2, 3);
	RGB two(4, 5, 6);

	EXPECT_EQ(1, one.red());
	EXPECT_EQ(2, one.green());
	EXPECT_EQ(3, one.blue());

	RGB plus = one + two;
	EXPECT_EQ(5, plus.red());
	EXPECT_EQ(7, plus.green());
	EXPECT_EQ(9, plus.blue());
}

TEST_F(MatrixTest, Constructors){
	Matrix test = Matrix(nmatrix);
	EXPECT_TRUE(test == nmatrix);
}

TEST_F(MatrixTest, At){
	EXPECT_EQ(1, imatrix.at(0, 0));
	EXPECT_EQ(1, imatrix.at(1, 1));
	EXPECT_EQ(0, imatrix.at(1, 2));
	EXPECT_EQ(3, imatrix.at(3, 2));
}

TEST_F(MatrixTest, Multiply){
	Matrix examine = imatrix.multiply(imatrix);
	EXPECT_TRUE(examine == imatrix);

	examine = nmatrix.multiply(imatrix);
	EXPECT_TRUE(examine == nmatrix);

	std::vector<double> one {90, 100, 110, 120};
	std::vector<double> two {202, 228, 254, 280};
	std::vector<double> three {314, 356, 398, 440};
	std::vector<double> four {426, 484, 542, 600};
	std::vector<std::vector<double>> nums {one, two, three, four};
	Matrix final(nums);
	examine = nmatrix.multiply(nmatrix);
	EXPECT_TRUE(final == examine);
}

//TODO: test vmultiply

TEST_F(MatrixTest, Transpose){
	Matrix examine = imatrix.transpose();
	EXPECT_TRUE(examine == imatrix);

	std::vector<double> one {1, 5, 9, 13};
	std::vector<double> two {2, 6, 10, 14};
	std::vector<double> three {3, 7, 11, 15};
	std::vector<double> four {4, 8, 12, 16};
	std::vector<std::vector<double>> nums {one, two, three, four};
	Matrix final(nums);
	examine = nmatrix.transpose();
	EXPECT_TRUE(final == examine);

	examine = nmatrix.transpose().transpose();
	EXPECT_TRUE(examine == nmatrix);
}

TEST_F(MatrixTest, Accessors){
	std::vector<double> examine {1, 2, 3, 4};
	EXPECT_TRUE(examine == nmatrix.row(0));
	examine = nmatrix.getElms();
	EXPECT_TRUE(examine == nelms);
	EXPECT_EQ(4, nmatrix.rowCount());
	EXPECT_EQ(4, nmatrix.colCount());
}

TEST_F(TransformTest, Constructors){
	std::vector<double> one {1, 0, 0, 0};
	std::vector<double> two {0, 1, 0, 0};
	std::vector<double> three {0, 0, 1, 0};
	std::vector<double> four {0, 0, 0, 1};
	std::vector<std::vector<double>> ielms {one, two, three, four};
	tMatrix imatrix(Matrix(ielms));

	EXPECT_TRUE(imatrix == transformer.transformationMatrix());
}

TEST_F(MatrixTest, Identity){
	EXPECT_TRUE(imatrix == Identity());
}

TEST(TMatrixTest, AxisAngle){
	std::vector<double> u {0, -(1/sqrt(2)), 0, 0};
	std::vector<double> v {0, 0, -(1/sqrt(2)), 0};
	std::vector<double> w {1, 0, 0, 0};
	std::vector<double> last {0, 0, 0, 1};
	std::vector<std::vector<double>> composed {u, v, w, last};
	Matrix test(composed);

	Axisangle examine(1, 0, 0);
	EXPECT_TRUE(examine == test);
}

TEST(TmatrixTest, Zrotation){
	double theta = 90;
	std::vector<double> one {cos(theta), -sin(theta), 0, 0};
	std::vector<double> two {sin(theta), cos(theta), 0, 0};
	std::vector<double> three {0, 0, 1, 0};
	std::vector<double> four {0, 0, 0, 1};
	std::vector<std::vector<double>> elms {one, two, three, four};
	Matrix test(elms);

	Zrotation examine(theta);
	EXPECT_TRUE(test == examine);
}

TEST_F(TransformTest, AxisAngle){
	transformer.axis_angle(1, 0, 0, 90);
	std::vector<double> u {1, 0, 0, 0};
	std::vector<double> v {0, -.22404, -.447, 0};
	std::vector<double> w {0, 44700, -.22404, 0};
	std::vector<double> last {0, 0, 0, 1};
	std::vector<std::vector<double>> composed {u, v, w, last};

	EXPECT_TRUE(composed == transformer.transformationMatrix());
}

TEST_F(TransformTest, ScaleByTranslate){
	transformer.scale(2, 2, 2);
	transformer.translate(5, 5, 5);
	std::vector<double> one {2, 0, 0, 5};
	std::vector<double> two {0, 2, 0, 5};
	std::vector<double> three {0, 0, 2, 5};
	std::vector<double> four {0, 0, 0, 2};
	std::vector<std::vector<double>> elms {one, two, three, four};
	tMatrix composed(Matrix(elms));

	EXPECT_TRUE(composed == transformer.transformationMatrix());
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
