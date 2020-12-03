#pragma once
#include <iostream>
#include <vector>
using namespace std;

class book {
private:
	long long ISBN;
	string title;
	string author;
	string category;
	int count;
	int favor;

	vector <int> index;

public:
	book(long long a, string b, string c, string d, int e, int f, vector <int> g);	// Constructor

	void setISBN(long long x);
	void setTitle(string x);
	void setAuthor(string x);
	void setCategory(string x);		//Set functions
	void setFavor(int x);
	void setCount(int x);	
	void addIndex(int x);

	long long getISBN();
	string getTitle();
	string getAuthor();	// get functions
	string getCategory();
	int getCount();
	int getFavor();
	int getIndex(int x);
};