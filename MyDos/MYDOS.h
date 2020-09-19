#pragma once
#include "head.h"
#ifndef MYDOS
class MYDOS
{
public:
	string path;
	MYDOS(string path = "D:");

	void cd(vector<string> args);

	void ls();

	void cls();

	void run();

	void help();
	void rm(vector<string> args);
	void mv(vector<string> args);
	void cp(vector<string> args);
	void mkdir(vector<string> args);
};

#endif // !MYDOS
