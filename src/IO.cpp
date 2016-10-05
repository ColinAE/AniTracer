/*
 * IO.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

#include "IO.h"

void tracerio::badfile(string errType){
	cout << "Error: " + errType + "\nExiting." << endl;
	exit(EXIT_FAILURE);
}

//TODO: ERROR CHECKING
//Build Camera object
Camera tracerio::readCamera(const string in){

	//Open File
	ifstream camfile(in, ifstream::in);
	if(!camfile.good()){
		badfile(in);
	}

	string line; // Storage for read lines.

	//Get camera attributes
	Point focalPoint = camPoint(camfile);
	Point lookat = camPoint(camfile);
	Vector vup(camPoint(camfile));

	//Get focal length
	getline(camfile, line);
	double flength = camin(line, 1)[0];

	//Get screen size
	getline(camfile, line);
	std::vector<double> scr = camin(line, 4);
	Screen screen(scr[0], scr[1], scr[2], scr[3]);

	camfile.close();

	return Camera(focalPoint, lookat, vup, flength, screen);
}

//Read camera points
Point tracerio::camPoint(ifstream &camfile){
	string line;
	getline(camfile, line);
	std::vector<double> focalPoint = camin(line, 3);
	Point point(focalPoint[0], focalPoint[1], focalPoint[2]);
	return point;
}

// Parse camera file line.
// Meant to read lines containing variable quantities of attributes.
// Namely, it reads the focal length on line 4 and the four screen bounds on line 5
// of the camera files that this program uses.
// "usein" parameter contains full line read from the camera file.
std::vector<double> tracerio::camin(string usein, int bound){
	string holder = usein.substr(0, usein.length());
	size_t position;
	std::vector<double> lineAttributes;
	try{

		// Read each attribute from the line.
		for(int i = 0; i < bound; i++){
			double in = std::stod(holder, &position);
			lineAttributes.push_back(in);
			holder = holder.substr(position, holder.length());
		}

	} catch (exception &e){
		badfile(e.what());
	}
	return lineAttributes;
}

// Read Model Materials file.
// lights and materials parameters are passed to this function empty.
void tracerio::readMaterials(std::vector<Light> &lights, std::vector<Material> &materials, string materialFilename){

	//Open file
	ifstream mats(materialFilename, ifstream::in);
	if(!mats.good()){
		badfile("Materials file failed to open.");
	}

	//Read each line
	string holder;
	while(!std::getline(mats, holder).eof()){
		char firstChar = holder[0];
		size_t pos;

		//Read light lines
		if(firstChar == 'L'){
			holder = holder.substr(holder.find_first_of(" "), holder.length());

			double r, g, b, x, y, z;

			// Read color
			r = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			g = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			b = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Special case: Read ambient light.
			if(holder.find('A', 0) != string::npos){
				Light light(r, g, b, 0, 0, 0, false);
				lights.push_back(light);
				continue;
			}

			//Read (x, y, z) position
			x = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			y = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			z = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			Light light(r, g, b, x, y, z, true);
			lights.push_back(light);
		}

		//Read material lines
		else if(firstChar == 'M'){
			holder = holder.substr(holder.find_first_of(" "), holder.length());
			int mindex, beginning, end;
			double lone, ltwo, lthree;
			double k, alpha, translucency;

			//Read model number
			mindex = std::stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read first polygon that material applies to
			beginning = std::stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read last polygon that the material applies to
			end = std::stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read lambda diagonals
			lone = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			ltwo = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			lthree = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read specular reflectance constant
			k = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read shininess exponent
			alpha = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read percent translucency
			translucency = std::stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			Material material(mindex, beginning, end, lone, ltwo, lthree, k, alpha, translucency);
			materials.push_back(material);
		}
		else{
			badfile(materialFilename);
		}
	}

	mats.close();
}

// Read PLY files one by one and build the model object.
// modelFilename - Model file string.
// modnum - Number that has been assigned to this model for bookkeeping purposes.
// materials - Material vector loaded with all materials.
Model* tracerio::buildModel(string modelFilename, int modnum, const std::vector<Material> &materials){

	//Read file
	ifstream ply(modelFilename, std::ifstream::in);
	if(!ply.good()){
		badfile("PLY file failed to open.");
	}

	//Read first header
	string headerBegin;
	std::getline(ply, headerBegin);
	if(headerBegin.find("ply") == std::string::npos){
		badfile("Header line 1");
	}

	//Read format line
	string format;
	std::getline(ply, format);
	if(format.find("format ascii 1.0")){
		badfile("Header line 2");
	}

	int vertexCount = 0;
	int faceCount = 0;
	string vproperties = ""; // Properties of vertices.
	string properties; // Properties of position data.

	// Read Header.
	string holder;
	int stepper = 1;
	while(!std::getline(ply, holder).eof()){

		// Terminate loop if the end of the header is found.
		if(holder.find("end_header") != std::string::npos){
			break;
		}

		// Skip over PLY comment lines.
		if(holder.find("comment") != std::string::npos){
			continue;
		}

		// Read vertex count.
		else if(holder.find("element vertex ") == std::string::npos && stepper == 1){
			badfile("Header line 3: Vertex Line");
		}

		// After vertex count is read, read vertex datatype lines.
		else if(stepper == 1){
			int lastSpace = holder.find_last_of(" ") + 1;
			int vertexSize = lastSpace - holder.length();
			vertexCount = atoi(holder.substr(lastSpace, vertexSize).c_str());
			for(int i = 0; i < 3; i++){
				if(std::getline(ply, holder).eof()){
					badfile("Type lines are invalid");
				}
				vproperties.append(holder);
				if(i != 2){
					vproperties.append("\n");
				}
			}
			stepper++;
			continue;
		}

		// After vertex datatype lines are read, read face count.
		else if(stepper == 2){
			int lastSpace = holder.find_last_of(" ") + 1;
			int faceSize = lastSpace - holder.length();
			faceCount = atoi(holder.substr(lastSpace, faceSize).c_str());
			if(std::getline(ply, holder).eof()){
				badfile("Header line 7: face line");
			} else {
				properties = holder;
			}
			stepper++;
			continue;
		}
	}

	//Read each vertex line
	std::vector<Vertex> vertices;
	size_t pos;
	for(int i = 0; i < vertexCount; i++){
		double x, y, z;
		std::getline(ply, holder);

		x = std::stod(holder, &pos);
		holder = holder.substr(pos, holder.length());
		y = std::stod(holder, &pos);
		holder = holder.substr(pos, holder.length());
		z = std::stod(holder, &pos);
		holder = holder.substr(pos, holder.length());

		vertices.push_back(Vertex(x, y, z));
	}

	//Read each face line
	std::vector<Polygon> faces;
	for(int i = 0; i < faceCount; i++){
		std::vector<Vertex> faceVertices;
		std::getline(ply, holder);
		int count = std::stoi(holder, &pos);
		holder = holder.substr(pos, holder.length());

		//Polygons, by definition, must be composed of at least three points.
		if(count < 3) badfile("Faces");

		//Create a new Polygon object from a given face's associated vertices.
		int element;
		for(int j = 0; j < count; j++){
			element = std::stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());
			faceVertices.push_back(vertices[element]);
		}
		Polygon newface(faceVertices);

		//Split faces into triangles. Add to face vector.
		std::vector<Polygon> newfaces = geops::triangularize(newface);
		faces.insert(faces.end(), newfaces.begin(), newfaces.end());
	}

	//Recalculate face count to account for the new faces produced by the triangularize operation.
	faceCount = faces.size();

	Model* model = new Model(modelFilename, vertexCount, faceCount, faces, properties, vproperties);

	ply.close();

	return model;
}

//TODO: catch the error emerging from this function
// Output ply file of the models in their current positions.
void tracerio::writePLY(Scene* set, string out){
	std::vector<std::vector<string>> objectStrings = set->toString();
	std::for_each(objectStrings.begin(), objectStrings.end(), [&](std::vector<string> objinfo){
		string name = objinfo.at(0);
		string body = objinfo.at(1);
		string filename = out + "_" + name;
		cout << "Outputting \"" + out + "_" + name + "\"." << endl;
		ofstream output(filename, std::ofstream::out);

		// TODO: throw the error. This should be updated when I require user-input for output
		// filename.
		/*
		if(!output.good()){
			throw OpenException(out);
		}
		*/
		output << body;
		output.close();
	});
}

// Writes the .ppm image
void writePPM(std::vector<RGB> image, Camera camera, char *out){
	Screen screen = camera.getScreen();
	int rows = screen.getlowu() - screen.gethighu() + 1;
	int columns = screen.gethighv() - screen.getlowv() + 1;

	ofstream output(out);

	cout << "Writing PPM" << endl;
	output << "P3" << endl;
	output << rows << " " << columns << endl;
	int inc = 0;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			RGB cur = image[inc];
			//cout << cur.red() << endl;
			output << cur.red() << " " << cur.green() << " " << cur.blue();
			if(j != columns - 1){
				output << "   ";
			}
			inc++;
		}
		output << endl;
	}
	output.close();
}
