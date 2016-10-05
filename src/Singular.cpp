
#include "Singular.h"

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
	if(x == other.X() &&
			y == other.Y() &&
			z == other.Z()){
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
////

// Normal definitions
Vector Normal::normalize(double x, double y, double z){
	double mag = magnitude();
	return Vector(x/mag, y/mag, z/mag);
}

Normal::Normal(double x, double y, double z){
	Vector normal = normalize(x, y, z);
	x = normal.X();
	y = normal.Y();
	z = normal.Z();
}

Normal::Normal(const Vector &other) : Vector(){
	Vector normal = normalize(other.X(), other.Y(), other.Z());
	x = normal.X();
	y = normal.Y();
	z = normal.Z();
}

// This function computes
Normal::Normal(const std::vector<Vertex> &unconstrainedPolygon){
	Vector one = unconstrainedPolygon.at(0);
	Vector two= unconstrainedPolygon.at(1);
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
	string xyz = dtos(x) + " " + dtos(y) + " " + dtos(z);
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


