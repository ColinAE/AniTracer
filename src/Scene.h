/*
 * Model.h
 *
 *  Created on: Mar 13, 2016
 *      Author: colin
 */


#ifndef OBJECTS_H
#define OBJECTS_H

#include "Polygon.h"
#include "RGB.h"
#include "Singular.h"
#include "Transform.h"
#include "Camera.h"
#include "Utility.h"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

using std::string;
using std::ofstream;

class Model {
private:
	int vertexCount, faceCount;
	std::vector<Polygon> faces;
	string name;
	string properties;
	string vproperties;
public:
	Model(string, int, int, const std::vector<Polygon> &, string, string);
	string getName() { return name; }
	string getProperties() { return properties; }
	string getVProperties() { return vproperties; }
	int countVertices();
	int countFaces();
	int getOriginalVertexCount() { return vertexCount; }
	int getOriginalFaceCount() { return faceCount; }
	string toString();
	void addFace(Polygon newface){ faces.push_back(newface); }
	gar collide(const Ray &);
	void update(tMatrix);
};

class Light {
private:
	RGB rgb = RGB(-1, -1, -1);
	Point position;
	bool notAmbient;
public:
	Light(double, double, double, double, double, double, bool);
	RGB color() const { return rgb; }
	Point getPosition() const { return position; }
	bool hasPosition() const { return notAmbient; }
};

class Material {
private:
	int mindex, begin, end;
	double lambdaOne, lambdaTwo, lambdaThree;
	double k, alpha, translucence;
public:
	Material(int, int, int, double, double, double, double, double, double);
	int model() const { return mindex; }
	int beginning() const { return begin; }
	int ending() const { return end; }
	double lone() const { return lambdaOne; }
	double ltwo() const { return lambdaTwo; }
	double lthree() const {return lambdaThree; }
	double rConst() const { return k; }
	double shininess() const { return alpha; }
	double translucency() const { return translucence; }
};

class Object {
public:
	virtual Collision collide();
	virtual void update(tMatrix);
	virtual ~Object();
	virtual string toString();
};

class Polyhedron : public Object {
private:
	Model* model;
	std::vector<Material> materials;
	bool hasMaterial = false;

public:
	Polyhedron(Model *, const std::vector<Material> &, int);
	Polyhedron(const Polyhedron &);
	virtual ~Polyhedron();
	string getName() { return model->getName(); }
	string toString();
	virtual Collision collide(const Ray &);
	void update(tMatrix);


};

class Scene {
private:
	std::vector<Object> objects;
	std::vector<Light> lights;
	Camera camera;

	RGB see(const Ray &);
	Collision closestCollision(const std::vector<Collision> &);
	RGB color(const Collision &);
public:
	Scene(const Camera &, const std::vector<Model*> &, const std::vector<Light> &, const std::vector<Material> &);
	Scene(const Scene &);
	void update(tMatrix, int);
	int objCount(){ return objects.size(); }
	std::vector<std::vector<string>> toString();
	std::vector<RGB> trace();
};

#endif
