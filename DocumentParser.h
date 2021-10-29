//
// Created by Christian Gould on 11/15/2020.
//
/*
 * CHANGELOG
 * -----------------
 * -November 16
 * added private variables
 * made skeleton of each of the methods needed
 * -December 2
 * work on getting persistence files to work
 * -December 3
 * writing persistence files now working, need to get reading working
 * -December 5
 * added store current index
 * added pull stored index
 * added get first two stats
 * added clearIndex
 */
#ifndef SEARCH_ENGINE_BEANS_DOCUMENTPARSER_H
#define SEARCH_ENGINE_BEANS_DOCUMENTPARSER_H
#include <locale>
#include <codecvt>
#include <string>
#include "avlTree.h"
#include "DSPair.h"
#include "Document.h"
#include <stack>

using namespace std;

class DocumentParser {
private:
    avlTree<string> stopWords;
    stack<DSPair<string,docs::Document*>>* storage;
    int numArticles;
    int numWords;
public:
	/* Constructors */
	// normal constructor
    DocumentParser();
    /* Destructors */
    // default destructor, because storage will be initialized and deleted in store_Current_Index();
    ~DocumentParser() = default;
    /* Functions */
    // processes all of the json files in the given directory
    stack<DSPair<string,docs::Document*>> processJSON(const char* directory);
    // processes all of the files in the persistence file if it exists. Returns an empty stack if does not exist
	stack<DSPair<string,docs::Document*>> pull_Stored_Index();
	// removes the punctuation from the given word
	void remPunct(string& remove);
	// sets all the letters in the given string to lower case
    void toLower(string&);
    // stores the current index into a persistence file
    void store_Current_Index();
    /*
     * returns the first two stats in this format:
     * total number of individual articles indexed: (some number here)
     * average number of words indexed per article: (some number here)
     */
	string get_first_two_stats();
	// clears the current persistence file
	void clearIndex();
	/* Operators */
};


#endif //SEARCH_ENGINE_BEANS_DOCUMENTPARSER_H
