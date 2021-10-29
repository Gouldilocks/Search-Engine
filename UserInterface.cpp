//
// Created by Kellen Long on 11/15/2020.
//

#include "UserInterface.h"
#include "porter2_stemmer.h"
#include <iostream>
#include <cstring>
#include <sstream>

UserInterface::UserInterface(const char* input){
    bool rep = true;
    while (rep) {
        switch (q1()) {
            case 1:
                try {
                    engine = new QueryEngine(input, false);
                    q2();
                } catch (exception& e) {break;}
                break;
            case 2:
                try{
                    engine = new QueryEngine(input, true);
                    q2();
                } catch (exception& e) {break;}
                break;
            case 3:
                engine->clearIndex();
                break;
            default:
                rep = false;
                break;
        }
    }
}
int UserInterface::q1(){
    string ans1;
    cout<<"Index or read from persistence file?"<<endl<<"1. Index"<<endl<<"2. Read from persistence file"<<endl<<"3. Clear Persistence File"<<endl<<"4. Quit"<<endl;
    cin >> ans1;
    switch (stoi(ans1)) {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case 3:
            return 3;
            break;
        case 4:
            return 4;
            break;
        default:
            cout<<"not a valid answer. Try again."<<endl;
            return q1();
    }
}
bool UserInterface::q2(){
    string ans;
    cout<<"search or read statistics?"<<endl<<"1. Search"<<endl<<"2. Read statistics"<<endl<<"3. Back"<<endl;
    cin>>ans;
    switch(stoi(ans)){
        case 1:
            while(q3());
            break;
        case 2:
            engine->printStats();
            break;
        case 3:
            cout<<"returning"<<endl;
            return true;
            break;
        default:
            cout<<"not a valid answer. Try again."<<endl;
            return q2();
    }
    return true;
}
bool UserInterface::q3(){
    string ans;
    cout<<"search what?"<< endl;
    cin.ignore();
    getline(cin,ans);
    if(ans=="QUIT")return false;
    DecipherInput(ans);
    engine->outAllFiles();
    cout<<"select file"<<endl;
    cin>>ans;
    engine->select(stoi(ans));
    cin.ignore();
    cin>>ans;
    return true;
}
void UserInterface::DecipherInput(char* d) {
    bool auth = false;
    string mode = "AND";
    char* wrd = strtok(d," ");
    while(wrd!=NULL){
        string str = wrd;
        runWord(str,mode,auth);
        wrd = strtok(NULL," ");
    }
}
void UserInterface::DecipherInput(string d) {
	engine->clearIndex();
    bool auth = false;
    string mode = "AND";
    string wrd;
    int pos = 0;
    while ((pos = d.find(" ")) != std::string::npos) {
        wrd = d.substr(0, pos);
        runWord(wrd, mode, auth);
        d.erase(0, pos + 1);
    }
    runWord(d, mode, auth);
}

void UserInterface::runWord(string str, string& mode, bool& auth){
    if(!auth)Porter2Stemmer::stem (str);
    if(str=="AND")
        mode = "AND";
    else if(str=="OR")
        mode = "OR";
    else if(str=="NOT")
        mode = "NOT";
    else if(str=="AUTHOR")
        auth = true;
    else{
        if(mode=="AND")
            (auth)?engine->andAuthor(str):engine->andQuery(str);
        if(mode=="OR")
            (auth)?engine->orAuthor(str):engine->orQuery(str);
        if(mode=="NOT")
            (auth)?engine->notAuthor(str):engine->notQuery(str);
    }
}
