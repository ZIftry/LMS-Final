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
void borrowBook();
void returnBook();
void searchBook();
void reserveBook();
void cancelReservation();
void renewBook();
void recommendBook();
void addBook();
void deleteBook();
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
	while (logout) {

		cout << "Welcome " << admins[userIndex].getName() << endl << endl;
		cout << "(1) Search Book" << endl;
		cout << "(2) Add New Book" << endl;
		cout << "(3) Delete Book" << endl;
		cout << "(4) Add New User" << endl;
		cout << "(5) Delete User" << endl;
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
			addUser();
			break;
		case '5':
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
	bool canBorrow = true, hasOverdue = false;
	long long ISBN;
	int bookIndex;
	vector <int> availableCopies;   //A vector of the Index's of the available copies
	cout << "Enter the ISBN of the book you would like to borrow." << endl;
	cin >> ISBN;
	if (userType == 1) {	
		// We will check if they have overdue books now:
		for (int i = 0; i < students[userIndex].getBorrowedSize(); i++) {	// Loops for the number of borrowed books
			for (int j = 0; j < copiez.size(); j++) {
				if (copiez[j].getID() == students[userIndex].getBorrowedBook(j)) {	//This is true if they have borrowed this book
					if (copiez[j].getExpireDate() < adjustTime(getTime())) {				//this is true if the book is overdue
						hasOverdue = true;
						canBorrow = false;
					}
				}
			}
		}
		//Checking if they are at their max number of books borrowed:
		if (students[userIndex].getMaxAllowed() <= students[userIndex].getBorrowedSize()) {
			canBorrow = false;
			cout << "You already have borrowed your maximum amount of books. Please return a book to take one out." << endl;
		}
		if (hasOverdue) {
			canBorrow = false;
			cout << "You have overdue books. Please return them to take a new one out." << endl;
		}

		if (canBorrow) {
			for (int i = 0; i < copiez.size(); i++) {//loops through all the copies to find matching ISBN
				if (copiez[i].getISBN() == ISBN) {
					if (copiez[i].getAvailable()) {	// If this copy is available
						if (copiez[i].getReserverSize() == 0) {	// If the copy is not reserved the index is saved and the for loop is broken
							availableCopies.push_back(i);
						}
						else {
							if (copiez[i].getReserver(0) == students[userIndex].getName()) { //Checks if the User is the reservee
									availableCopies.push_back(i);
							}
						}
					}
				}
			}
			cout << "Here is a list of the book ID's of the available copies with the given ISBN:" << endl;
			for (int i = 0; i < availableCopies.size(); i++) {
				cout << i+1 << ": " << copiez[availableCopies[i]].getID() << endl;
			}
			cout << endl << "Which one would you like to borrow?" << endl;
			cin >> bookIndex;
			bookIndex--;

			students[userIndex].borrowCopy(copiez[availableCopies[bookIndex]].getID()); // Adds the ID of the copy to the student's list
			copiez[availableCopies[bookIndex]].setReader(students[userIndex].getName()); //sets reader name in the copy
			copiez[availableCopies[bookIndex]].setBorrowDate(adjustTime(getTime()));	//Sets borrow time in the copy
			copiez[availableCopies[bookIndex]].setAvailable(false); //update availibilty 
			copiez[availableCopies[bookIndex]].deleteFirstReserver();	//removes the logged in user from the reserve list
			copiez[availableCopies[bookIndex]].setExpireDate(adjustTime(getTime()) + students[userIndex].getMaxTime() - copiez[availableCopies[bookIndex]].getReserverSize());	//sets the expiration date, its the users maxtime - the number of current reservers

			cout << "ISBN: " << copiez[2].getISBN() << endl;
			cout << "Reader: " << copiez[2].getReader() << endl;
			if (copiez[2].getAvailable())
				cout << "It is available" << endl;

			if (!copiez[2].getAvailable())
				cout << "It is not available" << endl;
			cout << "Borrowed on " << copiez[2].getBorrowDate() << endl << "expires on " << copiez[2].getExpireDate() << endl;



			cout << "You have successfully borrowed this book. It must be returned within " << students[userIndex].getMaxTime() - copiez[availableCopies[bookIndex]].getReserverSize() << " days." << endl;
		}
	}
	else if (userType == 2) {
		// We will check if they have overdue books now:
		for (int i = 0; i < teachers[userIndex].getBorrowedSize(); i++) {	// Loops for the number of borrowed books
			for (int j = 0; j < copiez.size(); j++) {
				if (copiez[j].getID() == teachers[userIndex].getBorrowedBook(j)) {	//This is true if they have borrowed this book
					if (copiez[j].getExpireDate() < adjustTime(getTime())) {				//this is true if the book is overdue
						hasOverdue = true;
						canBorrow = false;
					}
				}
			}
		}
		//Checking if they are at their max number of books borrowed:
		if (teachers[userIndex].getMaxAllowed() <= teachers[userIndex].getBorrowedSize()) {
			canBorrow = false;
			cout << "You already have borrowed your maximum amount of books. Please return a book to take one out." << endl;
		}
		if (hasOverdue) {
			cout << "You have overdue books. Please return them to take a new one out." << endl;
		}

		if (canBorrow) {
			for (int i = 0; i < copiez.size(); i++) {//loops through all the copies to find matching ISBN
				if (copiez[i].getISBN() == ISBN) {
					if (copiez[i].getAvailable()) {	// If this copy is available
						if (copiez[i].getReserverSize() == 0) {	// If the copy is not reserved the index is saved and the for loop is broken
							availableCopies.push_back(i);
						}
						else {
							if (copiez[i].getReserver(0) == teachers[userIndex].getName()) { //Checks if the User is the reservee
								availableCopies.push_back(i);
							}
						}
					}
				}
			}
			cout << "Here is a list of the book ID's of the available copies with the given ISBN:" << endl;
			for (int i = 0; i < availableCopies.size(); i++) {
				cout << i+1 << ": " << copiez[availableCopies[i]].getID() << endl;
			}
			cout << endl << "Which one would you like to borrow?" << endl;
			cin >> bookIndex;
			bookIndex--;

			teachers[userIndex].borrowCopy(copiez[availableCopies[bookIndex]].getID()); // Adds the ID of the copy to the student's list
			copiez[availableCopies[bookIndex]].setReader(teachers[userIndex].getName()); //sets reader name in the copy
			copiez[availableCopies[bookIndex]].setBorrowDate(adjustTime(getTime()));	//Sets borrow time in the copy
			copiez[availableCopies[bookIndex]].setAvailable(false); //update availibilty 
			copiez[availableCopies[bookIndex]].deleteFirstReserver();	//removes the logged in user from the reserve list
			copiez[availableCopies[bookIndex]].setExpireDate(adjustTime(getTime()) + teachers[userIndex].getMaxTime() - copiez[availableCopies[bookIndex]].getReserverSize());	//sets the expiration date, its the users maxtime - the number of current reservers

			cout << "You have successfully borrowed this book. It must be returned within " << teachers[userIndex].getMaxTime() - copiez[availableCopies[bookIndex]].getReserverSize() << " days." << endl;
		}
	}
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
	int ID, index;

	cout << "Input ID to remove from reservation list- ";
	cin >> ID;
	cout << endl;


	for (int i = 0; i < copiez.size(); i++) {//Get index of copy
		if (copiez[i].getID() == ID) {
			index = i;
		}
	}


	//Remove reader from books reservation list
	for (int i = 0; i < copiez[index].getReserverSize(); i++) {
		if (copiez[index].getReserver(i) == students[userIndex].getName()) {
			copiez[index].removeReserver(i);
		}
	}


	//Remove book from readers reservation list
	for (int i = 0; i < students[userIndex].borrowsListSize(); i++) {
		if (students[userIndex].getListValue(i) == copiez[index].getID()) {
			students[userIndex].removeReserver(i);
		}
	}
}


void addUser() {
	char userType;
	string userName, password;
	bool duplicate = false;

	cout << "(1) Create New Student" << endl;
	cout << "(2) Create New Teacher" << endl;
	cout << "(3) Create New Librarian" << endl;
	cout << "Enter User Type to Make New Account- ";
	cin >> userType;


	switch (userType){
		case '1':
			cout << "Enter the Username for the new Student Account- ";
			cin >> userName;
			cout << "Enter the Password for the new Student Account- ";
			cin >> password;
			cout << endl;

			for (int i = 0; i < students.size(); i++) {
				if (students[i].getName() == userName) {
					duplicate = true;
				}
			}

			if (duplicate) {
				cout << "This Username was Already Used" << endl;
				return;
			}

		break;
		
		case '2':
			cout << "Enter the Username for the new Teacher Account- ";
			cin >> userName;
			cout << "Enter the Password for the new Teacher Account- ";
			cin >> password;
			cout << endl;

			for (int i = 0; i < teachers.size(); i++) {
				if (teachers[i].getName() == userName) {
					duplicate = true;
				}
			}

			if (duplicate) {
				cout << "This Username was Already Used" << endl;
				return;
			}

		break;

		case '3':
			cout << "Enter the Username for the new Librarian Account- ";
			cin >> userName;
			cout << "Enter the Password for the new Librarian Account- ";
			cin >> password;
			cout << endl;

			for (int i = 0; i < admins.size(); i++) {
				if (admins[i].getName() == userName) {
					duplicate = true;
				}
			}

			if (duplicate) {
				cout << "This Username was Already Used" << endl;
				return;
			}

		break;

	default:
		cout << "Invalid Input Returning to Main" << endl;
		break;
	}

	if (userType == '1') {
		student newStudent(userName, password);
		students.push_back(newStudent);
	}
	else if (userType == '2') {
		teacher newTeacher(userName, password);
		teachers.push_back(newTeacher);
	}
	else if (userType == '3') {
		librarian newLibrarian(userName, password);
		admins.push_back(newLibrarian);
	}


}


void deleteUser() {
	string userName;
	int index;
	char type;
	vector<int> idList;


	cout << "Enter the Name of the User to be deleted- ";
	cin >> userName;

	for (int i = 0; i < students.size(); i++) {
		if (students[i].getName() == userName) {
			index = i;
			type = '1';
		}
	}

	for (int i = 0; i < teachers.size(); i++) {
		if (teachers[i].getName() == userName) {
			index = i;
			type = '2';
		}
	}

	for (int i = 0; i < admins.size(); i++) {
		if (admins[i].getName() == userName) {
			index = i;
			type = '3';
		}
	}


	switch (type) {
		case '1':
			if (students[index].borrowsListSize() > 0) {
				cout << "The Student cannot be deleted they own book copies" << endl;
				return;
			}

			for (int i = 0; i < students[index].borrowsListSize(); i++) {
				idList.push_back(students[index].getReserveValue(i));
			}

			for (int i = 0; i < copiez.size(); i++) {
				for (int j = 0; j < idList.size() j++) {
					if (copiez[i].getID() == idList[j]) {
						for (int k = 0; k < copiez[i].getReserverSize(); k++) {
							if (copiez[i].getReserver(k) == students[userIndex].getName()) {
								copiez[i].removeReserver(k);
							}
						}

					}
				}
			}

			students.erase(students.begin() + index);

			cout << "Student Account Deleted" << endl;

			break;

		case '2':
			if (teachers[index].borrowsListSize() > 0) {
				cout << "The Teacher cannot be deleted they own book copies" << endl;
				return;
			}

			for (int i = 0; i < teachers[index].borrowsListSize(); i++) {
				idList.push_back(teachers[index].getReserveValue(i));
			}

			for (int i = 0; i < copiez.size(); i++) {
				for (int j = 0; j < idList.size() j++) {
					if (copiez[i].getID() == idList[j]) {
						for (int k = 0; k < copiez[i].getReserverSize(); k++) {
							if (copiez[i].getReserver(k) == teachers[userIndex].getName()) {
								copiez[i].removeReserver(k);
							}
						}

					}
				}
			}

			teachers.erase(teachers.begin() + index);

			cout << "Teacher Account Deleted" << endl;

			break;

		case '3':
			admins.erase(admins.begin() + index);

			break;

		default:
			cout << "No user exists with that name" << endl;
	}

}


void addBook() {
	long long ISBN;
	string title, author, category;
	bool flag;


	cout << "Enter Parameters of book to add it to database" << endl;
	cout << "Enter ISBN- ";
	cin >> ISBN;
	cout << "Enter Title- ";
	cin >> title;
	cout << "Enter Author- ";
	cin >> author;
	cout << "Enter Category- ";
	cin >> category;

	
	//Check if ISBN is on book database
	for (int i = 0; i < books.size(); i++) {
		if (books[i].getISBN() == ISBN) {
			flag = true;
		}
	}

	
	if (flag) {//If it is add copy 
		copies newCopy(ISBN);
		copiez.push_back(newCopy);
	}
	else{//If it isnt add to daatabase and add copy
		book newBook(ISBN, title, author, category);
		copies newCopy(ISBN);
		books.push_back(newBook);
		copiez.push_back(newCopy);
	}
}


void deleteBook() {    // Function deletes a copy of a book from the list of copies
	int ID;

	cout << "Enter the ID of the copy to be deleted: ";
	cin >> ID;
	for (int i = 0; i < copiez.size(); i++) {
		if (ID == copiez[i].getID()) {
			if (!copiez[i].getAvailable()) {
				cerr << "Cannot delete, copy is lent out." << endl;
				break;
			}
			else {
				copiez.erase(copiez.begin() + i);
				cout << "Copy deleted." << endl;
				break;
			}
		}
	}
}




void renewBook() {
	int id, index;
	bool hasOverDue = false;
	cout << "Which book would you like to renew?" << endl;
	cin >> id;
	for (int i = 0; i < students[userIndex].getBorrowedSize(); i++) {	// Loops for the number of borrowed books
		for (int j = 0; j < copiez.size(); j++) {
			if (copiez[j].getID() == students[userIndex].getBorrowedBook(j)) {	//This is true if they have borrowed this book
				if (copiez[j].getExpireDate() < adjustTime(getTime())) {				//this is true if the book is overdue
					hasOverDue = true;
				}
			}
		}
	}

	if (!hasOverDue) {
		for (int i = 0; i < copiez.size(); i++) {     // Searching for the matching ID
			if (copiez[i].getID() == id) {
				index = i;
				break;
			}
		}
		if (copiez[index].getReserverSize() == 0) {
			copiez[index].setBorrowDate(adjustTime(getTime()));
			if(userType ==1)
				copiez[index].setExpireDate(adjustTime(getTime()) + 86400 * students[userIndex].getMaxTime());
			if(userType ==2)
				copiez[index].setExpireDate(adjustTime(getTime()) + 86400 * teachers[userIndex].getMaxTime());
		}
		else {
			cout << "Sorry, this book can not be renewed because it is reserved by others." << endl;
		}
	}
	else {
		cout << "You have an overdue book please return that before renewing books." << endl;
	}
}




void reserveBook() {

}


void recommendBook() {

}


