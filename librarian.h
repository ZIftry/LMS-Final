#pragma once
#include "reader.h"
using namespace std;

class librarian : public user {
public:
	librarian(string x, string y);	// Constructor for Librarian
	
	string getName();
	string getPass();
};
