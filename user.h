#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class user {	//User class is the base for reader and librarian classes
protected:
	string userName;
	string passWord;
public:
	void setName(string x);		//Set functions
	void setPass(string x);
	

	string getName();		//Get functions
	string getPass();
};