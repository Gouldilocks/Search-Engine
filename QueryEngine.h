//
// Created by Kellen Long on 11/15/2020.
//

/*
 * November 14:
 * -------------
 * Added constructor
 * Added speedtest
 * November 18:
 * ---------------
 * Added andQuery
 * Added orQuery
 * Added notQuery
 * Added andAuthor
 * Added orAuthor
 * Added notAuthor
 * Added chooseResult
 * November 27:
 * ----------------
 * Added clearIndex
 * Added parseCorpus
 * Added openPersistenceFile
 * Added savePersistenceFile
 * Added printStats
 * Added outAllFiles
 * Added select
 */
#ifndef SEARCH_ENGINE_BEANS_QUERYENGINE_H
#define SEARCH_ENGINE_BEANS_QUERYENGINE_H

#include <string>
#include "IndexHandler.h"

using namespace std;

class QueryEngine {
public:
    //constructor
    QueryEngine(const char*,bool);

    void clearIndex();
    void parseCorpus();

    //working with the persistence files
    bool openPersistenceFile();
    void savePersistenceFile(string);

    //all of the word processing
    void andQuery(string);
    void orQuery(string);
    void notQuery(string);
    void andAuthor(string);
    void orAuthor(string);
    void notAuthor(string);

    //finding the result the user chooses
    void chooseResult(string);
    void speedTest(string);

    //printing stats and selecting documents
    void printStats();
    void outAllFiles();
    void select(int);
private:
    IndexHandler* handler;
};


#endif //SEARCH_ENGINE_BEANS_QUERYENGINE_H
