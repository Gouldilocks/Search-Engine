//
// Created by Kellen Long on 11/15/2020.
//

/*
 * November 14:
 * -------------
 * Added constructor
 * November 18:
 * -------------
 * Added unionAuthor
 * Added excludeAuthor
 * Added restrictAuthor
 * Added unionWord
 * Added excludeWord
 * Added restrictWord
 * Added writeToFile
 * November 27:
 * -------------
 * Added writeToFile
 * Added clearIndex
 * Added printIndex
 * Added printAllStats
 * Added clearFile
 * Added cleanup
 * December 5:
 * --------------
 * Added comparePtr
 * Added addWord
 * Added frequent50
 * Added compareSec
 * Added chooseFile
 */

#ifndef SEARCH_ENGINE_BEANS_INDEXHANDLER_H
#define SEARCH_ENGINE_BEANS_INDEXHANDLER_H
#include <string>
#include "avlTree.h"
#include "DSPair.h"
#include "DocumentParser.h"
#include "Document.h"
#include <vector>
#include <unordered_map>
using namespace std;
using namespace docs;

class IndexHandler {
private:
    unordered_map<string,vector<Document*>> authorsToDocs;
    unordered_map<string,vector<Document*>> wordsToDocs;
    unordered_map<string,int> wordsToFrequency;
    vector<Document*> matches;
    DocumentParser parser;
public:

    //constructor
    IndexHandler(const char*,bool);

    //all of the search engine capabilitites
    void unionAuthor(string);
    void excludeAuthor(string);
    void restrictAuthor(string);
    void unionWord(string);
    void excludeWord(string);
    void restrictWord(string);

    //dealing with persistence files
    void writeToFile();
    void clearIndex();
    void printIndex();

    //printing stats
    void printAllStats();

    //setting up the documents
    void clearFile();
    void cleanup();
    static bool comparePtr(Document*,Document*);
    void addWord(string);
    void frequent50();
    static bool compareSec(pair<string,int>,pair<string,int>);
    void chooseFile(int);
};


#endif //SEARCH_ENGINE_BEANS_INDEXHANDLER_H
