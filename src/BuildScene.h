/*
 * BuildScene.h
 *
 *  Created on: Dec 23, 2016
 *      Author: colin
 */

#ifndef BUILDSCENE_H_
#define BUILDSCENE_H_

#include <string.h>

#include <string>
#include <vector>
#include <iostream>

#include "Scene.h"
#include "IO.h"
#include "MyExceptions.h"

using std::vector;
using std::string;

namespace buildScene {

//Parse all command-line arguments.
//Performs validity checking for everything except whether the supplied filenames
//can be opened.
bool parseArgs(int argc, char* argv[], int &protocol, vector<string> &modelFiles,
		string &camFile, string &outFile, string &mats);

//read all data and construct containing structures
bool build(Scene* &set, vector<string> modelFiles, string camFile, string outFile,
		string matFile);

} // namespace buildScene


#endif /* BUILDSCENE_H_ */
