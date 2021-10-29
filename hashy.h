//
// Created by Christian Gould on 11/25/20.
//

/*
 * CHANGELOG
 * -------------------------
 * November 25:
 * -added skeleton
 * -implemented clear function
 * -implemented bucket_count function
 * -implemented bucket_size function
 * -implemented rehash function
 * -implemented hashIt function
 * -implemented erase function
 * -implemented size function
 * -implemented empty function
 * -implemented rehash function
 * -implemented destructor
 * -implemented default constructor
 * -implemented element number constructor
 * -implemented count function
 * -implemented insert function
 * November 26:
 * -implemented copy constructor
 * -implemented copy assignment operator
 * -added documentation
 * -----------------------
 * DOCUMENTATION / USAGE
 * -----------------------
 *
 * =============
 * IN ORDER TO USE: classes t and k must have these operators implemented: operator=,operator==,copy constructor, and operator!=.
 * =============
 *
 * FUNCTIONS:
 * =============
 * Default Constructor: No arguments, will set the number of buckets by default to 5,000.
 * int parameter Constructor: takes int, the int represents an estimated number of items that will be inserted.
 * copy Constructor: self-explanatory
 * clear: no parameters, clears the whole map of all elements
 * int bucket_count(): no parameters, returns the current number of buckets in the hash map
 * void insert(t,k): t and k as parameters, inserts them as a key value pair into the hash map
 * void erase(t): t parameter, deletes the element in the key of t from the hash map
 * bool empty(): no parameters, returns whether the hash map is empty or not
 * int size(): noparameters, returns the number of elements currently in the hashmap
 * int bucket_size(): no parameters, returns the maximum size of the buckets in the hash map(currently 2)
 * void rehash(int): takes integer for new size. NOT THE NEW NUMBER OF BUCKETS, but the new estimated NUMBER OF ELEMENTS.
 * k operator[](t): takes t as parameter. Returns the value which corresponds to the key of t.
 *      -If set up as hashmap[t] = k, then can assign key values with it.
 *      ex.)
 *      hashy<string,string> hasher;
 *      hasher["key"] = "value";
 *    //  ^^^ the above code will store the kv pair of "key","value".
 * operator= (assignment operator): self explanatory
 * operator== (comparison operator): checks all elements for equivalence
 */
#ifndef FLYINGBEANS_HASHY_H
#define FLYINGBEANS_HASHY_H

#define DEBUG_HASH_MAP false
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
using namespace std;

template<class t, class k>
class hashy {

public:
/* constructors */
// default constructor
	hashy () {
		maxBucketSize = 2;
		if(DEBUG_HASH_MAP)
			cout << "in default constr" << endl;
			const int defaultSize = 5000;
		map = new pair<t, k> *[defaultSize];
		for (int i = 0; i < defaultSize; i++) {
			map[ i ] = new pair<t, k>[maxBucketSize];
		}
		//cout << "got to here" << endl;
		mapSize = defaultSize;
		filled_elements = new vector<int>;
		totalElems = 0;
	}

// constructor with estimated number of elements
	hashy (int elems) {
		maxBucketSize = 2;
		if(DEBUG_HASH_MAP)
			cout << "in hash elems" << endl;
			map = new pair<t, k> *[2 * elems];
		for (int i = 0; i < 2 * elems; i++) {
			map[ i ] = new pair<t, k>[maxBucketSize];
		}
		mapSize = 2 * elems;
		filled_elements = new vector<int>;
		totalElems = 0;
	}

// copy constructor
	hashy (const hashy<t, k> &copy) {
		if(DEBUG_HASH_MAP)
cout << "in copy constr" << endl;
			// copy mapsize and map elements
		this->totalElems = copy.totalElems;
		this->mapSize = copy.mapSize;
		this->maxBucketSize = copy.maxBucketSize;
		map = new pair<t,k>*[mapSize];
		for(int i = 0; i < mapSize; i++)
			map[i] = new pair<t,k>[maxBucketSize];
		if(DEBUG_HASH_MAP) cout << "copy filled is: " << copy.filled_elements->size() << endl;
		for (int i = 0; i < copy.filled_elements->size(); i++) {
			if(DEBUG_HASH_MAP)cout << "now i is " << i << endl;
			for (int x = 0; x < maxBucketSize; x++) {
				if(DEBUG_HASH_MAP) cout << "now x is " << x << endl;
				if(copy.map[copy.filled_elements->at(i)][x] == pair<t,k>())
					break;
				else
				map[ copy.filled_elements->at(i) ][ x ] = copy.map[ copy.filled_elements->at(i) ][ x ];
			}
		}
		this->filled_elements = new vector<int> (*copy.filled_elements);
	}

/* destructor */
	~hashy () {
		if(DEBUG_HASH_MAP)
cout << "in destr" << endl;
			for(int i = 0; i < mapSize; i++)
			delete[] map[i];

			delete [] map;
		//delete map;
		delete filled_elements;
	}

/* functions */
	void clear () {
		if(DEBUG_HASH_MAP)
cout << "in clear" << endl;
		for(int i = 0; i < mapSize; i++)
			delete[] map[i];

			delete[] map;
		map = new pair<t, k> *[mapSize];
		for (int i = 0; i < mapSize; i++) {
			map[ i ] = new pair<t, k>[maxBucketSize];
		}
		this->totalElems = 0;
	}

	int bucket_count () {
		if(DEBUG_HASH_MAP)
cout << "in bucket count" << endl;
			return mapSize;
	}

	void insert (const t &type1, const k &type2) {
		if(DEBUG_HASH_MAP)
cout << "in insert" << endl;
			//cout << "inserting now" << endl;
		int at = hashIt (type1) % mapSize;
		//cout << "hash is: " << at << endl;
		// first check to see if there are any elements inside
		if (filled_elements->size () > 0) {
		//	cout << "inside first if" << endl;
			// if the bucket already has an element
			if (binary_search (filled_elements->begin (), filled_elements->end (), at)) {
				//cout << "binary search was good" << endl;
				int count = 1;
				// find a slot that is empty
				while (count < maxBucketSize) {
					//cout << "in the while loop" << endl;
					if (map[ at ][ count ] == pair<t, k> ()) {
						//cout << "in nested if" << endl;
						map[ at ][ count ] = pair<t, k> (type1, type2);
						totalElems++;
						++count;
						break;
					}
					++count;
				}
				// if the bucket is full after insertion
				if (count == maxBucketSize) {
					//cout << "rehashing now" << endl;
					rehash (mapSize);
				}
				return;
			}
		}
		// if the bucket does not already have an element
		// add the new element
		map[ at ][ 0 ] = pair<t, k> (type1, type2);
		// update the number of filled elements in the map
		filled_elements->push_back (at);
		totalElems++;
		std::sort (filled_elements->begin (), filled_elements->end ());
	}

	void erase (t erMe) {
		if(DEBUG_HASH_MAP)
cout << "in erase" << endl;
			if (binary_search (filled_elements->begin (), filled_elements->end (), (hashIt (erMe) % mapSize))) {
			map[ hashIt (erMe) % mapSize ][ 0 ] = pair<t,k>();
			totalElems--;
		} else {
			// if there is not an element to erase, do nothing
			return;
		}
	}

	bool empty () {
		if(DEBUG_HASH_MAP)
cout << "in empty" << endl;
			return totalElems == 0;
	}

	int size () const {
		if(DEBUG_HASH_MAP)
cout << "in size" << endl;
			return totalElems;
	}

	int bucket_size () {
		if(DEBUG_HASH_MAP)
cout << "in bucket size" << endl;
			return maxBucketSize;
	}

	void rehash (int hashNum) {
		if(DEBUG_HASH_MAP)
cout << "in rehash" << endl;
		this->mapSize = 2 * hashNum;
		if(DEBUG_HASH_MAP)
			cout << "mapsize is now: " << mapSize << endl;
		auto newGuy = new pair<t, k> *[mapSize];

		if(DEBUG_HASH_MAP)
			cout << "got the new guy" << endl;
		for (int i = 0; i < mapSize; i++) {
			newGuy[ i ] = new pair<t, k>[maxBucketSize];
		}
		if(DEBUG_HASH_MAP)
			cout << "got past the for loop" << endl;
		// for each of the buckets
		for (int &filled_element : *filled_elements) {
			if(DEBUG_HASH_MAP)
				cout << "in the second for loop" << endl;
			// for each element in the buckets
			for (int x = 0; x < maxBucketSize; x++) {
				if(DEBUG_HASH_MAP)
					cout << "in the second nested for loop "<< endl;
				newGuy[ hashIt (map[ filled_element ][ x ].first) % this->mapSize ][ x ] = map[ filled_element ][ x ];
			}
		}
		if(DEBUG_HASH_MAP)
			cout << "deleting map now" << endl;
		for(int i = 0; i < mapSize/2; i++)
			delete[] map[i];
		delete[] map;
		this->map = newGuy;
	}

	bool isInside(const t& ex){
		if(DEBUG_HASH_MAP)
cout << "is in isinside" << endl;
			if(this->operator[](ex) == k()){
		return false;
	}
	return true;
	}

/* operators */
	k& operator[] (t at) {
		if(DEBUG_HASH_MAP)
cout << "is in [] operator" << endl;
			// get the position where it is to be put into the array
		int addy = hashIt (at) % mapSize;
		// i is the sub position in the second dimension of the array
		int i = 0;
		// finds the open slot
		while (map[ addy ][ i ] != pair<t, k> () && i < maxBucketSize) {
			//cout << "addy[i].first: " << map[addy][i].first << endl;
			//cout << "addy[i].second: " << map[addy][i].second << endl;
			if (at == map[ addy ][ i ].first) {
				return map[ addy ][ i ].second;
			}
				i++;
		}
		// this is if the user wants to modify the position of "at"
		// ex.) hash[t] = k
		// this is done by returning the reference of the place where it would be if it existed.

		if (i < maxBucketSize) {
			/*
			 * If i is less than max bucket size, then there is an open slot, and we return the spot
			 * where there is an open slot. This code assumes that the value is used.
			 */
			map[ addy ][ i ].first = at;
//			filled_elements->push_back(addy);
//			totalElems++;
			return map[ addy ][ i ].second;
		} else if (i == maxBucketSize) {
			/*
			 * If i is equal to maxbucketsize, then there is already an element in all the
			 * max bucket slots, which by default is 2, so then in order to add this new element,
			 * the hashmap needs to be rehashed to make space, and then call this function
			 * recursively again.
			 */
			rehash (mapSize);
//			addy = hashIt (at) % mapSize;
//			map[ addy ][ i ].first = at;
//			return map[ addy ][ i ].second;
		return this->operator[](at);
		}
		else if (i > maxBucketSize) {
			rehash (mapSize);
			return map[addy][i].second;
		}
		else {throw out_of_range ("error at [] operator in hash map.");}
	}

	hashy<t, k> &operator= (const hashy<t, k> &eq) {
		if(DEBUG_HASH_MAP)
			cout << "is in = operator" << endl;
		// handle self assignment with if statement
		if (!(*this == eq)) {
			for(int i = 0; i < mapSize; i++)
				delete[] this->map[i];
			delete[] this->map;
			delete this->filled_elements;
			this->mapSize = eq.mapSize;
			this->maxBucketSize = eq.maxBucketSize;
			this->map = new pair<t, k>*[mapSize];
			for(int i = 0; i < mapSize; i++){
				map[i] = new pair<t,k>[maxBucketSize];
			}
			// copy all elements of map
			for (int i = 0; i < filled_elements->size (); i++) {
				for (int x = 0; x < maxBucketSize; x++) {
					this->map[ filled_elements->at(i)][ x ] = eq.map[ eq.filled_elements->at(i)][ x ];
				}
			}

			this->filled_elements = new vector<int> (*eq.filled_elements);
			return *this;
		}
		return *this;
	}

	bool operator== (const hashy<t, k> &rhs) const {
		if(DEBUG_HASH_MAP)
cout << "in == operator" << endl;
			if (*filled_elements != *rhs.filled_elements) return false;
		bool returnMe = true;
		for (int i = 0; i < filled_elements->size (); i++) {
			for (int x = 0; x < maxBucketSize; x++) {
				if (!((map[( filled_elements->at(i))])[ x ] == (rhs.map[ (rhs.filled_elements->at(i))])[ x ])) {
					returnMe = false;
				}
			}
		}
		return returnMe;
	}

private:
	/* elements */
	pair<t, k> **map;
	vector<int> *filled_elements;
	hash<t> hasher;
	int mapSize;
	int maxBucketSize;
	int totalElems;
	/* member functions */
	// uses standard hash function in order to get int.
	int hashIt (const t &hashMe) const {
		return abs ((int) hasher (hashMe));
	}

};

#endif //FLYINGBEANS_HASHY_H
