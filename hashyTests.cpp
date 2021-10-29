//
// Created by christian on 11/25/20.
//

#include "hashy.h"
#include "catch.hpp"
TEST_CASE("hash class tests"){
	hashy<string,string> tester;
	string* strings = new string[6];
	strings[0] = "stringOne";
	strings[1] = "stringTwo";
	strings[3] = "hashmaps";
	strings[4] = "are";
	strings[5] = "fun";
	// returns whether the hash table is empty
	SECTION("Empty function"){
	CHECK(tester.empty());
	}
	// returns the total size of the hash table
	SECTION("size function"){
	CHECK(tester.size() == 0);
	tester.insert("hello","hello2");
	CHECK(tester.size() == 1);
	for(int i = 0; i < 10; i++){
		string one = " ";
		one += i;
		string two = " ";
		two += i+2;
		tester.insert(one,two);
		CHECK(tester.size() == i+2);
	}
	}
	// access the elements at this section
	SECTION("operator[]"){
		// testing getting elements with [] operator
		tester.insert(strings[0],strings[1]);
		tester.insert(strings[2],strings[3]);
		tester.insert(strings[4],strings[5]);
		CHECK(tester[strings[0]] == strings[1]);
		CHECK(tester[strings[2]] == strings[3]);
		CHECK(tester[strings[4]] == strings[5]);
	}
	// inserts an element
	SECTION("insert function"){
	tester.insert("hello","second");
	tester.insert("hey there", "hello again");
	CHECK(tester.size() == 2);
	}
	// erases an element at a certain place
	SECTION("erase function"){
	tester.insert("okay","oh man");
	CHECK(tester.size() == 1);
	tester.erase("okay");
	CHECK(tester.size() == 0);
	}
	// the container is emptied, with he size being 0 at the end
	SECTION("clear function"){
	tester.insert("one","two");
	tester.insert("three","four");
	tester.insert(strings[0],strings[1]);
	tester.insert(strings[2],strings[3]);
	CHECK(tester.size() == 4);
	tester.clear();
	CHECK(tester.size() == 0);
	}
	// returns the total number of buckets
	SECTION("bucket_count"){
	tester.insert(strings[0],strings[1]);
	CHECK(tester.bucket_count() == 5000);
	tester.clear();
	CHECK(tester.bucket_count() == 5000);
	}
	// returns the size of all the buckets currently
	SECTION("bucket_size"){
	CHECK(tester.bucket_size() == 2);
	}
	// will rehash the hashmap using the given number of buckets
	SECTION("rehash"){
		tester.insert("hey","hey");
	CHECK(tester.bucket_count() == 5000);
	CHECK(tester.size() == 1);
	tester.rehash(tester.bucket_count());
	CHECK(tester.bucket_count() == 10000);
	CHECK(tester.size() == 1);
	}
	SECTION("copy constructor"){
		tester.insert("k","l");
	hashy<string,string> copyMe(tester);
	CHECK(tester == copyMe);
	}
	SECTION("empty function"){
		CHECK(tester.empty());
		tester.insert("hey","hey");
		CHECK(!tester.empty());
		tester.clear();
		CHECK(tester.empty());
	}
	SECTION ("assignment operator"){
		tester.insert(strings[0],strings[1]);
		tester.insert(strings[2],strings[3]);
		tester.insert(strings[4],strings[5]);
	hashy<string,string> copyMe = tester;
	CHECK(tester[strings[0]] == strings[1]);
	CHECK(tester[strings[2]] == strings[3]);
	CHECK(tester[strings[4]] == strings[5]);
	}
	SECTION("int constructor tests"){
		hashy<string,string> compare(5);
		CHECK(compare.bucket_count() == 10);
	}
	SECTION("comparison operator tests"){
		hashy<string,string> compare;
		for(int i = 0; i < 6; i++){
			compare.insert("key",strings[i]);
			tester.insert("key",strings[i]);
			CHECK(compare == tester);
		}
		// for when they are not equal
		compare.insert("second","key");
		CHECK(!(compare == tester));
	}
	SECTION("copy constructor tests"){
		hashy<string,string> compare(tester);
		CHECK(compare == tester);
		compare.insert("hello","world");
		CHECK(!(compare == tester));
	}
	SECTION("isInside tests"){
		tester.insert(strings[0],strings[1]);
		tester.insert(strings[2],strings[3]);
		tester.insert(strings[4],strings[5]);
		CHECK(tester.isInside(strings[0]));
		CHECK(tester.isInside(strings[2]));
		CHECK(tester.isInside(strings[4]));
		CHECK(!tester.isInside("hey there"));
	}
}
