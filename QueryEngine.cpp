//
// Created by Kellen Long on 11/15/2020.
//

#include "QueryEngine.h"

QueryEngine::QueryEngine(const char* charry, bool pers) {
    try {
        handler = new IndexHandler(charry, pers);
    } catch (exception& e) {
        throw e;
    }
}

void QueryEngine::clearIndex(){
handler->clearIndex();
}

void QueryEngine::parseCorpus(){

}

bool QueryEngine::openPersistenceFile(){
    return false;
}

void QueryEngine::savePersistenceFile(string s){

}

void QueryEngine::andQuery(string s){
    handler->restrictWord(s);
}

void QueryEngine::orQuery(string s){
    handler->unionWord(s);
}

void QueryEngine::notQuery(string s){
    handler->excludeWord(s);
}

void QueryEngine::andAuthor(string s) {
    handler->restrictAuthor(s);
}

void QueryEngine::orAuthor(string s) {
    handler->unionAuthor(s);
}

void QueryEngine::notAuthor(string s) {
    handler->excludeAuthor(s);
}

void QueryEngine::chooseResult(string s){

}

void QueryEngine::printStats(){
    handler->printAllStats();
}


void QueryEngine::outAllFiles() {
    handler->printIndex();
}
void QueryEngine::select(int i) {
    handler->chooseFile(i);
}