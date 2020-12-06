#include <iostream>
#include <vector>
#include "book.h"
using namespace std;


book::book(){
	vector<int> temp;

	ISBN = -1;
	title = "";
	author = "";
	category = "";
	favor = 0;
	count = 0;
	index = temp;
}

//Constructor:
book::book(long long a, string b, string c, string d, int e, int f, vector <int> g) {
	ISBN = a;
	title = b;
	author = c;
	category = d;
	favor = e;
	count = f;
	index = g;
}

book::book(long long a, string b, string c, string d){
	vector<int> g;

	ISBN = a;
	title = b;
	author = c;
	category = d;
	favor = 0;
	count = 1;
	index = g;
}


// Set functions:
void book::setISBN(long long x) {
	ISBN = x;
}
void book::setTitle(string x) {
	title = x;
}
void book::setAuthor(string x) {
	author = x;
}
void book::setCategory(string x) {
	category = x;
}
void book::setCount(int x) {
	count = x;
}
void book::setFavor(int x) {
	favor = x;
}
void book::addIndex(int x) {
	index.push_back(x);
}


// Get functions:
long long book::getISBN() {
	return ISBN;
}
string book::getTitle() {
	return title;
}
string book::getAuthor() {
	return author;
}
string book::getCategory() {
	return category;
}
int book::getCount() {
	return count;
}
int book::getFavor() {
	return favor;
}
int book::getIndex(int x) {
	return index[x];			// Returns the index at position x of the vector
}