#pragma once
#include "user.h"
#include <string>
using namespace std;

class reader: public user{
protected:
	int maxAllowed;	// Number of books allowed to take out
	int maxTime;	// The amount of time they are allowed to take them out
	int penalties;	// The number of penalties
	vector <int> borrowedList;	// This is a list of the books the user has borrowed
	vector <int> reservedList;	// The list of reserved books
public:
	void setMaxAllowed(int x);
	void setMaxTime(int x);					//set functions
	void setBorrowedList(vector <int> x);

	int getMaxAllowed();
	int getMaxTime();			// get functions

	void printList();	// Prints the borrowedList
	void printReader();	//Prints all the information on the reader

	void borrowCopy(int x);		// When a student borrows a copy of ID X
	void returnCopy(int x);		// used when a student returns a copy of ID X
	bool hasCopy(int x);		// Returns true if the student has taken out the copy with ID x
};