/*
 * BuildScene.cpp
 *
 *  Created on: Dec 23, 2016
 *      Author: colin
 */

#include "BuildScene.h"

namespace sf{
	const bool SUCCESS = true;
	const bool FAILURE = false;
};

namespace buildScene {

// Not all arguments are required for all protocols. Error checking whether or not all
// required flags have been given occurs in the protocol checking functions.
// TODO: write protocol checking functions.
bool parseArgs(int argc, char* argv[], int &protocol, vector<string> &modelFiles,
		string &camFile, string &outFile, string &mats) {

	// Parses all command-line arguments.
	for (int i = 1; i < argc; i += 2) {

		// Protocol flag
		if (strcmp(argv[i], "-p") == 0) {

			try {
				protocol = std::stoi(argv[i + 1]);
			} catch (std::exception& e) {
				std::cout << e.what() << ": Protocol flag parse error" << std::endl;
				return sf::FAILURE;
			}

			try {
				if (protocol < 1 || protocol > 2) {
					throw pbexc;
				}
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				return sf::FAILURE;
			}

		// Camera flag
		} else if (strcmp(argv[i], "-cam") == 0) {
			camFile = argv[i + 1];

		//Model flag
		} else if (strcmp(argv[i], "-models") == 0) {
			int modIndex = i + 2; //Model filenames begin after the flag and count arguments.
			i += 1; //Model count is after flag.
			int count;

			// Parses model flag.
			try {
				count = std::stoi(argv[i]);
			} catch (std::exception& e) {
				std::cout << e.what() << ": Model count parse error" << std::endl;
				return sf::FAILURE;
			}

			// Makes sure there is at least one model file given
			try {
				if (count < 1) {
					throw mbexc;
				}
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				return sf::FAILURE;
			}

			// Stores all model filenames.
			try {
				for (int modPos = 0; modPos < count; modPos++) {
					modelFiles.push_back(string(argv[modIndex + modPos]));
				}
			} catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				return sf::FAILURE;
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
			std::cout << "Flag unrecognized: " << argv[i] << std::endl;
			return sf::FAILURE;
		}
	}
	return sf::SUCCESS;
}


//"scene" variable get loaded.
//Return value is error
bool build(Scene* &set, vector<string> modelFiles, string camFile, string outFile,
		string matFile){
	Camera camera;
	vector<Model*> models;
	vector<Light> lights;
	vector<Material> materials;

	//~~~Read all data~~~
	try{
		camera = tracerio::readCamera(camFile);
	} catch(...){
		return sf::FAILURE;
	}
	try{
		tracerio::readMaterials(lights, materials, matFile);
	} catch(...){
		return sf::FAILURE;
	}

	int modelCount = modelFiles.size();
	for (int i = 0; i < modelCount; i++) {
		try{
			string current = modelFiles.at(i);
			Model* next = tracerio::buildModel(current, i, materials);
			models.push_back(next);
		} catch(...){
			std::cerr << "Model " << i << std::endl;
			return sf::FAILURE;
		}
	}

	set = new Scene(camera, models, lights, materials);
	return sf::SUCCESS;
}

} // namespace buildScene
