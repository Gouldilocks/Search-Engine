//
// Created by gouldilocks on 11/12/20.
//
/*
 * November 14 - added test cases
 */

#include "catch.hpp"
#include "avlTree.h"
#include <string>
#include <iostream>
using namespace std;
TEST_CASE("AVL Tree" "[avlTree.h]") {
	int nums[8] = { 2, 3, 8, 27, 33, 9, 12, 5 };
	avlTree<int> treee;
	// insert function used here, for the rest of the tests, so it does not have its own section
	for (int i = 0; i < 8; i++) {
		treee.insert (nums[ i ]);
	}
	// finds and returns the searched for item
	SECTION("search function") {
		for(int i = 0; i < 8; i++)
		CHECK(treee.search (nums[ i ]) == nums[ i ]);
	}
	SECTION("isInside function"){
		for(int i = 0; i < 8; i++)
			CHECK(treee.isInside(nums[i]));
	}
		// returns the depth of the tree
	SECTION("depth") {
		CHECK(treee.getDepth () == 4);
	}
		// returns the elements in order
	SECTION("inOrder traversal") {
		list<int> ok = treee.inOrderTraversal ();
		int ord[8] = {2,3,5,8,9,12,27,33};
		int i = 0;
		for(auto it = ok.begin(); it != ok.end(); ++it){
			CHECK(ord[i] == *it);
			++i;
		}
		}
		// returns the elements pre ordered- fashion
		SECTION("preOrder traversal") {
			list<int> ok = treee.preOrderTraversal ();
		//	auto it = ok.begin ();
			int ord[8] = {8,3,2,5,27,9,12,33};
			int i = 0;
			for(auto it = ok.begin(); it != ok.end(); ++i, ++it){
			CHECK(ord[i] == *it);
			}
		}
			// returns the elements pre ordered-fashion
		SECTION("postOrder traversal") {
			list<int> ok = treee.postOrderTraversal ();
			int i = 0;
			int ord[8] = {2,5,3,12,9,33,27,8};
			for(auto it = ok.begin(); it != ok.end(); ++it){
				CHECK(ord[i] == *it);
				++i;
			}
		}

}
