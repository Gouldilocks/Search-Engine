//
// Created by Kellen Long on 11/15/2020.
//

/*
 * November 14:
 * -------------
 * Added constructor
 * Added speedteest
 * Added Decipherinput
 * November 27:
 * --------------
 * Added runword
 * Added q1
 * Added q2
 * Added q3
 */

#ifndef SEARCH_ENGINE_BEANS_USERINTERFACE_H
#define SEARCH_ENGINE_BEANS_USERINTERFACE_H

#include <string>
#include "QueryEngine.h"

using namespace std;

class UserInterface {
public:
    //constructor
    UserInterface(const char*);

    //pass in the raw search and turn to words
    void DecipherInput(char*);
    void DecipherInput(string);
    //process by word
    void runWord(string,string&,bool&);
    void speedTeest(char*);

    //set up the user questions
    int q1();
    bool q2();
    bool q3();
private:
    QueryEngine* engine;
};


#endif //SEARCH_ENGINE_BEANS_USERINTERFACE_H
