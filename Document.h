//
// Created by Christian Gould on 11/15/2020.
//
/*
 * CHANGELOG
 * --------------
 * -November 16
 * added normal constructor
 * added settitle
 * added setauthor
 * added setid
 * added setBody
 * added getauthor
 * added getid
 * added getbody
 * added gettitle
 * added outformat
 * -December 3
 * added namespace due to clash with processjson
 * -December 4
 * added operator+=
 * added operator <<
 * added calcless
 * -December 5
 * added RelevancyRank
 * added getscore
 * added incrementscore
 * added resetscore
 * added publish date
 * added publisher
 * added get publisher
 * added get publish date
 * added set publisher
 * added set publish date
 */
#ifndef SEARCH_ENGINE_BEANS_DOCUMENT_H
#define SEARCH_ENGINE_BEANS_DOCUMENT_H
#include <string>
#include <iostream>
#include "hashy.h"
#include "avlTree.h"
#include "DSPair.h"
#include <unordered_map>

using namespace std;
namespace docs {
	class Document {
	public:
		/* Constructors */
		// normal constructor
		Document ();

		// input file construction
		Document(string);

		/* Destructor */
		// normal destructor
		~Document () = default;

		/* Functions */
		// sets the title to given string
		void setTitle (string string1);

		// adds given string to list of authors
		void setAuthor (string string1);

		// sets the id with the given string
		void setId (string string1);

		// sets the body with the given string, and makes it first 300 chars.
		void setBody (string string1);

		// returns the vector of strings of the authors
		vector<string> getAuthor ();

		// returns the id
		string getId ();

		// returns the first 300 characters of the body
		string getBody () const;

		// returns the title
		string &getTitle ();

		// outputs the document in the correct format for persistence files to the given output file
		void outFormat(ofstream&);

		// returns a double which represents the relevancy of this document to that word
		double relevancyRank(const string&);

		// returns he current score of this document
		double getScore();

		// sets the score of this document to this integer
		void setScore(double);

		// increments the current score by the given integer
		void incrementScore(double);

		// resets the score of this document to 0
		void resetScore();

		string getPublish_Date();

		string getPublisher();

		void setPublish_Date(string);

		void setPublisher(string);
		/* Operators */
		// outputs all the information needed to be outputed to the given output stream
		friend ostream& operator<< (ostream &os, const Document&);
		// adds a single word to this document (for use in the doc parser)
		Document &operator+= (const string &toAdd);

		// a replacement for the < operator, and returns true if this document is less relevant to the given word than the right hand side doc
		bool calcLess (Document &rhs, const string &);

		bool operator== (const Document&) const;

		bool operator< (const Document&) const;

		bool operator> (const Document&) const;

	private:
		double score;
		vector<string> author;
		string title;
		string id;
		string body;
		string publish_date;
		string publisher;
		// for term analysis
		//hashy<string, int>* words_To_Freq;
		unordered_map<string,int> words_To_Freq;
		pair<string, int> most_Freq_Word;
		/* Private member functions */
		// used for calculating relevancy
		double calcFactor (string factor);
		// used for printing to a persistence file
		string printForPers();
	};
}
#endif //SEARCH_ENGINE_BEANS_DOCUMENT_H
