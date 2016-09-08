/*
 * IO.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: colin
 */

#include "IO.h"

void badfile(string type){
	cout << "Error: " + type + "\nExiting." << endl;
	exit(EXIT_FAILURE);
}


//TODO: ERROR CHECKING
//Build Camera object
Camera readCamera(const string in){

	//Open File
	ifstream camfile(in, ifstream::in);
	if(!camfile.good()){
		cout << "camera file failed to open" << endl;
		exit(EXIT_FAILURE);
	}

	string line;

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
Point camPoint(ifstream &camfile){
	string line;
	getline(camfile, line);
	std::vector<double> fpoint = camin(line, 3);
	Point point(fpoint[0], fpoint[1], fpoint[2]);
	return point;
}

//Read Camera file lines
std::vector<double> camin(string usein, unsigned int inlength){
	string holder = usein.substr(0, usein.length());
	size_t pos;
	int bound = inlength;
	try{
		std::vector<double> ret;
		for(int i = 0; i < bound; i++){
			double in = std::stod(holder, &pos);
			ret.push_back(in);
			holder = holder.substr(pos, holder.length());
		}
		return ret;
	} catch (exception &e){
		cout << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

//Read Model Materials file.
void readMaterials(std::vector<Light> &lights, std::vector<Material> &materials, string filename){

	//Open file
	ifstream mats(filename, ifstream::in);
	if(!mats.good()){
		cout << "Materials file failed to open. Exiting." << endl;
		exit(EXIT_FAILURE);
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

			r = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			g = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			b = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Special case: Read ambient light.
			if(holder.find('A', 0) != string::npos){
				Light light(r, g, b, 0, 0, 0, false);
				lights.push_back(light);
				continue;
			}

			//Read (x, y, z) position

			x = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			y = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			z = stod(holder, &pos);
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
			mindex = stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read first polygon that material applies to
			beginning = stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read last polygon that the material applies to
			end = stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read lambda diagonals
			lone = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			ltwo = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());
			lthree = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read specular reflectance constant
			k = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read shininess exponent
			alpha = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			//Read percent translucency
			translucency = stod(holder, &pos);
			holder = holder.substr(pos, holder.length());

			/*
			cout << "mindex: " << mindex << " beginning: " << beginning
			<< " end: " << end << "\nlone: " << lone << " ltwo: " << ltwo
			<< " lthree: " << lthree << "\nk: " << k << " alpha: " << alpha << endl;
			*/

			Material material(mindex, beginning, end, lone, ltwo, lthree, k, alpha, translucency);
			materials.push_back(material);
		}
		else{
			badfile("material");
		}
	}

	mats.close();
}


Model* readPLY(string in, int modnum, const std::vector<Material> &materials){

	//Read file
	ifstream ply(in, std::ifstream::in);
	if(!ply.good()){
		cout << "PLY file failed to open. Exiting." << endl;
		exit(EXIT_FAILURE);
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
	string vproperties = ""; //properties of vertices
	string properties; // properties of position data

	// Read Header
	string holder;
	int stepper = 1;
	while(!std::getline(ply, holder).eof()){

		// Terminate loop if the end of the header is found
		if(holder.find("end_header") != std::string::npos){
			break;
		}

		// Skip over comment lines
		if(holder.find("comment") != std::string::npos){
			continue;
		}

		// Read vertex count
		else if(holder.find("element vertex ") == std::string::npos && stepper == 1){
			badfile("Header line 3: Vertex Line");
		}

		// After vertex count is read, read vertex datatype lines
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

		// After vertex datatype lines are read, read face count
		else if(stepper == 2){
			int lastSpace = holder.find_last_of(" ") + 1;
			int faceSize = lastSpace - holder.length();
			faceCount = atoi(holder.substr(lastSpace, faceSize).c_str());
			if(std::getline(ply, holder).eof()){
				badfile("Header line 7: face line");
			} else {
				//TODO: Process the property list for vertex faces
				properties = holder;
			}
			stepper++;
			continue;
		}
	}

	//Read vertices
	std::vector<Vertex> vertices;
	size_t pos;
	for(int i = 0; i < vertexCount; i++){
		double x, y, z;
		//TODO: Will definitely need to debug this
		std::getline(ply, holder);

		x = stod(holder, &pos);
		holder = holder.substr(pos, holder.length());

		y = stod(holder, &pos);
		holder = holder.substr(pos, holder.length());

		z = stod(holder, &pos);
		holder = holder.substr(pos, holder.length());

		vertices.push_back(Vertex(x, y, z));
	}

	//Read faces
	std::vector<Polygon> faces;
	for(int i = 0; i < faceCount; i++){
		std::vector<Vertex> faceVertices;
		std::getline(ply, holder);
		int count = stoi(holder, &pos);
		holder = holder.substr(pos, holder.length());

		//TODO: try/catch for error
		if(count < 3) badfile("Faces");
		int element;
		for(int j = 0; j < count; j++){
			element = stoi(holder, &pos);
			holder = holder.substr(pos, holder.length());
			faceVertices.push_back(vertices[element]);
		}
		//Split faces into triangles. Add to face vector.
		Polygon newface(faceVertices);
		std::vector<Polygon> newfaces = geops::triangularize(newface);
		faces.insert(faces.end(), newfaces.begin(), newfaces.end());
	}

	//Recalculate face count
	faceCount = faces.size();

	cout << in << endl;

	Model* model = new Model(in, vertexCount, faceCount, faces, properties, vproperties);

	ply.close();

	return model;
}

void writePLY(Scene* set, string out){
	std::vector<std::vector<string>> objectStrings = set->toString();
	std::for_each(objectStrings.begin(), objectStrings.end(), [&](std::vector<string> objinfo){
		string name = objinfo.at(0);
		string body = objinfo.at(1);
		string filename = out + "_" + name;
		cout << "Outputting \"" + out + "_" + name + "\"." << endl;
		ofstream output(filename, std::ofstream::out);
		if(!output.good()){
			cout << "Output file failed to open. Exiting." << endl;
			exit(EXIT_FAILURE);
		}
		output << body;
		output.close();
	});
}


//Write the .ppm image
/*
void writePPM(vector<RGB> image, Camera camera, char *out){
	Screen screen = camera.getScreen();
	int rows = screen.maxu() - screen.minu() + 1;
	int columns = screen.maxv() - screen.minv() + 1;

	ofstream output(out);

	cout << "Writing PPM" << endl;
	output << "P3" << endl;
	output << rows << " " << columns << " 255" << endl;
	int inc = 0;
	for(int i = 0; i < columns; i++){
		for(int j = 0; j < rows; j++){
			RGB cur = image[inc];
			//cout << cur.red() << endl;
			output << cur.red() << " " << cur.green() << " " << cur.blue();
			if(j != rows - 1){
				output << "   ";
			}
			inc++;
		}
		output << endl;
	}
	output.close();
}
*/
