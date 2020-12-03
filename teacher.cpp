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