
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

//Normal Vector::normalize() const{
//	return Normal(this);
//}

//TODO: check this extensively
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

string Vertex::toString() const{
	string xyz = dtos(x) + " " + dtos(y) + " " + dtos(z) + "\n";
	return xyz;
}

Ray::Ray(){
	center = Point(0, 0, 0);
	direction = Normal(1, 0, 0);
}

Ray::Ray(const Point &center, const Vector &direction){
	this->center = center;
	this->direction = direction;
}

