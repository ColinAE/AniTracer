/*
 * Model.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: colin
 */

#include "Scene.h"
#include <iostream>

Model::Model(string in, int vertexCount, int faceCount, const std::vector<Polygon> &faces, string properties, string vproperties){
	this->vertexCount = vertexCount;
	this->faceCount = faceCount;
	this->name = in;
	this->faces = faces;
	this->properties = properties;
	this->vproperties = vproperties;
}

int Model::countVertices(){
	int runsum = 0;
	std::for_each(faces.begin(), faces.end(), [&](Polygon face){
		runsum += face.getVertexCount();
	});
	return runsum;
}

int Model::countFaces(){
	return faces.size();
}

string Model::toString(){
	string collectVertices = "";
	string collectFaces = "";
	int vertInd = 0;
	int faceInd = 0;
	std::for_each(faces.begin(), faces.end(), [&](Polygon face){
		int previous = vertInd;
		collectVertices += face.toString();
		vertInd += face.getVertexCount();
		collectFaces += itos(face.getVertexCount()) + " ";
		for(int i = previous; i < vertInd; i++){
			collectFaces += itos(i) + " ";
		}
		collectFaces += "\n";
		faceInd++;
	});
	collectVertices += collectFaces;
	return collectVertices;
}

void Model::update(tMatrix trans){
	std::for_each(faces.begin(), faces.end(), [&](Polygon &face){
		face.update(trans);
	});
}

Light::Light(double red, double green, double blue, double x, double y, double z, bool notAmbient){
	rgb = RGB(red, green, blue);
	position = Point(x, y, z);
	this->notAmbient = notAmbient;
}

Material::Material(int mindex, int begin, int end, double lambdaOne, double lambdaTwo, double lambaThree, double reflectance, double shininess, double tranlucence){
	this->mindex = mindex;
	this->begin = begin;
	this->end = end;
	this->lambdaOne = lambdaOne;
	this->lambdaTwo = lambdaTwo;
	this->lambdaThree = lambdaThree;
	k = reflectance;
	alpha = shininess;
	this->translucence = translucence;
}

Polyhedron::Polyhedron(Model* model, const std::vector<Material> &mats, int position){
	this->model = model;
	for(unsigned int i = 0; i < mats.size(); i++){
		Material current = mats.at(i);
		if(current.model() == position){
			materials.push_back(current);
			hasMaterial = true;
		}
	}
}

Polyhedron::Polyhedron(const Polyhedron &source){
	model = new Model(*(source.model));
	materials = source.materials;
	hasMaterial = source.hasMaterial;
}

string Polyhedron::toString(){
	int vertexCount = model->countVertices();
	int faceCount = model->countFaces();
	string body = "";
	body += "ply\n";
	body += "format ascii 1.0\n";
	body += "element vertex " + itos(vertexCount) + "\n";
	body += model->getVProperties() + "\n";
	body += "element face " + itos(faceCount) + "\n";
	body += model-> getProperties() + "\n";
	body += "end_header\n";
	body += model->toString();
	return body;
}

Polyhedron::~Polyhedron(){
	delete model;
}

bool Polyhedron::collide(const Ray &ray){

}

void Polyhedron::update(tMatrix trans){
	model->update(trans);
}

Scene::Scene(const Camera &camera, const std::vector<Model*> &models, const std::vector<Light> &lights, const std::vector<Material> &materials){
	this->lights = lights;
	this->camera = camera;
	for(int i = 0; i < int(models.size()); i++){
		Model* current = models.at(i);
		std::vector<Material> objMats;
		int matSize = materials.size();
		for(int j = 0; j < matSize; j++){
			int modIndex = materials.at(i).model();
			if(modIndex == j){
				objMats.push_back(materials.at(j));
			}
		}
		objects.push_back(Polyhedron(current, objMats, i));
	}
	std::cout << "object size: " << objects.size() << std::endl;
}

Scene::Scene(const Scene &other){
	this->objects = other.objects;
	this->lights = other.lights;
	this->camera = other.camera;
}

void Scene::update(tMatrix trans, int objnum){
	objects.at(objnum).update(trans);
}

std::vector<std::vector<string>> Scene::toString(){
	std::vector<std::vector<string>> allInfo;
	std::for_each(objects.begin(), objects.end(), [&](Object obj){
		std::vector<string> objInfo;
		string name = obj.getName();
		string modelBody = obj.toString();
		objInfo.push_back(name);
		objInfo.push_back(modelBody);
		allInfo.push_back(objInfo);
	});
	return allInfo;
}

RGB Scene::see(const Ray &ray){
	std::vector<Collision> all;
	std::for_each(objects.begin(), objects.end(), [&](Object object){
		all.push_back(object.collide(ray));
	});
	Collision closest = closestCollision(all);
	return color(closest);
}

Collision closestCollision(const std::vector<Collision> &all){
	Collision lowest;
	std::for_each(all.begin(), all.end(), [&](Collision collision){

	});
}

RGB color(const Collision &closest){

}

std::vector<RGB> Scene::trace(){
	std::vector<Ray> rays = camera.shootAll();
	std::vector<RGB> colors;
	std::for_each(rays.begin(), rays.end(), [&](Ray ray){
		colors.push_back(see(ray));
	});
	return colors;
}
