#include "Utility.h"
#include <string>

using std::string;

string itos(int changethis){
	std::ostringstream sstream;
	sstream << changethis;
	string converted = sstream.str();
	return converted;
}


string dtos(double changethis){
	std::ostringstream sstream;
	sstream << changethis;
	string converted = sstream.str();
	return converted;
}
