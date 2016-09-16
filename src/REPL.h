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

// Functions involved in the REPL.
// All are private except the REPL function. These functions are helpers of REPL.
class ReadEval {
private:

	// Check if command is within string of valid commands.
	// commands - String of individual characters that stand for commands.
	// command - Command user has input.
	static bool validCommand(string commands, char command);

	// Transform individual model.
	// objnum - The assigned identity for a given object.
	// outFile - Output file for this model.
	static void transform(Scene* set, int objnum, string &outFile);

	// Print out help message for the first stage of the REPL.
	static void help();

	// Print help message for the transform stage of the REPL.
	static void helpTransform();

	// Print out error message when user puts in a bad command.
	// A string describing what went wrong.
	static void invalid(const string &errtype);

	// Parse user input string and check for validity.
	// usein - String containing user input.
	// x, y, and z - Empty return holders for the transformation scalars.
	// theta - Empty return holder for rotation angle when performing the rotation transformation.
	// theta is passed as -1000 if performing any transformation other than rotation.
	static int parseInputString(string usein, double &x, double &y, double &z, double &theta);

public:

	// Read Evaluate Print Line.
	// Begins user command processing loop.
	static void REPL(Scene* set, string outFile);
};

#endif
