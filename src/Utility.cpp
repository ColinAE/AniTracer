#include "Utility.h"
#include <string>

using std::string;

namespace myUtilities {

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

bool tolerates(double left, double right){
	const double tolerance = .00000001;
	if(abs(left - right) < tolerance){
		return true;
	} else {
		return false;
	}
}

} // namespace myUtilities
