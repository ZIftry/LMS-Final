#include "user.h"


void user::setName(string x) {
	userName = x;
}

void user::setPass(string x) {
	passWord = x;
}

string user::getName(){
	return userName;
}

string user::getPass(){
	return passWord;
}

