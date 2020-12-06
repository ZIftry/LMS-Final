#include <iostream>	// cout cin
#include <vector> //vectors
#include <string>	//string variables used
#include <fstream>	// download and uploading files
#include <ctime>		// ctime used for clock and time functions
#include "book.h"			//importing all the header files
#include "librarian.h"
#include "teacher.h"
#include "student.h"
#include "copies.h"
using namespace std;

void loadUsers();
void loadBooks();
void loadCopies();
void login();
void readerInterface();
void adminInterface();
void uploadUsers();
void uploadBooks();
void uploadCopies();
void borrowBook();
void returnBook();
void searchBook();
void reserveBook();
void cancelReservation();
void renewBook();
void recommendBook();
void addBook();
void deleteBook();
void searchUser();
void addUser();
void deleteUser();
int getTime();
int adjustTime(int currentTime);


int startTime = getTime();//startTime is the time the program started at   (time is kept in seconds since january 1, 2010)

vector <book> books;	// This vector holds all the books loaded from book.txt
vector <copies> copiez;

vector <student> students; // students holds all the student accounts loaded from student.txt
vector <teacher> teachers; // holds all the teachers
vector <librarian> admins; // Holds all the librarians

int userIndex = -1;  //This is the index in the vector of the logged in user
					  // It doesn't need to be -1 but it made user authentication easier
int userType = -1;  // This will differentiate which type of user is using the system


int main() {
	loadBooks();
	loadCopies();
	loadUsers();
	login();
	if (userType == 3) {
		adminInterface();
	}
	else if (userType == 2 || userType == 1) {
		readerInterface();
	}
	else {
		cerr << "Error in user type." << endl;
		exit(1);
	}

	cout << "Thank you for visiting the library!" << endl;
	
	uploadUsers();
	uploadBooks();
	uploadCopies();

}

void loadBooks() {    //Reads the data from book.txt
	int count, favor;
	long long ISBN;							// these are the holder variables to transfer file contents to vector books
	string title, author, category;
	ifstream data_in;
	data_in.open("book.txt");				//opens book.txt as data_in and if fails returns an error
	if (data_in.fail()) {
		cerr << "Error: Failed to open 'book.txt'.";
		exit(1);
	}
	while (!data_in.eof()) {			 // loops until end of file
		vector <int> index;
		int holder;
		data_in >> ISBN >> title >> author >> category >> favor >> count;// the book.txt file holds information in this order
		for (int i = 0; i < count; i++) {
			data_in >> holder;
			index.push_back(holder);
		}
		books.push_back(book(ISBN, title, author, category, favor, count, index));		//creates a book in the vector with the information
	}
	data_in.close();
	cout << "Successfully loaded book data" << endl;	// closes book.txt and gives a msg
}

void loadCopies() {
	// the copies.txt file holds information in this format:
	//ID ISBN Availability reader borrowDate ExpireDate #ofReservers reserverName reserveDate
	ifstream data_in;
	data_in.open("copies.txt");			//opens users.txt as data_in
	if (data_in.fail()) {
		cerr << "Error: Failed to open 'copies.txt'.";	//gives an error if it cant open user.txt
		exit(1);
	}

	while (!data_in.eof()) {			//loop until at end of file for copies.txt 
		long long ISBN;
		int ID, borrowDate, expireDate, NumofReservers, reserverDate;
		bool availability;
		string reader, reserver;
		vector <string> reservers;
		vector <int> reserverDates;
		data_in >> ID >> ISBN >> availability;
		if (!availability)
			data_in >> reader >> borrowDate >> expireDate;
		data_in >> NumofReservers;
		for (int i = 0; i < NumofReservers; i++) {
			data_in >> reserver >> reserverDate;
			reservers.push_back(reserver);
			reserverDates.push_back(reserverDate);
		}
		if (!availability) {
			copiez.push_back(copies(ID, ISBN, availability, reader, borrowDate, expireDate, reservers, reserverDates));
		}
		else {
			copiez.push_back(copies(ID, ISBN, availability, reservers, reserverDates));
		}
	}
	data_in.close();	//closes user.txt
	cout << "Successfully loaded book copies" << endl;	//gives a success message
}

void loadUsers() { //reads the data from users.txt
	string username, password;
	int type;

	ifstream data_in;
	data_in.open("users.txt");			//opens users.txt as data_in
	if (data_in.fail()) {
		cerr << "Error: Failed to open 'users.txt'.";	//gives an error if it cant open user.txt
		exit(1);
	}

	while (!data_in.eof()) {			//loop until at end of file for user.txt 
		int maxAllowed, maxTime, listSize1, listSize2, ID, penalties, ISBN;		//creates the corrosponding variables to the information in user.txt	
		vector <int> borrowed;	// This vector is going to hold the borrowed copy ID's for the user before construction for that user
		vector <int> reserved;	// This vector will hold the reserved book ISBN's for the user

		data_in >> type >> username >> password;

		switch (type) {
		case 1:
		case 2:
			data_in >> penalties >> maxAllowed >> maxTime >> listSize1;	//takes in more information from user.txt
			for (listSize1; listSize1 > 0; listSize1--) {		//listSize1 will indicate how many copy ID's the user has taken out
				data_in >> ID;								//So, it will tell the code how many more integers to scan
				borrowed.push_back(ID);
			}
			data_in >> listSize2;
			for (listSize2; listSize2 > 0; listSize2--) {		//listSize2 will indicate how many books the user has reserved
				data_in >> ISBN;								//So, it will tell the code how many more integers to scan
				reserved.push_back(ISBN);
			}
			//These if statments checks the 'type' of user to make the correct object: student or teacher
			if (type == 1) {
				students.push_back(student(username, password, maxAllowed, maxTime, penalties, borrowed, reserved));
			}
			else if (type == 2) {
				teachers.push_back(teacher(username, password, maxAllowed, maxTime, penalties, borrowed, reserved));
			}
			break;
		case 3:
			admins.push_back(librarian(username, password));
			break;
		}
	}
	data_in.close();	//closes user.txt
	cout << "Successfully loaded user data" << endl << endl;	//gives a success message
}

void login() {	// user authentication function
	while (1) {
		string user = "", pass = "";	// strings holding the username and password
		int ACCtype;
		bool found = false;			// bool if a matching account is found
		cout << "Login: " << endl << endl;
		cout << "(1) Student Login" << endl;
		cout << "(2) Teacher Login" << endl;
		cout << "(3) Admin Login" << endl;
		cin >> ACCtype;
		cout << endl << "Username:" << endl;
		cin >> user;
		cout << "Password:" << endl;		// takes in username and password
		cin >> pass;
		if (ACCtype == 3) {
			for (int i = 0; i <= admins.size() - 1 || found; i++) {			// This searches every student in the vector students
				if (admins[i].getName() == user && admins[i].getPass() == pass) {
					found = true;						//If the username and password match then the index of the student is remembered
					userIndex = i;					//and found is set to true
					userType = 3;
					break;
				}
			}
		}
		else if (ACCtype == 2) {
			for (int i = 0; i <= teachers.size() - 1 || found; i++) {			// This searches every student in the vector students
				if (teachers[i].getName() == user && teachers[i].getPass() == pass) {
					found = true;						//If the username and password match then the index of the student is remembered
					userIndex = i;					//and found is set to true
					userType = 2;
					break;
				}
			}
		}
		else if (ACCtype == 1) {
			for (int i = 0; i <= students.size() - 1 || found; i++) {			// This searches every student in the vector students
				if (students[i].getName() == user && students[i].getPass() == pass) {
					found = true;						//If the username and password match then the index of the student is remembered
					userIndex = i;					//and found is set to true
					userType = 1;
					break;
				}
			}
		}
		else {
			cout << "Please enter a valid account type." << endl;
		}

		if (!found) {
			cout << "No user with this password found" << endl << endl;	// if not found
		}
		else {
			break;		//breaks the infinite while loop 
		}
	}
}

void readerInterface() {
	char input;
	bool logout = false;
	if (userType == 1) {
		cout << "Welcome " << students[userIndex].getName() << endl << endl;
	}
	else if (userType == 2) {
		cout << "Welcome " << teachers[userIndex].getName() << endl << endl;
	}
	while (1) {
		cout << "(1) Borrow Book" << endl;
		cout << "(2) Return Book" << endl;
		cout << "(3) Search Book" << endl;
		cout << "(4) Reserve Book" << endl;
		cout << "(5) Cancel Reservation" << endl;
		cout << "(6) Renew Book" << endl;
		cout << "(7) Receive a book recomendation!" << endl;
		cout << "(0) Logout" << endl;

		cin >> input;
		switch (input) {
		case '0':
			logout = true;
			break;
		case '1':
			borrowBook();
			break;
		case '2':
			returnBook();
			break;
		case '3':
			searchBook();
			break;
		case '4':
			reserveBook();
			break;
		case '5':
			cancelReservation();
			break;
		case '6':
			renewBook();
			break;
		case '7':
			recommendBook();
			break;
		default:
			cout << "Not a valid option. Please try again.";
			break;
		}

	}
}

void adminInterface() {
	char input;
	bool logout = false;
	while (1) {
		cout << "\nWelcome " << admins[userIndex].getName() << endl << endl;
		cout << "(1) Search Book" << endl;
		cout << "(2) Add New Book" << endl;
		cout << "(3) Delete Book" << endl;
		cout << "(4) Search User" << endl;
		cout << "(5) Add New User" << endl;
		cout << "(6) Delete User" << endl;
		cout << "(0) Logout" << endl;
		cin >> input;
		switch (input) {
		case '0':
			logout = true;
			break;
		case '1':
			searchBook();
			break;
		case '2':
			addBook();
			break;
		case '3':
			deleteBook();
			break;
		case '4':
			searchUser();
			break;
		case '5':
			addUser();
			break;
		case '6':
			deleteUser();
			break;
		default:
			cout << "Not a valid option. Please try again.";
			break;
		}
	}
}

int getTime() {	// This function gets the current time
	time_t timer;			// Time is counted in seconds since January 1, 2010
	struct tm y2k = { 0 };
	double seconds;
	// ********I did not write this
	y2k.tm_hour = -1;   y2k.tm_min = 0; y2k.tm_sec = 0;		// This code is from the <ctime> library reference : time
	y2k.tm_year = 110; y2k.tm_mon = 0; y2k.tm_mday = 2;		// The link to the page is:
																					// http://www.cplusplus.com/reference/ctime/time/
	time(&timer);  // get current time; same as: timer = time(NULL)  

	seconds = difftime(timer, mktime(&y2k));
	return seconds;
}

int adjustTime(int currentTime) { // This takes in the current time which will be gotton with the function getTime()
								  //It then adjusts the time so every X amount of seconds in real life while using this program
									// will have a day pass inside the program
	int time_difference = currentTime - startTime;
	int time = currentTime + time_difference * (864 * 10); // <--- The number being multipled with 864 is how many seconds while the program is openned will count a day	
	return time;											// At the time of writing this it is currently 10: So every 10 seconds the program is open 1 day will pass

	// This function takes the difference from the start time of the program and the current time
	// and skews it so every 10 seconds the program was open a day passed
}







void borrowBook() {
	int bookID, bookIndex = -1;       //bookID is the ID of the book the user wants to take borrow   
									// bookIndex is the index of that book in the vector books
	bool allowed = true;	// this bool is false if the student is not allowed to take out another book

	//Test if the student has maximum copies borrowed
	if (students[userIndex].getMaxAllowed() <= students[userIndex].borrowsListSize()) {
		// First checks if the student has his maximum allowed number of books taken out

		cout << "You have already taken out your maximum amount of books. Please return a book to take one out." << endl;
		allowed = false;	// if so prints this msg and sets allowed = false
	}

	//Check for overdue copies


	//Enter ID of book to see if copy is available


	//Check if others have reserved the book/If they they have cancelled reservation

	//Use formula to determine borrow period

	//Make student borrower of the copy

	//Put ID on the students borrow list
}




void returnBook() {
	int returnID, copyIndex;	//ID of the book the student is returning
	student student = students[userIndex];
	vector<int> newList;
	bool copy = false;
	

	cout << "What is the ID of the book you want to return?" << endl;
	cin >> returnID;							// Asks for the ID of the book and takes it in

	
	//Make sure student Book ID input is on the list
	for (int i = 0; i < student.borrowsListSize(); i++) {
		if ((student.getListValue(i) == returnID)) {
			copy = true;
		}
	}



	//Copy removed from students list of borrowed copies
	if (copy) {
		for (int i = 0; i < student.borrowsListSize(); i++) {
			if (!(student.getListValue(i) == returnID)) {
				newList.push_back(student.getListValue(i));
			}
		}
		//Set penalty using formula/////////////////////////////////////////////////////////////////////////////////////////////////////////


		student.setBorrowedList(newList);
	}
	else {
		cout << "The ID entered is not on the list of borrowed books" << endl;
		return;
	}

	//make copy available in copiez vector

	for (int i = 0; i < copiez.size(); i++) {//Find index of the copy
		if (copiez[i].getID() == returnID)
			copyIndex = i;
	}

	copiez[copyIndex].setAvailable(true);//Make the copy avaible


}




void searchBook() {
	int index = -1;
	char input;
	long long ISBN;
	int counter = 1;
	bool flag = false;
	string title, author, category;
	vector<book> popularityVector;
	book temp;


	cout << "(1) Search by ISBN" << endl;
	cout << "(2) Search by Title" << endl;
	cout << "(3) Search by Author's Name" << endl;
	cout << "(4) Search by Category" << endl;

	cin >> input;
	
	cout << endl;

	switch (input) {
		case '1'://ISBN Case

			cout << "Enter ISBN- ";
			cin >> ISBN;
			cout << endl;

			//Book Info listing
			for (int i = 0; i < books.size(); i++) {
				if (books[i].getISBN() == ISBN) {
					index = i;
					cout << "Book Found" << endl;
					cout << "ISBN- " << books[i].getISBN() << endl;
					cout << "Title- " << books[i].getTitle() << endl;
					cout << "Author's Name- " << books[i].getAuthor() << endl;
					cout << "Category- " << books[i].getCategory() << endl << endl;
				}
			}

			//If index is invalid
			if (index == -1) {
				cout << "ISBN entered is not a book in the database" << endl << endl;
				return;
			}

			//Check if there exists any copies
			for (int i = 0; i < copiez.size(); i++) {
				if (copiez[i].getISBN() == ISBN && copiez[i].getAvailable() == true) {
					flag = true;
				}
			}

			//12423347357
			//ID copy listing
			if (flag) {
				cout << "ID's of the available Copies" << endl;
				for (int i = 0; i < copiez.size(); i++) {
					if (copiez[i].getISBN() == ISBN && copiez[i].getAvailable() == true) {
						cout << "(" << counter << ") " << copiez[i].getID() << endl;
						counter++;
					}
				}
				cout << endl << endl;
			}


			break;
		case '2'://Title case
			cout << "Enter Title of Book- ";
			cin >> title;
			cout << endl;

			//Book Info listing
			for (int i = 0; i < books.size(); i++) {
				if (books[i].getTitle() == title) {
					index = i;
					cout << "Book Found" << endl;
					cout << "ISBN- " << books[i].getISBN() << endl;
					cout << "Title- " << books[i].getTitle() << endl;
					cout << "Author's Name- " << books[i].getAuthor() << endl;
					cout << "Category- " << books[i].getCategory() << endl << endl;
				}
			}

			//If index is invalid
			if (index == -1) {
				cout << "Title entered is not a book in the database" << endl << endl;
				return;
			}

			//Check if there exists any copies
			for (int i = 0; i < books.size(); i++) {
				if (books[i].getTitle() == title) {
					ISBN = books[i].getISBN();
				}
			}

			for (int i = 0; i < copiez.size(); i++) {
				if (copiez[i].getISBN() == ISBN && copiez[i].getAvailable() == true) {
					flag = true;
				}
			}

			//12423347357
			//ID copy listing
			if (flag) {
				cout << "ID's of the available Copies" << endl;
				for (int i = 0; i < copiez.size(); i++) {
					if (copiez[i].getISBN() == ISBN && copiez[i].getAvailable() == true) {
						cout << "(" << counter << ") " << copiez[i].getID() << endl;
						counter++;
					}
				}
				cout << endl << endl;
			}

			break;
		
		
		
		
		case '3'://Author case
			cout << "Enter the Author's Name- ";
			cin >> author;
			cout << endl;

			//Putting books in Vector
			for (int i = 0; i < books.size(); i++) {
				if (books[i].getAuthor() == author) {
					popularityVector.push_back(books[i]);
				}
			}

			//Check if the author exists
			if (popularityVector.size() == 0) {
				cout << "Author entered is not in the database" << endl << endl;
				return;
			}

			//Sort by popularity
			for (int i = 0; i < popularityVector.size(); i++) {
				for (int j = 0; j < popularityVector.size(); j++) {
					if (popularityVector[i].getFavor() > popularityVector[j].getFavor()) {
						temp = popularityVector[i];
						popularityVector[i] = popularityVector[j];
						popularityVector[j] = temp;
					}
				}
			}

			//Print info
			for (int i = 0; i < popularityVector.size(); i++) {
				if(counter == 1)
					cout << "Books Found" << endl;
				
				cout << "Book #" << counter << endl;

				counter++;

				cout << "ISBN- " << popularityVector[i].getISBN() << endl;
				cout << "Title- " << popularityVector[i].getTitle() << endl;
				cout << "Author's Name- " << popularityVector[i].getAuthor() << endl;
				cout << "Category- " << popularityVector[i].getCategory() << endl << endl;
			}

			counter = 1;
			
			//See if any copies exist
			for (int i = 0; i < copiez.size(); i++) {
				for (int j = 0; j < popularityVector.size(); j++) {
					if (copiez[i].getISBN() == popularityVector[j].getISBN() && copiez[i].getAvailable() == true) {
						flag = true;
					}
				}
			}

			//ID copy listing
			if (flag) {
				cout << "ID's of the available Copies" << endl;
				for (int i = 0; i < copiez.size(); i++) {
					for (int j = 0; j < popularityVector.size(); j++) {
						if (copiez[i].getISBN() == popularityVector[j].getISBN() && copiez[i].getAvailable() == true) {
							cout << "(" << counter << ") " << copiez[i].getID() << endl;
							counter++;
						}
					}
				}
				cout << endl << endl;
			}

			break;
		case '4'://Category case
			cout << "Enter the Category- ";
			cin >> category;
			cout << endl;

			//Putting books in Vector
			for (int i = 0; i < books.size(); i++) {
				if (books[i].getCategory() == category) {
					popularityVector.push_back(books[i]);
				}
			}

			//Check if the author exists
			if (popularityVector.size() == 0) {
				cout << "Category entered is not in the database" << endl << endl;
				return;
			}

			//Sort by popularity
			for (int i = 0; i < popularityVector.size(); i++) {
				for (int j = 0; j < popularityVector.size(); j++) {
					if (popularityVector[i].getFavor() > popularityVector[j].getFavor()) {
						temp = popularityVector[i];
						popularityVector[i] = popularityVector[j];
						popularityVector[j] = temp;
					}
				}
			}

			//Print info
			for (int i = 0; i < popularityVector.size(); i++) {
				if (counter == 1)
					cout << "Books Found" << endl;

				cout << "Book #" << counter << endl;

				counter++;

				cout << "ISBN- " << popularityVector[i].getISBN() << endl;
				cout << "Title- " << popularityVector[i].getTitle() << endl;
				cout << "Author's Name- " << popularityVector[i].getAuthor() << endl;
				cout << "Category- " << popularityVector[i].getCategory() << endl << endl;
			}

			counter = 1;

			//See if any copies exist
			for (int i = 0; i < copiez.size(); i++) {
				for (int j = 0; j < popularityVector.size(); j++) {
					if (copiez[i].getISBN() == popularityVector[j].getISBN() && copiez[i].getAvailable() == true) {
						flag = true;
					}
				}
			}

			//ID copy listing
			if (flag) {
				cout << "ID's of the available Copies" << endl;
				for (int i = 0; i < copiez.size(); i++) {
					for (int j = 0; j < popularityVector.size(); j++) {
						if (copiez[i].getISBN() == popularityVector[j].getISBN() && copiez[i].getAvailable() == true) {
							cout << "(" << counter << ") " << copiez[i].getID() << endl;
							counter++;
						}
					}
				}
				cout << endl << endl;
			}



			break;
		default:
			cout << "Not a valid Input returning to main" << endl;
				return;
	}



}





void cancelReservation() {

}

void renewBook() {

}
void recommendBook() {  //Bonus

}



void addBook() { // Function adds a copy of a book if it already exists, creates a book object otherwise
	int ID = rand() % 1000000;
	long long ISBN;
	string title, author, category;
	vector<int> index;
	index.push_back(0);

	cout << "Enter the ISBN, title, author's name, and category of the book: " << endl;
	cin >> ISBN;
	cin >> title;
	cin >> author;
	cin >> category;
	for (int i = 0; i < books.size(); i++) {
		if (books[i].getISBN() == ISBN) {
			copiez.push_back(copies(ID, ISBN, 1));
			cout << "New copy added." << endl;
			break;
		}
		else {
			books.push_back(book(ISBN, title, author, category, 1, 0, index));
			cout << "New book added." << endl;
			break;
		}
	}
}
void deleteBook() { // Function deletes a copy of a book from the list of copies
	int ID;

	cout << "Enter the ID of the copy to be deleted: ";
	cin >> ID;
	for (int i = 0; i < copiez.size(); i++) {
		if (ID == copiez[i].getID()) {
			copiez.erase(copiez.begin() + i);
			cout << "Copy deleted." << endl;
			break;
		}
		else if (!copiez[i].getAvailable()) {
			cerr << "Cannot delete, copy is lent out." << endl;
			break;
		}
		else {
			cerr << "Copy does not exist." << endl;
			break;
		}
	}
}
void searchUser() { // Function searches for a user
	string name;

	cout << "Enter the username of the user to search: ";
	cin >> name;
	for (int i = 0; i < admins.size(); i++) {
		if (admins[i].getName() == name) // If user is a librarian, prints username and password
			cout << "Username: " << admins[i].getName()
			<< "\nPassword: " << admins[i].getPass() << endl;
	}
	for (int i = 0; i < students.size(); i++) {
		if (students[i].getName() == name) // If user is a student or a teacher, prints username, password,
												// type, and the copies being borrowed
			cout << "Username: " << students[i].getName()
				 << "\nPassword: " << students[i].getPass()
				 << "\nReader type: Student"
				 << "\nCopies being kept: " << students[i].getBorrowedBook(i) << endl;
	}
	for (int i = 0; i < teachers.size() - 1; i++) {
		if (teachers[i].getName() == name)
			cout << "Username: " << teachers[i].getName()
				 << "\nPassword: " << teachers[i].getPass()
				 << "\nReader type: Teacher"
				 << "\nCopies being kept: " << teachers[i].getBorrowedBook(i) << endl;
	}
}
void addUser() { // Creates an account for a new user
	string type, name, pass;

	cout << "Enter the type of user (student/teacher/librarian): ";
	cin >> type;
	cout << "Enter the username and password for the user: ";
	cin >> name;
	cin >> pass;
	for (int i = 0; i < students.size(); i++) {
		if (students[i].getName() == name) {
			cerr << "User already exists." << endl;
			return;
		}
	}
	for (int i = 0; i < teachers.size(); i++) {
		if (teachers[i].getName() == name) {
			cerr << "User already exists." << endl;
			return;
		}
	}
	for (int i = 0; i < admins.size(); i++) {
		if (admins[i].getName() == name) {
			cerr << "User already exists." << endl;
			return;
		}
	}

	if (type == "student") {
		students.push_back(student(name, pass));
		cout << "New user added." << endl;
	}
	else if (type == "teacher") {
		teachers.push_back(teacher(name, pass));
		cout << "New user added." << endl;
	}
	else if (type == "librarian") {
		admins.push_back(librarian(name, pass));
		cout << "New user added." << endl;
	}

	else
		cout << "Enter a valid type of user." << endl;
}
void deleteUser() { // Deletes a user from the system
	string name;

	cout << "Enter the name of the user: ";
	cin >> name;
	for (int i = 0; i < students.size(); i++) {
		if (students[i].getName() == name)
			students.erase(students.begin() + i);
	}
	for (int i = 0; i < teachers.size(); i++) {
		if (teachers[i].getName() == name)
			teachers.erase(teachers.begin() + i);
	}
	for (int i = 0; i < admins.size(); i++) {
		if (admins[i].getName() == name)
			admins.erase(admins.begin() + i);
	}
	cout << "User deleted." << endl;
}
