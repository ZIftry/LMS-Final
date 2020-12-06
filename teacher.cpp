#include "teacher.h"



teacher::teacher(string a, string b, int c, int d, int e, vector <int> f, vector <int> g) {
	//Constructor
	userName = a;
	passWord = b;
	maxAllowed = c;
	maxTime = d;
	penalties = e;
	borrowedList = f;
	reservedList = g;
}


teacher::teacher(string a, string b) {
	vector<int> borrow, reserve;
	
	userName = a;
	passWord = b;
	maxAllowed = 10;
	maxTime = 50;
	penalties = 0;
	borrowedList = borrow;
	reservedList = reserve;
}


int teacher::borrowsListSize() {
	return(borrowedList.size());
}

int teacher::getReserveValue(int x){
	return(reservedList[x]);
}
