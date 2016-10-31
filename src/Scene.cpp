/*
 * Model.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: colin
 */

#include "Scene.h"
#include <iostream>

const bool debug = true;

Model::Model(string in, int vertexCount, int faceCount, const std::vector<Polygon> &faces,
		string properties, string vproperties){
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

void Model::collide(const Ray &incoming, int &closestIndex, Polygon &closest, double distance){
	double smallestDist = std::numeric_limits<double>::max();
	double examine;
	int index = 0;
	std::for_each(faces.begin(), faces.end(), [&](Polygon &face){
		examine = face.collide(incoming);
		if(examine < smallestDist){
			smallestDist = examine;
			closestIndex = index;
			std::cout << "cindex " << closestIndex << std::endl;
		}
		index++;
	});
	if(debug){ std::cout << "face index" << std::endl;
	std::cout << "size: " << faces.size() << " -- index: " << closestIndex << std::endl;
	}
	closest = faces.at(closestIndex);
	if(debug) std::cout << "reached" << std::endl;
	distance = smallestDist;
}

void Model::update(tMatrix trans){
	std::for_each(faces.begin(), faces.end(), [&](Polygon &face){
		face.update(trans);
	});
}

Object::~Object(){}

Polyhedron::Polyhedron(Model* model, const std::vector<Material> &mats, int position){
	this->model = model;
	int matsSize = mats.size();
	materials.push_back(Material());
	for(int i = 0; i < matsSize; i++){
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

string Polyhedron::toString() const{
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

int Polyhedron::matchMaterial(int polyIndex) const{
	int beginning;
	int ending;
	int index = 1;
	std::for_each(materials.begin(), materials.end(), [&](const Material &material){
		beginning = material.beginning();
		ending = material.ending();
		if(beginning <= polyIndex && polyIndex <= ending){
			return index;
		}
	});
	return 0;
}

Collision Polyhedron::collide(const Ray &incoming) const{
	int closestIndex = 0;
	Polygon closest = Polygon();
	double distance = -1;
	model->collide(incoming, closestIndex, closest, distance);
	if(distance < 0){
		return Collision();
	} else {
		return Collision(incoming, distance, closest.normal(), materials.at(matchMaterial(closestIndex)));
	}
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
		objects.push_back(new Polyhedron(current, objMats, i));
	}
	std::cout << "object size: " << objects.size() << std::endl;
}


Scene::Scene(const Scene &other){
	this->objects = other.objects;
	this->lights = other.lights;
	this->camera = other.camera;
}

void Scene::update(tMatrix trans, int objnum){
	objects.at(objnum)->update(trans);
}

std::vector<std::vector<string>> Scene::toString(){
	std::vector<std::vector<string>> allInfo;
	std::for_each(objects.begin(), objects.end(), [&](const Object *object){
		std::vector<string> objInfo;
		string name = object->getName();
		string modelBody = object->toString();
		objInfo.push_back(name);
		objInfo.push_back(modelBody);
		allInfo.push_back(objInfo);
	});
	return allInfo;
}

RGB Scene::calcAmbient(const Collision &collision, const Light &light) const{
	Material mat = collision.collisionMaterial();
	RGB brightness = light.color();
	double r = mat.lone() * brightness.red();
	double g = mat.ltwo() * brightness.green();
	double b = mat.lthree() * brightness.blue();
	return RGB(r, g, b);
}

RGB Scene::calcSpecularDiffuse(const Collision &collision){
	RGB ret = RGB();
	std::for_each(lights.begin(), lights.end(), [&](const Light &light){
		Normal L = Vector(light.getPosition()) - Vector(collision.getPosition());
		Normal surfaceNorm = collision.getNormal();
		double LN = L.dot(surfaceNorm);
		if(LN < 0){
			return;
		}

		//specular
		Material material = collision.collisionMaterial();
		double theta = surfaceNorm.dot(L);
		Vector R = surfaceNorm * (2 * abs(LN)) - L;
		Vector V = collision.getV();
		double upShine = pow(V.dot(R), material.shininess());
		double rConst = material.rConst();
		RGB specular = light * rConst;

		//diffuse
		RGB brightness = light.color();
		double r = material.lone() * brightness.red();
		double g = material.ltwo() * brightness.green();
		double b = material.lthree() * brightness.blue();
		RGB diffuse = RGB(r, g, b) * LN;

		//TODO: set defaults of material class
		//TODO: check calculations

		ret = ret + ((specular * upShine) + diffuse);
	});
	return ret;
}

RGB Scene::see(const Ray &ray){
	if(ray.reachedLimit()){
		return RGB();
	}
	Collision current = Collision();
	Collision examine;
	std::for_each(objects.begin(), objects.end(), [&](const Object *object){
		examine = object->collide(ray);
		if(current > examine){
			current = examine;
		}
	});
	Light ambientLight;
	std::for_each(lights.begin(), lights.end(), [&](const Light &light){
		if(!light.hasPosition()){
			ambientLight = light;
			return;
		}
	});
	RGB ambient = calcAmbient(current, ambientLight);
	RGB specular = calcSpecularDiffuse(current);

	//RGB reflection = see(current.reflect());
	//RGB refraction = see(current.refract());
	return ambient + specular;
}

std::vector<RGB> Scene::trace(){
	std::vector<Ray> rays = camera.shootAll();
	std::vector<RGB> colors;
	if(debug) std::cout << "Rays created. Beginning tracing." << std::endl;
	std::for_each(rays.begin(), rays.end(), [&](Ray ray){
		colors.push_back(see(ray));
	});
	return colors;
}
