#include "REPL.h"

void ReadEval::REPL(Scene* set, string outFile){
	string usein;
	int selection;

	cout << "\n~~REPL started~~\n" << endl;

	// Process user commands
	while(true){
		help();

		// Grab input
		getline(cin, usein);
		char firstin = usein[0];
		if(!ReadEval::validCommand("TEH", firstin)){
			ReadEval::invalid(usein);
		}

		//
		switch(firstin){
			case 'T':
				cout << "Objects 0 - " << set->objCount() - 1 << " available. Select object: ";
				cin >> selection;
				cout << endl;
				ReadEval::transform(set, selection, outFile);
				break;
			case 'H':
				ReadEval::help();
				break;
			case 'E':
				cout << "Exiting REPL." << endl;
				return;
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

void ReadEval::transform(Scene* set, int objnum, string &outFile){
	Transformer transformer = Transformer();
	string usein;
	tMatrix trans;
	cout << endl;
	bool end = false;

	while(true){
		usein = "";

		// Grab input
		double x, y, z, theta;
		cout << "Transforming object " << objnum << "." << endl;
		cout << "Input transformation:" << endl;
		getline(cin, usein);
		cout << usein << endl;
		char firstin = usein[0];
		cout << firstin << endl;
		if(!validCommand("FHSTRQKW", firstin)){
			invalid(usein);
			continue;
		}

		//
		switch(firstin){
			case 'S':
				theta = -1000;
				usein += " " + itos(x) + " " + itos(x);
				if(checkInput(usein, 7, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.scale(x, y, z);
				}
				break;
			case 'K':
				theta = -1000;
				if(usein.size() > 3 || checkInput(usein, 7, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.scale(x, y, z);
				}
				break;
			case 'T':
				theta = -1000;
				if(checkInput(usein, 7, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.translate(x, y, z);
				}
				break;
			case 'R':
				if(checkInput(usein, 9, x, y, z, theta) == -1){
					invalid(usein);
				} else {
					transformer.axis_angle(x, y, z, theta);
				}
				break;
			case 'F':
				trans = transformer.transformationMatrix();
				set->update(trans, objnum);
				end = true;
				break;
			case 'W':
				writePLY(set, outFile);
				break;
			case 'H':
				helpTransform();
				break;
			default:
				cout << "default" << endl;
				invalid(usein);
		}
		if(end) return;
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
	"L -- Finish transformation.\n" <<
	"H -- Print this help message.\n" <<
	"W -- Write object to .ply file.\n" << endl;
}

void ReadEval::invalid(const string &errtype){
	cout << "Invalid operation \"" << errtype << "\". Try again." << endl;
}

int ReadEval::checkInput(string usein, unsigned int inlength, double &x, double &y, double &z, double &theta){

	string holder = usein.substr(1, usein.length() - 1);
	size_t pos;
	try{
		x = stod(holder, &pos);
		holder = holder.substr(pos, holder.length() - 1);
		y = stod(holder, &pos);
		holder = holder.substr(pos, holder.length() - 1);
		z = stod(holder, &pos);
	} catch (exception& e){
		cout << "Could not parse user input for x, y, or z.\n Exception: " << e.what() << endl;
		return -1;
	}

	if(theta != -1000){
		try{
			holder = holder.substr(pos, holder.length() - 1);
			theta = stod(holder, &pos);
			theta = ((theta * 3.14159265)/180);
		} catch (exception& e){
			cout << "Could not parse user input for theta.\n Exception: " << e.what() << endl;
			return -1;
		}
	}

	return 0;
}
