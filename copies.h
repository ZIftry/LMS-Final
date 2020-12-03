#pragma once
#include <iostream>
#include <vector>
using namespace std;

class copies{
private:
	int ID;
	long long ISBN;
	string reader;			
	bool available;
	int borrowDate;
	int expireDate;
	vector <string> reservers;
	vector <int> reserverDates;

public:
	//Constructors:
	//ID ISBN Availability reader borrowDate ExpireDate #ofReservers reserverName reserveDate
	copies(int a, long long b, bool c, string d, int e, int f, vector <string> g, vector <int> h);
	copies(int a, long long b, bool c, vector <string> g, vector <int> h);


	void setID(int x);
	void setISBN(long long x);
	void setReader(string x);
	void setAvailable(bool x);			// Set functions
	void setBorrowDate(int x);
	void setExpireDate(int x);
	void setReservers(vector <string> x);


	int getID();
	long long getISBN();
	string getReader();
	bool getAvailable();			//Get functions
	int getBorrowDate();
	int getExpireDate();
	void setReserverDates(vector <int> x);


	int getReserverDate(int x);	//These functions get information about the reserver
	string getReserver(int x);	//X is the index of the reserver
	int getReserverSize();
	int getReserverDateSize();

	void addReserverDate(int x);		//These functions add information on a reserver
	void addReserverName(string x);		// Either the date or name
};