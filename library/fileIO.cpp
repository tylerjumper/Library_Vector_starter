#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>

#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"

using namespace std;

/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 *             NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 *             SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
    //if books isn't cleared this will clear it
    books.clear();

    //opening up the file and then checking to see if it's open
    ifstream inputFile;
    inputFile.open(filename, ios::in);
    if (!inputFile.is_open()){
        return COULD_NOT_OPEN_FILE;
    }

    /*Load books here*/
    string ln;
    	while (getline(inputFile,ln)){
    		book curBook;
    		string title, author, b_id, patron_num, status;
    		int tmp;


    		stringstream stream(ln);

    		getline(stream, b_id, ',');
    		curBook.book_id = stoi(b_id);
    		getline(stream, title, ',');
    		curBook.title = title;
    		getline(stream, author, ',');
    		curBook.author = author;
    		getline(stream, status, ',');
    		tmp = stoi(status);
    		switch (tmp){
    		case 0:
    			curBook.state= UNKNOWN;
    			break;
    		case 1:
    			curBook.state = IN;
    			break;
    		case 2:
    			curBook.state = OUT;
    			break;
    		}
    		getline(stream, patron_num, ',');
    		curBook.loaned_to_patron_id = stoi(patron_num);

    		books.push_back(curBook);

    	}
    	inputFile.close();

    //used if books is empty
    if(books.empty()){
        return NO_BOOKS_IN_LIBRARY;
    }

    inputFile.close();
    return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 *             NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 *             SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
    	//used if books is empty
        if(books.empty()){
            return NO_BOOKS_IN_LIBRARY;
        }

        ofstream outputFile;
        outputFile.open(filename, ios::in);

        //checks if file is open
        if (!outputFile.is_open()){
                return COULD_NOT_OPEN_FILE;
        }

        for (int i = 0; i < int(books.size()); i++){
        	outputFile << books[i].book_id << "," << books[i].title << "," << books[i].author
        	<< "," << books[i].state << "," << books[i].loaned_to_patron_id << endl;
        }

    outputFile.close();
    return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 *             NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 *             SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
    patrons.clear();

    ifstream inputFile;
    inputFile.open(filename, ios::in);
    if (!inputFile.is_open()){
            return COULD_NOT_OPEN_FILE;
    }

    /*Load books here*/
    string line;
    	while (getline(inputFile,line)){
    		patron currPatron;
    		string pat_id, name, num_b_out;

    		stringstream stream(line);

    		getline(stream, pat_id, ',');
    		currPatron.patron_id = stoi(pat_id);
    		getline(stream, name, ',');
    		currPatron.name = name;
    		getline(stream, num_b_out, ',');
    		currPatron.number_books_checked_out = stoi(num_b_out);

    		patrons.push_back(currPatron);

    	}

    if (patrons.empty()){
            return NO_PATRONS_IN_LIBRARY;
        }

    inputFile.close();
    return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 *             NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 *             SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{

    if (patrons.empty()){
        return NO_PATRONS_IN_LIBRARY;
    }

    ofstream outputFile;
    outputFile.open(filename);

    if (!outputFile.is_open()){
        return COULD_NOT_OPEN_FILE;
    }

    for (int i = 0; i < int(patrons.size()); i++){
    	outputFile << patrons[i].patron_id << "," << patrons[i].name
    			<< "," << patrons[i].number_books_checked_out << endl;
            }


    outputFile.close();
    return SUCCESS;
}
