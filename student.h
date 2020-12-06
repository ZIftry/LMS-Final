#pragma once
#include "reader.h"

class student : public reader {
public:
	student(string a, string b, int c, int d, int e, vector <int> f, vector <int> g);
	student(string a, string b);

	int borrowsListSize();
	int getPenalties();
	int getListValue(int x);
	// Check the .cpp file for more details
};
