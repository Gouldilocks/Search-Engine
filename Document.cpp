//
// Created by Christian Gould on 11/26/20.
//

#define debugDoc false
#define thing (char)31
#include "Document.h"
using namespace docs;
void Document::setAuthor (string string1) {
this->author.push_back(string1);
}

void Document::setId (string string1) {
this->id = string1;
}

void Document::setBody (string string1) {
body += string1;
}

vector<string> Document::getAuthor () {
	return author;
}

string Document::getId () {
	return id.substr(0,id.size()-5);
}

string Document::getBody () const {
	if(body.size() >= 1500)
		return body.substr(0,1500);
	else
		return body.substr(0,body.size());
}

ostream &docs::operator<< (ostream &os, const Document &k) {
	os << "Title: " << k.title << endl;

	os << "First Author: " << k.author.at(0) << endl;

	os << "Date published: " << k.publish_date << endl;

	os << "publisher: " << k.publisher << endl;

	os << "score: " << k.score << endl;

	return os;
}
void Document::setTitle (string string1) {
this->title = string1;
}

string& Document::getTitle () {
	return this->title;
}

Document& Document::operator+= (const string &toAdd) {
	// add one to the word toAdd if it doesn't exist, else add one to freq.
	words_To_Freq[toAdd] += 1;
	if (most_Freq_Word.first.empty()) {
		//cout << "if empty" << endl;
		most_Freq_Word = pair<string, int> (toAdd, 1);
	}
	else if(most_Freq_Word.first == toAdd) {
		//cout << "most freq" << endl;
		most_Freq_Word.second++;
	}
	else if((words_To_Freq)[toAdd] > most_Freq_Word.second) {
		//cout << "setting" << endl;
		most_Freq_Word = pair<string, int> (toAdd, (words_To_Freq)[ toAdd ]);
	}
	//cout << "end of += op" << endl;
	return *this;
}

bool Document::calcLess (Document &rhs,const string& term) {
return this->calcFactor(term) < rhs.calcFactor(term);
}

double Document::calcFactor (string term) {
	/*
 * Using term frequency analysis
 * Factor = 0.5 + 0.5*(raw term count)/(count for most pop term)
 */
	double factor = 0.5;
	factor += (0.5 * ((double)(words_To_Freq)[term] / (double)most_Freq_Word.second));
	return factor;
}
/*
 * Order:
 * 1.)Authors
 * 2.)Title
 * 3.)Id
 * 4.)
 */
void Document::outFormat (ofstream& out) {
	if(author.size() > 0) {
		out << author.at (0);
	}
for(int i = 1; i < author.size(); i++) {
	if(author.at(i) != "|")
	out << "," << author.at (i);
}
out << thing;

out << title << thing;

out << this->getId() << thing;

out << this->getBody() << thing;

//if(words_To_Freq.size() > 0)
out << printForPers() << thing;

out << most_Freq_Word.first << "," << most_Freq_Word.second << thing;

out << publish_date << thing;

out << publisher << thing;

out << endl;
}

Document::Document (string i) {
	this->score = 0;
	this->author = vector<string>();
	this->title = string();
	this->id = string();
	this->body = string();
	this->words_To_Freq = unordered_map<string,int>();
	this->most_Freq_Word = pair<string,int>();
stringstream ss(i);
string temp;
//list of authors
	getline(ss,temp,thing);
		stringstream authors(temp);
	if(debugDoc)cout << "list of authors: " << endl;
	if(debugDoc)cout << temp << endl;
		while(getline(authors,temp,',')){
			setAuthor(temp);
		}
// title
	getline(ss,temp,thing);
	if(debugDoc)cout << "title: " << endl;
	if(debugDoc)cout << temp << endl;
		this->setTitle(temp);
//	id
	getline(ss,temp,thing);
	if(debugDoc)cout << "id: " << endl;
	if(debugDoc)cout << temp << endl;
		this->setId(temp);
// body
	getline(ss,temp,thing);
	if(debugDoc)cout << "body: " << endl;
	if(debugDoc)cout << temp << endl;
		this->setBody(temp);
// hashtable
	getline(ss,temp,thing);
	if(debugDoc)cout << "hashtable: " << endl;
	if(debugDoc)cout << temp << endl;
		stringstream hasherman(temp);
		// this while loop adds one pair to the hash map until hasherman is ended
		while(getline(hasherman,temp,' ')){
			stringstream pair(temp);
			// first
			getline(pair,temp,',');
			// second
			string temp2;
			getline(pair,temp2);
			// add the pair
			(words_To_Freq)[temp] = stoi(temp2);
		}
// most freq
	getline(ss,temp,thing);
	stringstream spair(temp);
	if(debugDoc)cout << "spair is this: " << endl;
	if(debugDoc)cout << temp << endl;
	getline(spair,temp,',');
	most_Freq_Word.first = temp;
	getline(spair,temp);
	most_Freq_Word.second = stoi(temp);
	// publish date
	getline(ss,temp,thing);
	this->setPublish_Date(temp);
	getline(ss,temp,thing);
	this->setPublisher(temp);
}

double Document::relevancyRank (const string& word) {
	return calcFactor(word);
}

double Document::getScore () {
	return score;
}

void Document::setScore (double a) {
this->score = a;
}

void Document::incrementScore (double a) {
this->score += a;
}

void Document::resetScore () {
this->score = 0;
}

Document::Document () {
	score = 0;
}

string Document::printForPers () {
	string returnMe;
auto it = words_To_Freq.begin();
returnMe += it->first;
returnMe += ",";
returnMe += std::to_string(it->second);
++it;
while(it != words_To_Freq.end()){
	returnMe += " ";
	returnMe += it->first;
	returnMe += ",";
	returnMe += std::to_string(it->second);
			++it;
}
return returnMe;
}

string Document::getPublish_Date () {
	return publish_date;
}

string Document::getPublisher () {
	return publisher;
}

void Document::setPublish_Date (string s) {
	this->publish_date = s;
}

void Document::setPublisher (string s) {
this->publisher = s;
}

bool Document::operator== (const Document &rhs) const {
	return this-> score == rhs.score;
}

bool Document::operator< (const Document &rhs) const {
	return this->score < rhs.score;
}

bool Document::operator> (const Document &rhs) const {
	return this->score > rhs.score;
}

