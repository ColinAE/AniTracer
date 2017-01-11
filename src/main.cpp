/*
 * main.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: colin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

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
#include "BuildScene.h"

namespace freeAll{
void free(Scene* set){
	delete set;
}
} // namespace freeAll

int main(int argc, char *argv[]){
	string camFile, outFile, matFile;
	vector<string> modelFiles;
	int protocol;

	//Parse all command-line arguments.
	if(!buildScene::parseArgs(argc, argv, protocol, modelFiles, camFile, outFile, matFile)){
		cout << "Could not parse arguments" << endl;
		return -1;
	}

	//Build the scene
	Scene* set;
	if(!buildScene::build(set, modelFiles, camFile, outFile, matFile)){
		cout << "Could not build scene" << endl;
		return -1;
	}

	vector<RGB> colors;
	vector<sRGB> scaled;
	switch(protocol){
	case 1:
		ReadEval::REPL(set, outFile);
		break;
	case 2:
		colors = set->trace();
		scaled = colors::scale(colors);
		tracerio::writePPM(scaled, set->getCamera(), outFile);
		cout << colors.size() << endl;
		break;
	default:
		break;
	}

	//TODO: error checking depending on protocol


	freeAll::free(set);
}

