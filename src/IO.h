/*
 * IO.h
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

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

void badfile(string);
Camera readCamera(const string);
Point camPoint(std::ifstream &);
std::vector<double> camin(string, unsigned int);
void readMaterials(std::vector<Light> &, std::vector<Material> &, string);
Model* readPLY(string, int, const std::vector<Material> &);
void writePLY(Scene*, string);
//void writePPM(vector<RGB>, Camera, char *);

#endif

