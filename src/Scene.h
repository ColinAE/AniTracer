/*
 * Model.h
 *
 *  Created on: Mar 13, 2016
 *      Author: colin
 */

// Contains the guts of the raytracer.
//

#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "Polygon.h"
#include "RGB.h"
#include "Singular.h"
#include "Transform.h"
#include "Camera.h"
#include "Utility.h"
#include "TraceTools.h"
#include "LightMaterial.h"

using std::string;
using std::ofstream;

class Model {
private:
	int vertexCount, faceCount;
	std::vector<Polygon> faces;
	string name;
	string properties; // Properties of the numerical values of each line.
	string vproperties; //Vertex properties.

public:
	Model(string in, int vertexCount, int faceCount, const std::vector<Polygon> &faces,
			string properties, string vproperties);
	string getName() { return name; }
	string getProperties() { return properties; }
	string getVProperties() { return vproperties; }
	int countVertices();
	int countFaces();
	int getOriginalVertexCount() { return vertexCount; } //TODO: Defunct, I think.
	int getOriginalFaceCount() { return faceCount; }
	string toString();
	void addFace(Polygon newface){ faces.push_back(newface); }
	void collide(const Ray &incoming, int &closestIndex, Polygon &closest, double distance);
	void update(tMatrix trans);
};

class Object {
public:
	virtual Collision collide(const Ray &) const = 0;
	virtual void update(tMatrix) = 0;
	virtual ~Object() = 0;
	virtual string toString() const = 0;
	virtual string getName() const = 0;
};

class Polyhedron : public Object {
private:
	Model* model;
	std::vector<Material> materials;
	bool hasMaterial = false;

	Material matchMaterial(int) const;
public:
	Polyhedron(Model *, const std::vector<Material> &, int);
	Polyhedron(const Polyhedron &);
	~Polyhedron();
	string getName() const { return model->getName(); }
	string toString() const;
	Collision collide(const Ray &) const;
	void update(tMatrix);

};

class Scene {
private:
	std::vector<Object *> objects;
	std::vector<Light> lights;
	Camera camera;

	RGB see(const Ray &ray);
public:
	Scene(const Camera &, const std::vector<Model*> &, const std::vector<Light> &, const std::vector<Material> &);
	Scene(const Scene &);
	void update(tMatrix, int);
	int objCount(){ return objects.size(); }
	Camera getCamera() const { return camera; }
	std::vector<std::vector<string>> toString();
	std::vector<RGB> trace();
};

namespace surfaceColor{
	RGB ambient(const Collision &collision, const Light &light);
	RGB specularDiffuse(const Collision &collision, const std::vector<Light> &lights);
};

#endif
