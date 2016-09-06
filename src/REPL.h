#ifndef REPL_H
#define REPL_H

#include <stdio.h>
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include <exception>
#include "Scene.h"
#include "IO.h"

using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::exception;

class ReadEval {
private:
	static bool validCommand(string, char);
	static void transform(Scene*, int, string &);
	static void help();
	static void helpTransform();
	static void invalid(const string &);
	static int checkInput(string, unsigned int, double &, double &, double &, double &);
public:
	static void REPL(Scene*, string);
};

#endif
