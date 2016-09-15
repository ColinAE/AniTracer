/*
 * MyException.h
 *
 *  Created on: Aug 16, 2016
 *      Author: colin
 */

#ifndef MYEXCEPTIONS_H_
#define MYEXCEPTIONS_H_

#include <iostream>
#include <exception>
#include <string>
using std::exception;
using std::string;

class ProtocolException;
class ProtocolBoundsException;
class ModelCountException;
class ModelBoundsException;
class FlagException;
class ReadException;

class ProtocolException: public exception {
	virtual const char* what() const throw () {
		return "Protocol argument could not be converted to integer.";
	}
} pexc;

class ProtocolBoundsException: public exception {
	virtual const char* what() const throw () {
		return "Protocol range out of bounds.";
	}
} pbexc;

class ModelCountException: public exception {
	virtual const char* what() const throw () {
		return "Model count argument could not be converted to integer.";
	}
} mexc;

class ModelBoundsException: public exception {
	virtual const char* what() const throw () {
		return "Model count less than 1. There must be at least one model";
	}
} mbexc;

class FlagException: public exception {
	virtual const char* what() const throw () {
		return "Flag unrecognized.";
	}
} flagexc;

class OpenException: public exception {
	virtual const char* what(string readable) const throw () {
		return "Could not open file: ";
	}
} oexc;


#endif /* MYEXCEPTIONS_H_ */
