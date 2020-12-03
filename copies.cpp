#include "copies.h"
using namespace std;


//Constructor:
//ID ISBN Availability reader borrowDate ExpireDate #ofReservers reserverName reserveDate
copies::copies(int a, long long b, bool c, string d, int e, int f, vector <string> g, vector <int> h) {
	ID = a;
	ISBN = b;
	available = c;
	reader = d;
	borrowDate = e;
	expireDate = f;
	reservers = g;
	reserverDates = h;
}

copies::copies(int a, long long b, bool c, vector <string> g, vector <int> h) {
	ID = a;
	ISBN = b;
	available = c;
	reservers = g;
	reserverDates = h;
}

// Here are all the Set functions:
void copies::setID(int x) {
	ID = x;
}
void copies::setISBN(long long x) {
	ISBN = x;
}
void copies::setReader(string x) {
	reader = x;
}
void copies::setAvailable(bool x) {
	available = x;
}
void copies::setBorrowDate(int x) {
	borrowDate = x;
}
void copies::setExpireDate(int x) {
	expireDate = x;
}
void copies::setReserverDates(vector <int> x) {
	reserverDates = x;
}
void copies::setReservers(vector <string> x) {
	reservers = x;
}

//here are all the get functions:
int copies::getID() {
	return ID;
}
long long copies::getISBN() {
	return ISBN;
}
string copies::getReader() {
	return reader;
}
bool copies::getAvailable() {
	return available;
}
int copies::getBorrowDate() {
	return borrowDate;
}
int copies::getExpireDate() {
	return expireDate;
}


int copies::getReserverDate(int x) {
	return reserverDates[x];
}
string copies::getReserver(int x) {
	return reservers[x];
}
void copies::addReserverDate(int x) {
	reserverDates.push_back(x);
}
void copies::addReserverName(string x) {
	reservers.push_back(x);
}


int copies::getReserverSize() {
	return reservers.size();
}
int copies::getReserverDateSize() {
	return reserverDates.size();
}