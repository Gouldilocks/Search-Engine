//
// Created by Christian Gould on 11/12/20.
//
/* References
 * -------------
 * https://www.geeksforgeeks.org/optimal-sequence-for-avl-tree-insertion-without-any-rotations/ : for optimal avl insertion (printOrd() function)
 *
 */
/*
 * November 12:
 * -----------------
 * added destructor and shell for a few methods
 * November 14:
 * -----------------
 * added depth function
 * added copy constructor
 * added default constructor
 * added insert function
 * added isInside function
 * added search function
 * added preOrderTraversal function
 * added postOrderTraversal function
 * added inOrderTraversal function
 * added case1Rotation function
 * added case2Rotation function
 * added case3Rotation function
 * added case4Rotation function
 * November 15:
 * -----------------
 * tested and fixed insert function
 * tested and fixed depth function
 * added max function
 * tested and fixed max function
 * November 17
 * -----------------
 * added printOrd function
 */
#ifndef UNTITLED1_AVLTREE_H
#define UNTITLED1_AVLTREE_H
#define DEBUG_AVL_TREE false
#include "avlNode.h"
#include <string>
#include <cstdlib>
#include <list>
#include <iostream>
#include <queue>
using namespace std;
template <class t>
/*
 * For Usage:
 * ----------------------------
 * all objects used in this tree must have:
 * -copy constructor implemented
 * -operator== implemented
 * -operator< implemented
 * -operator= implemented
 *
 *
 * Documentation
 * ----------------------------
 * avlTree<t>(): default constructor sets the root to null
 * avlTree<t>(const avlTree<t>&): copy constructor makes deep copy of all elements
 * insert(const t&): takes t as parameter, puts t into the avlTree
 * t& search(const t&): takes t as a parameter, finds and returns by reference the object searched for. if not found, returns {t();} (default constructor of t)
 * bool isInside(const t&): takes t as a parameter, returns bool if object is in the tree already
 * list<t> inOrderTraversal(): returns a list of the tree in order
 * list<t> preOrderTraversal(): returns a list of the tree pre-ordered
 * list<t> postOrderTraversal(): returns a list of the tree post-ordered
 * void printOrd(): function will print the optimal order of all the elements to be inserted into the avl tree, in order for there to be no rotations
 */
class avlTree{
public:
	/* Iterator functions */

/*Constructors*/
avlTree<t>(){
	if(DEBUG_AVL_TREE) cout << "in default constructor" << endl;
	this-> root = nullptr;
}
avlTree<t>(const avlTree<t>& copy){
	// use num 1 to copy all elements
	if(DEBUG_AVL_TREE)cout << "in copy constr" << endl;
	everyElem(root,1);
}
/*Destructor*/
~avlTree<t>(){
	if(DEBUG_AVL_TREE) cout << "in destr" << endl;
	// use num 0 to delete all elements
	if(root != nullptr)
	everyElem(root,0);
	else ;
}
/*Functions*/
void insert(const t& insertMe){
	if(DEBUG_AVL_TREE) cout << "in insert, inserting " << endl;
	inserter(insertMe,root);
}
int getDepth(){
	if(DEBUG_AVL_TREE) cout << "in get Depth" << endl;
	return depth(this->root);
}
// searches for and returns an element. if the element is not found, then it returns a generic form
t& search(const t& searchMe){
	return searchRecur(searchMe,root);
}
list<t> inOrderTraversal(){
	if(DEBUG_AVL_TREE) cout << "in inOrder" << endl;
list<t> returnMe;
orderTraversal(root,returnMe);
return returnMe;
}
list<t> preOrderTraversal(){
	if(DEBUG_AVL_TREE) cout << "in preorder" << endl;
	list<t> returnMe;
	preTraversal(root,returnMe);
	return returnMe;
}
list<t> postOrderTraversal(){
	if(DEBUG_AVL_TREE) cout << "in postorder" << endl;
	list<t> returnMe;
	postTraversal(root,returnMe);
	return returnMe;
}
int size(){
	int returnMe = 0;
	sizeTrav(root,returnMe);
	return returnMe;
}
bool isInside(const t& findMe){
	if(DEBUG_AVL_TREE) cout << "in isInside" << endl;
	return boolRecur(findMe,root);
}
void printOrd(){
	printLevelOrder(root);
}
/*Operators*/

private:
	/* Elements */
	avlNode<t>* root;
	/* Member Functions */

	// function which will print out the order in which the elements should be added in order to make no swaps happen on the avl tree
	void printLevelOrder(avlNode<t> *root, ofstream& output)
	{
		if (root == NULL)  return;

		queue<avlNode<t> *> q;
		q.push(root);

		while (!q.empty())
		{
			avlNode<t> *node = q.front();
			output << node->getData() << endl;
			q.pop();
			if (node->getLeft() != NULL)
				q.push(node->getLeft());
			if (node->getRight() != NULL)
				q.push(node->getRight());
		}
	}
	/*
	 * function which changes what it does, based upon what num is
	 * if num == 0, it will delete
	 * if num == 1, it will insert element
	 */
	void everyElem(avlNode<t>*& p,int num){
		if(p->getRight() == nullptr && p->getLeft() == nullptr){
			switch(num){
				case 0:{
					delete p;
					break;
				}
				case 1:{
					this->insert(p->getData());
					break;
				}
				// if there is a problem here, just throw an exception
				default:
					throw exception();
			}
			return;
		}
		else {
			// if the right side is not null
			if(p->getRight() != nullptr){
				everyElem(p->getRight(),num);
			}
			// if the left side is not  null
			if(p->getLeft() != nullptr){
				everyElem(p->getLeft(),num);
			}
		}
	}
	void inserter(const t& x, avlNode<t>*& tt){
		if(tt == nullptr){
			tt = new avlNode<t>(x);
		return;
	}
		// if x is less than the data, then insert it to the left
		else if(x < tt->getData()){
			inserter(x,tt->getLeft());
			// if this is the node of imbalance
			if(difference(tt) > 1) {
				// case 1
				if(x < tt->getLeft()->getData())
					case1Rotation(tt);
				// case 2
				else
					case2Rotation(tt);
			}
		}
		else if(tt->getData() < x){
			inserter(x,tt->getRight());
			// if this is the node of imbalance
			if(difference(tt) > 1){
				if(x > tt->getRight()->getData())
					case4Rotation(tt);
				else
					case3Rotation(tt);
			}
		}
		/* This is what to do if there are duplicates
		 * Currently there is no implementation for dupes
		 * */
		else ;
		// update the height of the node to the left and right sides new height
		tt->setHeight(max(tt) + 1);
	}
	// assumes that the node is in the tree
	t& searchRecur(const t& search, avlNode<t>*& node){
		// if it is found, return it
		if(node == nullptr){t* ret = new t(); return *ret;}
		if(node->getData() == search) return node->getData();
		// if it is less than the node and left is not null, go left
		else if (search < node->getData()) return searchRecur(search,node->getLeft());
		// if it is greater than the node and right is not null, go right
		else return searchRecur(search,node->getRight());

	}
	bool boolRecur(const t& search, avlNode<t>*& node){
		// if it is found, return it
		if(node == nullptr) return false;
		if(node->getData() == search) return true;
			// if it is less than the node and left is not null, go left
		else if (search < node->getData())return boolRecur(search,node->getLeft());
			// if it is greater than the node and right is not null, go right
		else return boolRecur(search,node->getRight());
			// if none of these work, then it is at the end and is not in the list.
	}
	int max(avlNode<t>*& start){
		if(start->getLeft() != nullptr && start->getRight() != nullptr)
			return (start->getLeft()->getHeight() > start->getRight()->getHeight())? start->getLeft()->getHeight() : start->getRight()->getHeight();
		else if(start->getLeft() == nullptr)
			return (start->getRight()->getHeight());
		else
			return (start->getLeft()->getHeight());
	}
	int difference(avlNode<t>*& start){
		if(start->getLeft() != nullptr && start->getRight() != nullptr)
		return abs(start->getLeft()->getHeight() - start->getRight()->getHeight());
		else{
			if(start->getLeft() != nullptr)
				return abs(start->getLeft()->getHeight());
			else if(start->getRight() != nullptr)
				return abs(start->getRight()->getHeight());
			else throw exception();
		}
	}

	int depth(avlNode<t>*& d){
		if(d == nullptr) return 0;
	if(d->getLeft() == nullptr && d->getRight() == nullptr) return 1;
	else{
		int leftyy = 0;
		int rightyy = 0;
		if(d->getLeft() != nullptr)
		 leftyy = 1 + depth(d->getLeft());
		if(d->getRight() != nullptr)
		 rightyy = 1 + depth(d->getRight());
		return (leftyy > rightyy)? leftyy : rightyy;
	}
	}
	// left left rotation
	void case1Rotation(avlNode<t>*& k2){
	avlNode<t>* k1 = k2->getLeft();
	k2->setLeft(k1->getRight());
	k1->setRight(k2);
	k2->setHeight(depth (k2));
	k1->setHeight(k2->getHeight() + depth (k1->getLeft ()));
	k2 = k1;
	}
// right subtree of left child
	void case2Rotation(avlNode<t>*& k3){
	case4Rotation(k3->getLeft());
	case1Rotation(k3);
	}
// left subtree of right child
	void case3Rotation(avlNode<t>*& k1){
	case1Rotation(k1->getRight());
	case4Rotation(k1);
	}
// right right rotation
	void case4Rotation(avlNode<t>*& k2){
	avlNode<t>* k1 = k2->getRight();
	k2->setRight(k1->getLeft());
	k1->setLeft(k2);
	k2->setHeight(depth (k2));
	k1->setHeight(k2->getHeight() + depth (k1->getRight ()));
	k2 = k1;
	}
	void sizeTrav(avlNode<t>*& k, int& soFar){
		if(k == nullptr) return;
		// do the left
		sizeTrav(k->getLeft(),soFar);
		// do this
		soFar++;
		// then do the right
		sizeTrav(k->getRight(),soFar);
	}
	void orderTraversal(avlNode<t>*& k, list<t>& adder){
		if(k == nullptr) return;
			// do the left
			orderTraversal(k->getLeft(),adder);
			// do this
			adder.push_back(k->getData());
			// then do the right
			orderTraversal(k->getRight(),adder);
	}
	void preTraversal(avlNode<t>*& k, list<t>& adder){
		if(k == nullptr) return;
			// pre order
			adder.push_back(k->getData ());
			// then do the left
			preTraversal(k->getLeft(),adder);
			// then do the right
			preTraversal(k->getRight(),adder);
	}
	void postTraversal(avlNode<t>*& k, list<t>& adder){
		if(k == nullptr) return;
		// do the left first
		postTraversal(k->getLeft(),adder);
		// then do the right
		postTraversal(k->getRight(),adder);
		// then do this
		adder.push_back(k->getData());
	}
};
#endif //UNTITLED1_AVLTREE_H
