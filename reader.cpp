#include <iostream>
#include <vector>
#include "reader.h"
using namespace std;

// Set functions:
void reader::setMaxAllowed(int x) {
	maxAllowed = x;
}
void reader::setMaxTime(int x) {
	maxTime = x;
}
void reader::setBorrowedList(vector <int> x) {
	borrowedList = x;
}

// Get functions:
int reader::getMaxAllowed() {
	return maxAllowed;
}
int reader::getMaxTime() {
	return maxTime;
}
int reader::getPenalties() {
	return penalties;
}
int reader::getBorrowedSize() {
	return borrowedList.size();
}
int reader::getReservedSize() {
	return reservedList.size();
}
int reader::getBorrowedBook(int x) {
	return borrowedList[x];
}
int reader::getReservedBook(int x) {
	return	reservedList[x];
}




void reader::printList() {							//This prints the ID's of all the reader's borrowed books
	for (int i = 0; i < borrowedList.size(); i++) {
		cout << borrowedList[i] << endl;
	}
	cout << endl;
}
void reader::printReader() {						// This function is for testing
													// It prints all the information on the logged in student
	cout << "Username: " << userName << endl;
	cout << "Password: " << passWord << endl;
	cout << "Penalties: " << penalties << endl;
	cout << "maxAllowed: " << maxAllowed << endl;
	cout << "maxTime: " << maxTime << endl;
	cout << "Borrowed List:" << endl;
	for (int i = 0; i < borrowedList.size(); i++) {
		cout << borrowedList[i] << endl;
	}
	cout << "Reserved List: " << endl;
	for (int i = 0; i < reservedList.size(); i++) {
		cout << reservedList[i] << endl;
	}
	cout << endl;
}


bool reader::hasCopy(int x) { // int x is a Copy ID

	for (int i = 0; i < borrowedList.size(); i++) {	// Loops through all the ID's in the reader's list
		if (borrowedList[i] == x)					// returns true if the student is borrowing that Copy
			return true;
	}
	return false;
}
void reader::returnCopy(int x) {					//takes 'x' which is a copy ID
	for (int i = 0; i < borrowedList.size(); i++) {	//  <----loops through the reader's list of borrowed copies
		if (borrowedList[i] == x) {
			borrowedList.erase(borrowedList.begin() + i);    //  Erases it from the list since the reader returned it
		}
	}

	// This can be much more efficient if the copy is updated individually here instead of rewriting the entire files

}
void reader::borrowCopy(int x) {     // The student borrowed a book of ID 'x'
	borrowedList.push_back(x);		// adds that ID to the students borrowed list
}
