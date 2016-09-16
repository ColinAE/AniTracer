#include "REPL.h"

//TODO: need to sort out the weird things that happen during turnaround.

void ReadEval::REPL(Scene* set, string outFile){
	string usein;
	int selection;

	cout << "\n~~REPL started~~\n" << endl;

	// Process user commands.
	while(true){
		help();

		// Grab single command character from the command line.
		getline(cin, usein);
		char firstin = usein[0];
		if(!ReadEval::validCommand("TEH", firstin)){
			ReadEval::invalid(usein);
		}

		// Handle "Transform", "Help", and "Exit" commands depending on user input.
		switch(firstin){

			// Transform an entire model. User specifies which model
			case 'T':
				cout << "Objects 0 - " << set->objCount() - 1 << " available. Select object: ";
				cin >> selection;
				cout << endl;
				ReadEval::transform(set, selection, outFile);
				break;

			// Print help message.
			case 'H':
				ReadEval::help();
				break;

			// Exit out of REPL
			case 'E':
				cout << "Exiting REPL." << endl;
				return;

			// If user enter some other command, print error message. Loop will continue.
			default:
				ReadEval::invalid(usein);
		}
	}
}


bool ReadEval::validCommand(string commands, char command){
	string::size_type found = commands.find(command);
	if(found == string::basic_string::npos){
		return false;
	}
	else{
		return true;
	}
}

// Transformations get accumulated in
void ReadEval::transform(Scene* set, int objnum, string &outFile){
	Transformer transformer = Transformer();
	string usein;
	tMatrix trans;
	cout << endl;
	bool end = false;

	// Transform object.
	while(true){
		usein = ""; // Empty user input holder with each iteration of the loop.

		// Grab the transformation command and the x, y, z scalars for the transformation.
		double x, y, z, theta;
		cout << "Transforming object " << objnum << "." << endl;
		cout << "Input transformation:" << endl;
		getline(cin, usein);
		cout << usein << endl; //TODO this line and the line one two lines below it are experiments
		char firstin = usein[0];
		cout << firstin << endl;

		// Check for validity of the command. Does not check for validity of the scalars
		// associated with each transformation. That comes during the "switch" statement.
		if(!validCommand("FHSTRQKW", firstin)){
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
				usein += " " + itos(x) + " " + itos(x);
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.scale(x, y, z);
				}
				break;

			// Skew the model.
			case 'K':
				theta = -1000;
				if(usein.size() > 3 || parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.scale(x, y, z);
				}
				break;

			// Transform the model.
			case 'T':
				theta = -1000;
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.translate(x, y, z);
				}
				break;

			// Rotate the model.
			// This is the only transformation that utilizes the theta parameter of parseInputString.
			case 'R':
				if(parseInputString(usein, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.axis_angle(x, y, z, theta);
				}
				break;

			// Finish transformation.
			// This case triggers the update of the entire model.
			case 'F':
				trans = transformer.transformationMatrix();
				set->update(trans, objnum);
				end = true; // Tell the loop to end.
				break;

			// Write the model back out to a PLY file.
			case 'W':
				tracerio::writePLY(set, outFile);
				break;

			// Output transform help message.
			case 'H':
				helpTransform();
				break;

			// If the default case gets hit, something has gone wrong in the program.
			default:
				cout << "default" << endl;
				invalid(usein);
		}
		if(end) return; // Break out of loop.
	}
}

void ReadEval::help(){
	cout << "Available commands: \n" <<
	"T -- Initiate object transformation.\n" <<
	"E -- Exit\n" <<
	"H -- Print this help message" << endl;
}

void ReadEval::helpTransform(){
	cout << "Available commands:\n" <<
	"S sx -- Scale. sx is a scale factor.\n" <<
	"K sx sy sz -- Skew. sx, sy, and sz are scale factors.\n" <<
	"T tx ty tz -- Translate. tx, ty, and tz together are a translation vector.\n" <<
	"R rx ry rz theta -- Rotate. rx, ry, and rz together are the axis to be rotated around." <<
	"Theta is the angle of rotation in degrees.\n" <<
	"F -- Finish transformation.\n" <<
	"H -- Print this help message.\n" <<
	"W -- Write object to .ply file.\n" << endl;
}

void ReadEval::invalid(const string &errtype){
	cout << "Invalid operation \"" << errtype << "\". Try again." << endl;
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
		cout << "Could not parse user input for x, y, or z.\n Exception: " << e.what() << endl;
		return -1;
	}

	if(theta != -1000){
		try{
			holder = holder.substr(pos, holder.length() - 1);
			theta = std::stod(holder, &pos);
			theta = ((theta * 3.14159265)/180);
		} catch (exception& e){
			cout << "Could not parse user input for theta.\n Exception: " << e.what() << endl;
			return -1;
		}
	}

	return 0;
}
