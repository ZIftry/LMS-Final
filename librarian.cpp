#include "librarian.h"

// Constructor :
librarian::librarian(string x, string y) {
	userName = x;
	passWord = y;
}

string librarian::getName() {
	return userName;
}

string librarian::getPass() {
	return passWord;
}
