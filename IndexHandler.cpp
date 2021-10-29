//
// Created by Kellen Long on 11/15/2020.
//

#include "IndexHandler.h"
#include <stack>
#include <set>
#include "porter2_stemmer.h"
#define DEBUG_IND_HAND false
#define SHOW_DOC_NUMBERS false
#define DEBUG_KELLEN false
using namespace docs;
IndexHandler::IndexHandler(const char* currDirectory, bool pers){
	int counter = 2;
	int docCounter = 0;
//	cout << "your fate lies with: " << twoo.size() << endl;
	stack<DSPair<string,docs::Document*>> stackOfAll = (pers)? parser.pull_Stored_Index(): parser.processJSON(currDirectory);
	if(!pers) writeToFile();
	if(DEBUG_IND_HAND) cout << "The whole stack holds " << stackOfAll.size() << " items" << endl;
    // pop off the naming pair, holds : "WORDS","WORDS"
	if(DEBUG_IND_HAND) cout << stackOfAll.top().first << " is the word " << endl;
	// put the first element on independently to avoid seg fault.
    // DSPair<string, vector<string>> temp (stackOfAll.top ().first, vector<string>{stackOfAll.top().second});
    // wordsToDocs.insert(temp);
    if(stackOfAll.top().first=="FAILED")throw exception();
    set<Document*> tester;
    Document* focus = stackOfAll.top().second;
    while(!stackOfAll.empty()) {
    	if(DEBUG_IND_HAND) cout << stackOfAll.top().first << " is the word " << endl;
	    if(DEBUG_IND_HAND) cout << "With the document: \n" << *stackOfAll.top().second << endl;
	    if(stackOfAll.top().first=="NEWDOC"){
            auto auths = focus->getAuthor();
            for(int i = 0; i < auths.size(); i++){
                authorsToDocs[auths[i]] = vector<Document*>{focus};
            }
		    while(!stackOfAll.empty() && stackOfAll.top().first == "NEWDOC")
            stackOfAll.pop();
            if(!stackOfAll.empty())focus = stackOfAll.top().second;
        }else {
	    	//if(stackOfAll.top().first == "virus") cout << "virus found! focus is: " << *focus << endl;
	    	//cout << "putting " << stackOfAll.top().first << "into " << *focus << endl;
            addWord(stackOfAll.top().first);
	    	wordsToDocs[stackOfAll.top().first].push_back(focus);
            if(!stackOfAll.empty())stackOfAll.pop();
        }
    }
}
void IndexHandler::unionAuthor(string s){
	auto it = authorsToDocs.find(s);
    if(it != authorsToDocs.end()) {
        for (int i = 0; i < authorsToDocs[s].size(); i++) {
            matches.push_back(authorsToDocs[s].at(i));
        }
    }
}

void IndexHandler::excludeAuthor(string s){
        auto it = authorsToDocs.find(s);
        for(int i = 0; i < matches.size(); i++) {
            for(int j = 0; j < it->second.size(); j++){
                if(matches[i]==it->second[j]){
                    matches[i]->resetScore();
                    matches.erase(matches.begin() + i);
                }
            }
        }
}

void IndexHandler::restrictAuthor(string s) {
    bool add = matches.size() == 0;
    auto it = authorsToDocs.find(s);
    if(it != authorsToDocs.end()) {
        if(add){
            for (int i = 0; i < it->second.size(); i++) {
                matches.push_back(it->second[i]);
                //it->second[i]->incrementScore(it->second[i]->relevancyRank(s));
            }
        }else{
            for(int i = 0; i < matches.size();i++){
                bool has = false;
                for(int j = 0; j < it->second.size();j++){
                    if(matches[i]==it->second[j])has=true;
                }
                if(!has) {
                    matches[i]->resetScore();
                    matches.erase(matches.begin() + i);
                }
            }
        }
    }
}

void IndexHandler::unionWord(string s){
    auto it = wordsToDocs.find(s);
    if(it != wordsToDocs.end()) {
        for (int i = 0; i < it->second.size(); i++) {
            matches.push_back(it->second[i]);
            it->second[i]->incrementScore(it->second[i]->relevancyRank(s));
        }
    }
}

void IndexHandler::excludeWord(string s){
    bool add = matches.size() == 0;
    auto it = wordsToDocs.find(s);
    for(int i = 0; i < matches.size(); i++) {
        for (int j = 0; j < it->second.size(); j++) {
            if (matches[i] == it->second[j]) {
                matches[i]->resetScore();
                matches.erase(matches.begin() + i);
            }
        }
    }
}

void IndexHandler::restrictWord(string s){
	if(DEBUG_IND_HAND) cout << "********" << endl;
	if(DEBUG_IND_HAND) cout << "*******" << endl;
    bool add = matches.size() == 0;
    if(DEBUG_KELLEN)cout<<"matches.size is: "<<matches.size()<<endl<<"add is:"<<add<<endl;
    auto it = wordsToDocs.find(s);
    if(it != wordsToDocs.end()) {
        if(add){
            if(DEBUG_KELLEN)cout<<"adding words, second.size is "<<endl<<it->second.size()<<endl;
            for (int i = 0; i < it->second.size(); i++) {
                if(DEBUG_KELLEN)cout<<"adding "<<it->second[i]->getTitle()<<endl;
                matches.push_back(it->second[i]);
                it->second[i]->incrementScore(it->second[i]->relevancyRank(s));
            }
        }else{
            for(int i = 0; i < matches.size();i++){
                bool has = false;
                for(int j = 0; j < it->second.size();j++){
                    if(matches[i]==it->second[j])has=true;
                }
                if(!has){
                    matches[i]->resetScore();
                    matches.erase(matches.begin() + i);
                }
            }
        }
    }
}

void IndexHandler::writeToFile(){
	parser.store_Current_Index();
}

void IndexHandler::clearIndex(){
    matches.clear();
}

void IndexHandler::printIndex(){
    if(matches.size()>0) {
        cout<<"found documents:"<<endl;
        cleanup();
        for (int i = 0; i < 15 && i < matches.size(); i++) {
            cout << i+1 <<". "<<*(matches.at(i)) << endl;
        }
    }else{
        cout<<"could not find any documents with those parameters."<<endl;
    }
}

void IndexHandler::printAllStats(){
    cout<<parser.get_first_two_stats();
    cout<<"Number of unique words: "<<wordsToDocs.size()<<endl;
    cout<<"Number of unique authors"<<authorsToDocs.size()<<endl;
    cout<<"50 most frequent words:";
    frequent50();
}

void IndexHandler::clearFile () {
parser.clearIndex();
}

void IndexHandler::cleanup(){
	sort( matches.begin(), matches.end(), comparePtr);
	matches.erase( unique(matches.begin(),matches.end() ), matches.end() );
}
bool IndexHandler:: comparePtr(Document* lhs, Document* rhs){
	return ((*lhs) > (*rhs));
}
void IndexHandler::addWord(string s){
    auto it = wordsToFrequency.find(s);
    if(it != wordsToFrequency.end()) {
        it->second+=1;
    }else{
        wordsToFrequency.insert(pair<string,int>(s,1));
    }
}
void IndexHandler::frequent50(){
    vector<pair<string,int>> nums;
    for(auto& it:wordsToFrequency){
        nums.push_back(it);
    }
    sort(nums.begin(), nums.end(), compareSec);
    for(int i = 0; i < 50; i++){
        cout<<nums[i].first<<" at "<<nums[i].second<<" times"<<endl;
    }
}

bool IndexHandler::compareSec(pair<string,int> a, pair<string,int> b){
    return a.second>b.second;
}

void IndexHandler::chooseFile(int i) {
    cout << matches[i-1]->getBody() << endl;
}