//
// Created by gouldilocks on 11/12/20.
//

#ifndef UNTITLED1_AVLNODE_H
#define UNTITLED1_AVLNODE_H
template <class t>
class avlNode {
public:
	/*Constructors*/
	avlNode<t>(){
		this->right = nullptr;
		this->left = nullptr;
		this->data = nullptr;
		this-> height = 0;
	}
	avlNode<t>(const t& element,  avlNode<t>*& parent, avlNode<t>*& left = nullptr, avlNode<t>*& right = nullptr){
		this->data = new t(element);
		this->left = left;
		this->right = right;
		this->parent = parent;
		this-> height = 1;
	}
	avlNode<t>(const t& element){
		this->data = new t(element);
		this-> left = nullptr;
		this-> right = nullptr;
		this-> parent = nullptr;
		this-> height = 1;
	}
	avlNode<t>(const avlNode<t>& copy){
		this->data = new t(*copy.data);
		this->right = copy.right;
		this->left = copy.left;
		this->parent = copy.parent;
		this-> height = copy.height;
	}
	/*Destructor*/
	~avlNode<t>(){
		delete this->data;
	}
	/*Functions*/
	void setHeight(int x){
		this-> height = x;
	}
	int getHeight(){
		return this-> height;
	}
	t& getData(){
		return *(this->data);
	}
	avlNode<t>*& getLeft(){
		return this->left;
	}
	avlNode<t>*& getRight(){
		return this->right;
	}
	avlNode*& getParent(){
		return this->parent;
	}
	void setData(const t& element){
		this-> data = element;
	}
	void setLeft(avlNode<t>*& neww){
		this-> left = neww;
	}
	void setRight(avlNode<t>*& neww){
		this-> right = neww;
	}
	void setParent(avlNode<t>*& neww){
		this->parent = neww;
	}
	/*Operators*/
	avlNode<t>& operator=(const avlNode<t>& rhs){
		if(*this == rhs) return *this;
		else {
			delete this-> data;
			this->height = rhs.height;
			this-> left = rhs.left;
			this->right = rhs.right;
			this-> parent = rhs.parent;
		}
	}
private:
	int height;
	t* data;
	avlNode<t>* left;
	avlNode<t>* right;
	avlNode<t>* parent;
};
#endif //UNTITLED1_AVLNODE_H
