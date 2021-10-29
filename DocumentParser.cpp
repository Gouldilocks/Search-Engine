//
// Created by Christian Gould on 11/15/2020.
//

/*References
 * ------------
 * https://www.ranks.nl/stopwords : for stop words list (used long list at bottom)
 * https://thispointer.com/converting-a-string-to-upper-lower-case-in-c-using-stl-boost-library/ : for making letters lower case
 * https://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch : for system compatibility
 *https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c : for learning how to clear a file
*/
#include "DocumentParser.h"
#include <string>
#include <set>
// this must be included for windows users
#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include "dirent.h"
#else
#error "Not supported, run on Windows or Linux"
#endif
#include <locale>
#include <codecvt>
#include <algorithm>
#include "rapidFix/include/document.h"
#include <fstream>
#include "porter2_stemmer.h"
#include <unordered_map>
#include <exception>
using namespace std;
#define DEBUG_DOC_PARSE false
#define DEBUG_PUNCT false
#define SHOW_NUMS false

DocumentParser::DocumentParser() {
	numArticles = 0;
	numWords = 0;
	// set up the list of stop words into an avl tree
    ifstream streamy;
    streamy.open("stopWords.txt");
    string word;
    while(getline(streamy,word)){
    	stopWords.insert(word);
    }
}
// needs a constant char in order to know where to go for the files
stack<DSPair<string,docs::Document*>> DocumentParser::processJSON(const char* directoryy) {
	unordered_map<string,string> docID_To_pubInfo;
	avlTree<string> bannedDocs;
	string metadir(directoryy);
	metadir+= "/metadata-cs2341.csv";
	ifstream metadat;
	metadat.open(metadir.c_str());
	// get a whole line
	int numdocs = 0;
	while(getline(metadat,metadir)){
		/*
		 * looking for count == 1 (doc id)
		 * count == 9 (pub date)
		 * count == 11 (journal)
		 */
		int count = 0;
		stringstream sk(metadir);
		string banMe;
		while(getline(sk,metadir,',')){
			if(metadir[0] == '"'){
				string addMe;
				// add the rest of the entry to the piece, indicated by the quotes.
				getline(sk,addMe,'"');
				metadir+= addMe;
				// ignore the trailing comma
				sk.ignore();
			}
			//cout << "current: " << metadir << endl;
			// add the id
			if(count == 1){
				banMe = metadir;
				count++;
				continue;
			}
			// add the pub date
			else if (count == 9){
				if (DEBUG_DOC_PARSE)cout << "storing " << metadir << " for pub date" << endl;
				if (DEBUG_DOC_PARSE)cout << "into " << banMe << endl;
				docID_To_pubInfo[banMe] = metadir;
				docID_To_pubInfo[banMe] += ",";
			}
			// add the journal
			else if (count == 11){
				if (DEBUG_DOC_PARSE)cout << "storing " << metadir << " for journal" << endl;
				docID_To_pubInfo[banMe] += metadir;
				if (DEBUG_DOC_PARSE)cout << "into " << banMe << endl;
			}
			if (metadir == "biorxiv_medrxiv") {
				banMe += ".json";
				bannedDocs.insert(banMe);
			}
			if (DEBUG_DOC_PARSE)cout << metadir << endl;
			count++;
		}
		if (DEBUG_DOC_PARSE)cout << "*********************" << endl;
		numdocs++;
	}
	metadat.close();
	int documentCounter = 0;
	stack<DSPair<string, docs::Document*>> returnMe;
	//stemming::english_stem<> stemmy;
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	rapidjson::Document doc;
	cout << "directory is: " << directoryy << endl;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (directoryy)) != NULL) {
		/*print all the files and directories withing directory */
		while ((ent = readdir (dir)) != NULL) {
			if (SHOW_NUMS)cout << "parsing doc number " << documentCounter << endl;
			// ent->d_name is the name of the file
			/*
			 * The below code will print out all of the lines of all of the files. The file will change with each
			 * loop of the above while loop, and the line number in the json file will change with each
			 * iteration of the below while loop.
			 */
			if (DEBUG_DOC_PARSE)printf ("%s\n", ent->d_name);
			if (strcmp(ent->d_name,".") == 0||strcmp(ent->d_name,"..") == 0||strcmp (ent->d_name, "metadata-cs2341.csv") == 0 || bannedDocs.isInside(string(ent->d_name))) {
				/* Do something with the metadata */
				if (DEBUG_DOC_PARSE)cout << "BANNED" << endl;
				continue;
			}
			string temp (directoryy);
			temp += "/";
			temp += ent->d_name;
			ifstream streamy (temp.c_str (), std::ifstream::binary);
			string wholeFile;
			if (DEBUG_DOC_PARSE) cout << "doc num " << documentCounter << ", with directory: " << temp.c_str () << endl;

			int length;
			streamy.seekg (0, streamy.end);   // go to the end
			length = streamy.tellg ();          // report location (this is the length)
			if (DEBUG_DOC_PARSE) cout << "length = " << length << endl;
			streamy.seekg (0, streamy.beg);   // go back to the beginning
			char *buff = new char[length+1];     // allocate memory for a buffer of appropriate dimension
			streamy.read (buff, length);    // read the whole file into the buffer
			buff[length] = '\0';
			if (DEBUG_DOC_PARSE) cout << "buff = " << buff << endl;
			// parse the document
			doc.Parse (buff);
			streamy.close ();
			if (DEBUG_DOC_PARSE) if (doc.IsObject ()) cout << "ITS AN OBJECT" << endl;

			// parse title if it exists
			auto* pushDoc = new docs::Document();
			// state that there is a new document
			returnMe.push(DSPair<string,docs::Document*>("NEWDOC",nullptr));
			string id(ent->d_name);
			id = id.substr(0, id.size()-5);
			if (DEBUG_DOC_PARSE)cout << "the id is now: " << id << endl;
			stringstream pubs(docID_To_pubInfo[id]);
			getline(pubs,id,',');
			pushDoc->setPublish_Date(id);
			getline(pubs,id);
			pushDoc->setPublisher(id);
			if (DEBUG_DOC_PARSE)cout << "id: " << ent->d_name << endl;
			if (DEBUG_DOC_PARSE)cout << "pub date: " << pushDoc->getPublish_Date() << endl;
			if (DEBUG_DOC_PARSE)cout << "publisher: " << pushDoc->getPublisher() << endl;
			if (DEBUG_DOC_PARSE)cout << "document number: " << documentCounter << endl;
			if (DEBUG_DOC_PARSE)cout << "whole file: " << endl;
			if (DEBUG_DOC_PARSE)cout << "************************************ \n end whole file" << endl;
			string titleeee = doc["metadata"]["title"].GetString();
			if(titleeee.size() < 3){
				if(doc["metadata"]["title"].IsArray()) {
					//cout << ent->d_name << endl;
					titleeee = "";
					for (auto &v : doc[ "metadata" ][ "title" ].GetArray ()) {
						titleeee += v.GetString ();
					}
				}
				// if a document is found without a title, then it is skipped
				else continue;
			}
			if(titleeee.size() < 3) continue;
			// add the title to the document and the id
			pushDoc->setId(ent->d_name);
			pushDoc->setTitle(titleeee);
			// parse authors if they exist
				for (auto &v : doc["metadata"]["authors"].GetArray ()) {
					if (DEBUG_DOC_PARSE)cout << "AUTHOR : ";
					if (DEBUG_DOC_PARSE)cout << v[ "first" ].GetString () << " ";
					if (DEBUG_DOC_PARSE)
						if (v[ "middle" ].Size () > 0)
							cout << v[ "middle" ][ 0 ].GetString () << " ";
					if (DEBUG_DOC_PARSE)cout << v[ "last" ].GetString () << endl;
					string author_Name (v[ "last" ].GetString ());
					// middle and last name are not needed.
//					if (v[ "middle" ].Size () > 0)
//						author_Name += v[ "middle" ][ 0 ].GetString ();
//					author_Name += v[ "last" ].GetString ();
					pushDoc->setAuthor(author_Name);
				}
				// push the abstract MAYBE NOT NEEDED
//				for (auto &v: doc["abstract"].GetArray ()) {
//					if (DEBUG_DOC_PARSE)cout << "ABSTRACT: \n" << v[ "text" ].GetString () << endl;
//					returnMe.push (DSPair<string, string> (v[ "text" ].GetString (), titleeee));
//				}
				for (auto &v : doc["body_text"].GetArray ()) {
					if (DEBUG_DOC_PARSE)cout << "BODY : " << v[ "text" ].GetString () << endl;
					// put the body into a string stream
					stringstream ss (v[ "text" ].GetString ());
					pushDoc->setBody(v["text"].GetString());
					//ss << v[ "text" ].GetString ();
					// parse the body into individual words
					string ind_word;
					while (getline (ss, ind_word, ' ')) {
						if (DEBUG_DOC_PARSE)cout << "original word: " << ind_word << endl;
						// remove punctuation
						remPunct (ind_word);
						if (DEBUG_DOC_PARSE)cout << "removed punctuation: " << ind_word << endl;
						// make the words all lower case
						toLower (ind_word);
						if (DEBUG_DOC_PARSE)cout << "lower cased: " << ind_word << endl;
						// if the word in question is one of the stop words, continue
						if (stopWords.isInside (ind_word) || ind_word.size() < 2) continue;
						if (DEBUG_DOC_PARSE)cout << "not a stop word and before stem: " << ind_word << endl;
						// stem the word using the stem library
						Porter2Stemmer::stem (ind_word);
						//stem(ind_word,stemmy,converter);
						if (DEBUG_DOC_PARSE)cout << "after stem: " << ind_word << endl;
						// add the word to the document
						(*pushDoc)+= ind_word;
						// push the word pair to the stack
						returnMe.push (DSPair<string, docs::Document*> (ind_word, pushDoc));
						numWords++;
					}
				}
			documentCounter++;
		}
			closedir (dir);
	}
		else {
			/*could not open the directory */
			perror ("");
			throw exception ();
		}
		cout << "hhh" << endl;
		this->storage = new stack<DSPair<string,docs::Document*>>(returnMe);
	numArticles = documentCounter;
		return returnMe;
}

void DocumentParser::remPunct(string& remove) {
	if(DEBUG_PUNCT) cout << remove << "  ->  ";
	/*
	 * this method below is sometimes faster OVERALL because the above method has a harder time stemming words with interior punctuation.
	 */
	// traditional:
	for (int i = 0, len = remove.size(); i < len; i++)
	{
		// check whether parsing character is punctuation or not
		if (ispunct(remove[i]))
		{
			remove.erase(i--, 1);
			len = remove.size();
		}
	}
	if(DEBUG_PUNCT) cout << remove << endl;
}
void DocumentParser:: toLower(string& lowMe){
	std::for_each(lowMe.begin(), lowMe.end(), [](char & c) {
		c = ::tolower(c);
	});
}

void DocumentParser::store_Current_Index () {
	clearIndex();
ofstream store;
store.open("non_Volatile_Storage");
// print each of the values
docs::Document* focus = storage->top().second;
focus->outFormat(store);
store << storage->top().first;
while(!storage->empty()){
	if(storage->top().first == "NEWDOC"){
		// pop all the newdocs off
		storage->pop();
		while(!storage->empty() && storage->top().first == "NEWDOC")
			storage->pop();
		if(storage->empty())
			break;
		store << endl;
		store << "NEWDOC";
		focus = storage->top().second;
		// print out the document
		store << endl;
		focus->outFormat(store);
		store << storage->top().first;
		storage->pop();
	}
	store  << "," << storage->top().first;
//	while(!storage->empty() && storage->top().second == nullptr) storage->pop();
//	storage->top().second->outFormat(store);
	storage->pop();
}
store << endl;
delete storage;
}

stack<DSPair<string, docs::Document*>> DocumentParser::pull_Stored_Index () {
	stack<DSPair<string, docs::Document*>> returnMe;
	ifstream store;
	try {
		store.open("non_Volatile_Storage");
		store.seekg (0, store.end);
		int length = store.tellg();
		if(length < 5) throw exception();
	}
	catch (exception& e) {
		cout << "input file does not exist, please try again and parse the input" << endl;
		returnMe.push(DSPair<string,docs::Document*>("FAILED", nullptr));
		return returnMe;
	}
	store.seekg (0, store.beg);
	string wholeDoc;
	string words;
	string word;

// while there are pairs to acquire
while(getline(store,wholeDoc)){
	if(wholeDoc == "NEWDOC") {
		returnMe.push (DSPair<string, docs::Document *> ("NEWDOC", nullptr));
		continue;
	}
	// get the info for the document
	//cout << "new doc was made" << endl;
auto* currDoc = new docs::Document(wholeDoc);
	numArticles++;
// get the list of all the words
getline(store,words);
//cout << "all the words: " << words << endl;
if(DEBUG_DOC_PARSE) cout << "words are : " << words << endl;
stringstream wordStream(words);
// for each of the words, push back a pair
while(getline(wordStream,word, ',')){
	numWords++;
	returnMe.push(DSPair<string,docs::Document*>(word,currDoc));
	if(DEBUG_DOC_PARSE)cout << "for word: " << word << endl;
	if(DEBUG_DOC_PARSE)cout << "the doc is: " << endl;
	if(DEBUG_DOC_PARSE)cout << *currDoc << endl;
}
}
	return returnMe;
}
void DocumentParser::clearIndex () {
	std::ofstream ofs;
	ofs.open("non_Volatile_Storage", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

string DocumentParser::get_first_two_stats () {
	string returnMe;
	returnMe += ("Total number of individual articles indexed: ");
	returnMe += std::to_string(numArticles);
	returnMe += "\n";
	returnMe += ("Average number of words indexed per article (after removal of stop words): ");
	returnMe += std::to_string(numWords / numArticles);
	returnMe += "\n";
	return returnMe;
}
