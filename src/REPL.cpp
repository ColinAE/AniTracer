#include "REPL.h"

//TODO: need to sort out the weird things that happen during turnaround.

void ReadEval::REPL(Scene* set, string outFile){
	string usein;
	int selection;

	std::cout << "\n~~REPL started~~\n" << std::endl;

	// Process user commands.
	while(true){
		help();

		// Grab single command character from the command line.
		getline(std::cin, usein);
		char firstin = usein[0];
		if(!ReadEval::validCommand("TEH", firstin)){
			ReadEval::invalid(usein);
		}

		// Handle "Transform", "Help", and "Exit" commands depending on user input.
		switch(firstin){

			// Transform an entire model. User specifies which model
			case 'T':
				std::cout << "Objects 0 through " << set->objCount() - 1 << " available. Select object: ";
				std::cin >> selection;
				ReadEval::transform(set, selection, outFile);
				break;

			// Print help message.
			case 'H':
				ReadEval::help();
				break;

			// Exit out of REPL
			case 'E':
				std::cout << "Exiting REPL." << std::endl;
				return;

			// If user enter some other command, print error message. Loop will continue.
			default:
				ReadEval::invalid(usein);
		}
	}
}

// Checks
bool ReadEval::validCommand(string commands, char command){
	string::size_type found = commands.find(command);
	if(found == string::basic_string::npos){
		return false;
	}
	else{
		return true;
	}
}

// Transformations get accumulated within the Transformer object
void ReadEval::transform(Scene* set, int objnum, string &outFile){
	Transformer transformer = Transformer();
	string usein;
	getline(std::cin, usein); // Consume any newlines that may still be lingering.
	tMatrix trans;
	bool end = false;

	// Transform object.
	while(true){

		// Grab the transformation command and the x, y, z scalars for the transformation.
		double x, y, z, theta;
		std::cout << "Transforming object " << objnum << "." << std::endl;
		std::cout << "Input transformation:" << std::endl;
		getline(std::cin, usein);
		char firstin = usein[0];

		// Check for validity of the command. Does not check for validity of the scalars
		// associated with each transformation. That comes during the "switch" statement.
		if(!validCommand("CUFHSTRQKW", firstin)){
			invalid(usein);
			continue;
		}

		// Execute the command. Performs validity checking of scalars for each recognized command.
		switch(firstin){

			// Scale the model.
			// Since the scale operations only uses one scalar to scale all three dimensions,
			// this case manually loads the usein string with three copies of the scalar in
			// place of the x, y, and z places. This means we can reuse the parseInputString
			// function.
			case 'S':
				theta = -1000;
				//Loads usein with dummies for y and z so parseInputString won't fail.
				usein += " " + myUtilities::itos(x) + " " + myUtilities::itos(x);
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					std::cout << "Scale by " << myUtilities::dtos(x) << std::endl;
					transformer.scale(x, x, x);
				}
				break;

			// Skew the model.
			case 'K':
				theta = -1000;
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					std::cout << "Skew by (" << myUtilities::dtos(x) << ", " << myUtilities::dtos(y) << ", " << myUtilities::dtos(z)
							<< ")" << std::endl;
					transformer.scale(x, y, z);
				}
				break;

			// Transform the model.
			case 'T':
				theta = -1000;
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					std::cout << "Translate by (" << myUtilities::dtos(x) << ", " << myUtilities::dtos(y) <<
							", " << myUtilities::dtos(z) << ")" << std::endl;
					transformer.translate(x, y, z);
				}
				break;

			// Rotate the model.
			// This is the only transformation that utilizes the theta parameter of parseInputString.
			case 'R':
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					std::cout << "Rotate by (" << myUtilities::dtos(x) << ", " << myUtilities::dtos(y) << ", " << myUtilities::dtos(z) <<
							")  " << myUtilities::dtos(theta) << std::endl;
					transformer.axis_angle(x, y, z, theta);
				}
				break;

			// Finish transformation.
			// This case triggers the update of the entire model.
			case 'U':
				std::cout << "Updating object." << std::endl;
				trans = transformer.transformationMatrix();
				set->update(trans, objnum);
				std::cout << "Object updated." << std::endl;
				break;

			case 'F':
				end = true; // Tell the loop to end.
				break;

			// Write the model back out to a PLY file.
			case 'W':
				tracerio::writePLY(set, outFile);
				break;

			case 'C':
				std::cout << transformer.toString();
				break;

			// Output transform help message.
			case 'H':
				helpTransform();
				break;

			// If the default case gets hit, something has gone wrong in the program.
			default:
				std::cout << "default" << endl;
				invalid(usein);
		}
		if(end) return; // Break out of loop.
		usein = ""; // Empty user input holder with each iteration of the loop.
	}
}

void ReadEval::help(){
	std::cout << "Available commands: \n" <<
	"T -- Initiate object transformation.\n" <<
	"E -- Exit\n" <<
	"H -- Print this help message" << std::endl;
}

void ReadEval::helpTransform(){
	std::cout << "Available commands:\n" <<
	"S sx -- Scale. sx is a scale factor.\n" <<
	"K sx sy sz -- Skew. sx, sy, and sz are scale factors.\n" <<
	"T tx ty tz -- Translate. tx, ty, and tz together are a translation vector.\n" <<
	"R rx ry rz theta -- Rotate. rx, ry, and rz together are the axis to be rotated around." <<
	"Theta is the angle of rotation in degrees.\n" <<
	"U -- Update object.\n" <<
	"F -- Finish transformation.\n" <<
	"W -- Write object to .ply file.\n" <<
	"C -- Print out current transformation matrix" <<
	"H -- Print this help message.\n" << std::endl;
}

void ReadEval::invalid(const string &errtype){
	std::cout << "Invalid operation \"" << errtype << "\". Try again." << std::endl;
}

int ReadEval::parseInputString(string usein, double &x, double &y, double &z, double &theta){

	string holder = usein.substr(1, usein.length() - 1);
	size_t pos;
	try{
		x = std::stod(holder, &pos);
		holder = holder.substr(pos, holder.length() - 1);
		y = std::stod(holder, &pos);
		holder = holder.substr(pos, holder.length() - 1);
		z = std::stod(holder, &pos);
	} catch (exception& e){
		std::cout << "Could not parse user input for x, y, or z.\n Exception: " << e.what() << std::endl;
		return -1;
	}

	if(theta != -1000){
		try{
			holder = holder.substr(pos, holder.length() - 1);
			theta = std::stod(holder, &pos);
			theta = ((theta * 3.14159265)/180);
		} catch (exception& e){
			std::cout << "Could not parse user input for theta.\n Exception: " << e.what() << std::endl;
			return -1;
		}
	}

	return 0;
}
