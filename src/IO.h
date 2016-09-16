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
void badfile(string);

// Read Camera
Camera readCamera(const string);

// Read individual Camera points on the current line. Helper of readCamera.
Point camPoint(std::ifstream &);

// Parse camera file line.
std::vector<double> camin(string, int);

// Read materials from the material file
void readMaterials(std::vector<Light> &, std::vector<Material> &, string);

// Read PLY files, create a Model object on the heap, and return a pointer to it.
Model* buildModel(string, int, const std::vector<Material> &);

// Output a ply file.
void writePLY(Scene*, string);

//void writePPM(vector<RGB>, Camera, char *);
};

#endif

