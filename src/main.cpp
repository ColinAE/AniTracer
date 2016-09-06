/*
 * main.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: colin
 */
#include "IO.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <math.h>

#include "Fixtures.h"
#include "GeometricOperations.h"
#include "IO.h"
#include "Scene.h"
#include "Polygon.h"
#include "REPL.h"
#include "RGB.h"
#include "Screen.h"
#include "Singular.h"
#include "Transform.h"
#include "MyExceptions.h"

//
int parseArgs(int, char**, int &, vector<string> &, string &, string &, string &);
int buildScene(Scene* &, vector<string>, string, string, string);

int parseArgs(int argc, char* argv[], int &protocol, vector<string> &modelFiles, string &camFile, string &outFile, string &mats){

	try {
		for (int i = 1; i < argc; i += 2) {

			//Protocol flag
			if (strcmp(argv[i], "-p") == 0) {

				try {
					protocol = std::stoi(argv[i + 1]);
				} catch (exception& e) {
					cout << e.what() << endl;
					return 0;
				}

				try {
					if (protocol < 0 || protocol > 1) {
						throw pbexc;
					}
				} catch (exception& e) {
					cout << e.what() << endl;
					return 0;
				}

				//Camera flag
			} else if (strcmp(argv[i], "-cam") == 0) {
				camFile = argv[i + 1];

				//Model flag
			} else if (strcmp(argv[i], "-models") == 0) {
				int modIndex = i + 2; //Model filenames begin after the flag and count arguments.
				i += 1; //Model count is after flag.
				int count;

				try {
					count = std::stoi(argv[i]);
					for (int modPos = 0; modPos < count; modPos++) {
						modelFiles.push_back(string(argv[modIndex + modPos]));
					}
				} catch (exception& e) {
					cout << e.what() << endl;
					return 0;
				}

				try {
					if (count < 1) {
						throw mbexc;
					}
				} catch (exception& e) {
					cout << e.what() << endl;
					return 0;
				}

				i += count - 1; //Advance incrementer two positions behind the next argument

				//Materials flag
			} else if (strcmp(argv[i], "-mats") == 0) {
				mats = argv[i + 1];
				//No error checking

				//Output flag
			} else if (strcmp(argv[i], "-out") == 0) {
				outFile = argv[i + 1];
				//No error checking

			} else {
				throw flagexc;
			}
		}
	} catch (exception& e) {
		cout << e.what() << endl;
		return 0;
	}
	return 1;
}

int buildScene(Scene* &scene, vector<string> modelFiles, string camFile, string outFile, string matFile){
	Camera camera;
	vector<Model*> models;
	vector<Light> lights;
	vector<Material> materials;

	//Read all data
	camera = readCamera(camFile);
	readMaterials(lights, materials, matFile);

	int modelCount = modelFiles.size();
	for (int i = 0; i < modelCount; i++) {
		string current = modelFiles.at(i);
		Model* next = readPLY(current, i, materials);
		models.push_back(next);
	}

	scene = new Scene(camera, models, lights, materials);
	return 1;
}

void freeAll(Scene* set){
	delete set;
}

int main(int argc, char *argv[]){
	string camFile, outFile, matFile;
	vector<string> modelFiles;
	int protocol;

	//Parse all command-line arguments.
	//Performs validity checking for everything except whether the supplied filenames
	//can be opened.
	if(!parseArgs(argc, argv, protocol, modelFiles, camFile, outFile, matFile)){
		cout << "Could not parse arguments" << endl;
		return -1;
	}

	Scene* set;
	if(true == false){
		buildScene(set, modelFiles, camFile, outFile, matFile);
		delete set;
		return -1;
	}
	if(!buildScene(set, modelFiles, camFile, outFile, matFile)){
		cout << "Could not build scene" << endl;
		return -1;
	}

	cout << set->objCount() << endl;

	switch(protocol){
	case 1:
		ReadEval::REPL(set, outFile);
		break;
	case 2:

	}

	//TODO: error checking depending on protocol


	freeAll(set);
}

