/*
 * IO.h
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

//Contains functions involved with input and output.

#ifndef IO_H
#define IO_H

#include "Singular.h"
#include "Scene.h"
#include "RGB.h"
#include "Camera.h"
#include "GeometricOperations.h"
#include "LightMaterial.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <exception>
#include <fstream>
#include <iostream>
using std::ifstream;
using std::exception;
using std::string;
using std::cout;
using std::endl;

namespace tracerio{

// Output some error message related to file read problems and then terminate the program.
void badfile(string errType);

// Read Camera
Camera readCamera(const string in);

// Read individual Camera points on the current line. Helper of readCamera.
Point camPoint(std::ifstream &camfile);

// Parse camera file line.
std::vector<double> camin(string usein, int bound);

// Read materials from the material file
void readMaterials(std::vector<Light> &lights, std::vector<Material> &materials,
		string materialFilename);

// Read PLY files, create a Model object on the heap, and return a pointer to it.
Model* buildModel(string modelFilename, int modnum,
		const std::vector<Material> &materials);

// Output a ply file.
void writePLY(Scene* set, string out);

//void writePPM(vector<RGB>, Camera, char *);

};

#endif

