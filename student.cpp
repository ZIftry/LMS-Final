#include <vector>
#include <stdio.h>
#include "student.h"
using namespace std;

student::student(string a, string b, int c, int d, int e, vector <int> f, vector <int> g) {
	//Constructor
	userName = a;
	passWord = b;
	maxAllowed = c;
	maxTime = d;
	penalties = e;
	borrowedList = f;
	reservedList = g;
}

student::student(string a, string b) {
	vector<int> borrow;
	vector<int> reserve;
	
	userName = a;
	passWord = b;
	maxAllowed = 5;
	maxTime = 30;
	penalties = 0;
	borrowedList = borrow;
	reservedList = reserve;
}

int student::borrowsListSize(){
	return(borrowedList.size());
}

int student::getPenalties(){
	return(penalties);
}

int student::getListValue(int x){
	return(borrowedList[x]);
}

int student::getReserveValue(int x){
	return(reservedList[x]);
}
