#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

std::vector<book> books;
std::vector<patron> patrons;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){

	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());

}

/* checkout a book to a patron
 * first load books and patrons from disk -
 * make sure patron enrolled (patronid is assigned to a patron in patrons container) -
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT -
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){
	reloadAllData();
	//loading books and patrons from disk

	/*Valid set to false then looks through the vector to see if
	  the patronid is valid.
	  if it is validID will turn to true and break out of the loop
	 */
	bool validID = false;
		for (int i = 0; i < int(patrons.size()) ; i++){
			if (patrons[i].patron_id == patronid){
				validID = true;
				break;
			}
		}

		if (patrons[patronid].patron_id == MAX_BOOKS_ALLOWED_OUT){
						return TOO_MANY_OUT;
		}

		if (!validID){
				return PATRON_NOT_ENROLLED;
		}

		books[bookid].loaned_to_patron_id = patronid;
		books[bookid].state = OUT;

	saveBooks(books, BOOKFILE.c_str());
	savePatrons(patrons, PATRONFILE.c_str());

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk -
 * make sure book in collection (bookid is assigned to a book in books container) -
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out-
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN; -
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){
	reloadAllData();

		int book_num = -1;
		for (int i = 0; i < int(books.size()); i++){
			if(books[i].book_id == bookid){
				book_num = i;
				break;
			}
		}

		if (book_num == -1){
			return BOOK_NOT_IN_COLLECTION;
		}

		for (int i = 0; i < int(patrons.size()); i++){
			if(patrons[i].patron_id == books[book_num].loaned_to_patron_id){
				patrons[i].number_books_checked_out--;
				break;
			}
		}
		books[book_num].loaned_to_patron_id = NO_ONE;
		books[book_num].state = IN;
		saveBooks(books, BOOKFILE.c_str());
		savePatrons(patrons, PATRONFILE.c_str());
		return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){
	reloadAllData();

	int newPatronID = patrons.size();
	patron newPatron;
	newPatron.name = name;
	newPatron.number_books_checked_out = NONE;
	newPatron.patron_id = newPatronID;


	patrons.push_back(newPatron);
	savePatrons(patrons, PATRONFILE.c_str());

	return newPatronID;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	reloadAllData();
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	reloadAllData();
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	reloadAllData();

	int booksOut = -1;
		for(int i = 0 ; i < int(patrons.size()) ; i++){
			if(patrons[i].patron_id == patronid){
				booksOut = patrons[i].number_books_checked_out;
							return booksOut;
			}
		}

		return PATRON_NOT_ENROLLED;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	reloadAllData();

	for (int i = 0; i < int(patrons.size()); i++){
				if (patrons[i].patron_id == patronid){
					return SUCCESS;
				}
			}

	return PATRON_NOT_ENROLLED;
}

