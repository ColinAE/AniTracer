
#include "Singular.h"
#include <iostream>

const bool debug = false;

// Point definitions
Point::Point(){
	x = 0;
	y = 0;
	z = 0;
}

Point::Point(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(const Point &other){
	x = other.X();
	y = other.Y();
	z = other.Z();
}

bool Point::operator==(const Point &other) const{
	double tolerance = .00000001;
	double xdiff = abs(x - other.X());
	double ydiff = abs(y - other.Y());
	double zdiff = abs(z - other.Z());
	if(xdiff < tolerance &&	ydiff < tolerance && zdiff < tolerance){
		return true;
	}
	return false;
}
////


// Vector definitions

// TODO: I have little idea of what I was planning on doing with this constructor.
// I assume it was meant to be used during the ray tracing portion of the code, but it
// appears defunct.
Vector::Vector(const Point &origin, const Point &direction) : Point(){
	x = direction.X() - origin.X();
	y = direction.Y() - origin.Y();
	z = direction.Z() - origin.Z();
}

Vector Vector::operator+(const Vector &other) const{
	return Vector(x + other.X(),
				  y + other.Y(),
				  z + other.Z());
}

Vector Vector::operator-(const Vector &other) const{
	return Vector(x - other.X(),
				  y - other.Y(),
				  z - other.Z());
}

Vector Vector::operator*(double scalar) const{
	return Vector(x * scalar,
				  y * scalar,
				  z * scalar);
}

Vector Vector::operator/(double scalar) const{
	return Vector(x / scalar,
				  y / scalar,
				  z / scalar);
}

double Vector::magnitude() const{
	double ax = x * x;
	double ay = y * y;
	double az = z * z;
	return sqrt(ax + ay + az);
}

Vector Vector::cross(const Vector &other) const{
	if(debug) std::cout << "other: " << other.X() << " " << other.Y() << " " << other.Z() << std::endl;
	double a = (y * other.Z()) - (z * other.Y());
	double b = (z * other.X()) - (x * other.Z());
	double c = (x * other.Y()) - (y * other.X());
	return Vector(a, b, c);
}

double Vector::dot(const Vector &other) const{
	double a = x * other.X();
	double b = y * other.Y();
	double c = z * other.Z();
	return a + b + c;
}

double Vector::C(const Vector &other) const{
	return (this->dot(other) / other.dot(other));
}

Vector Vector::project(const Vector &other) const{
	return other * this->C(other);
}

////

// Normal definitions
Vector Normal::normalize(double x, double y, double z){
	double mag = Vector(x, y, z).magnitude();
	return Vector(x/mag, y/mag, z/mag);
}

/*

Normal::Normal(double x, double y, double z){
	Vector normal = normalize(x, y, z);
	this->x = normal.X();
	this->y = normal.Y();
	z = normal.Z();
}

Normal::Normal(const Vector &other) : Vector(){
	Vector normal = normalize(other.X(), other.Y(), other.Z());
	x = normal.X();
	y = normal.Y();
	z = normal.Z();
}
*/

// This function computes the face normal of a polygon that is not constrained
// by a C++ class.
Normal::Normal(const std::vector<Vertex> &unconstrainedPolygon){
	Vector one = unconstrainedPolygon.at(0);
	Vector two = unconstrainedPolygon.at(1);
	Vector three = unconstrainedPolygon.at(2);
	Vector left = two - one;
	Vector right = three - one;
	Vector direction = left.cross(right);
	x = direction.X();
	y = direction.Y();
	z = direction.Z();
}

// Format:
// x y z
string Vertex::toString() const{
	string xyz = myUtilities::dtos(x) + " " + myUtilities::dtos(y) + " " + myUtilities::dtos(z);
	return xyz;
}

Ray::Ray(){
	origin = Point(0, 0, 0);
	direction = Normal(1, 0, 0);
	limit = 20;
}

Ray::Ray(const Point &center, const Vector &direction, int limit){
	this->origin = center;
	this->direction = direction;
	this->limit = limit;
}

bool Ray::operator==(const Ray &other){
	return origin == other.getOrigin() && direction == other.getDirection();
}

// TODO: Reflect and refract methods need more explanation.
// The reflect and refract methods return rays with new origins and directions, along
// with this ray's limit minus one.
// Checking whether or not a ray should continue to be reflected or refracted after
// it has reached its limit is done elsewhere.
Ray Ray::reflect(const double &distance, const Normal &N) const{
	Vector V = direction * -1;
	Vector twoNV = N * abs(2 * V.dot(N));
	Vector reflectionDirection = twoNV - V;
	Vector newDirectionLength = direction * distance;
	Vector vOrigin = Vector(origin);
	Vector toNewOrigin = newDirectionLength + vOrigin;
	Point newOrigin = Point(toNewOrigin.X(), toNewOrigin.Y(), toNewOrigin.Z());
	return Ray(newOrigin, reflectionDirection, limit - 1);
}

Ray Ray::refract(const double &distance, const Normal &N) const {
	//TODO
}


