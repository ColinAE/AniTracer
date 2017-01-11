/*
 * Utility.h
 *
 *  Created on: Aug 20, 2016
 *      Author: colin
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <sstream>

using std::string;

namespace myUtilities {

string itos(int);
string dtos(double);
bool tolerates(double left, double right);

} // namespace myUtilities

#endif /* UTILITY_H_ */
