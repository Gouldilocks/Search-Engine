//
// Created by Kellen Long on 11/15/2020.
//

#ifndef SEARCH_ENGINE_BEANS_DSPAIR_H
#define SEARCH_ENGINE_BEANS_DSPAIR_H

#include <iostream>
#include <sstream>
#include <fstream>

/*a pair of any objects that contains a public first and second object
 * and mainly runs operators on the first object*/

template<typename T,typename K>
class DSPair{
public:
	T first;
	K second;
   DSPair<T,K>(T first, K second){
   	this->first = first;
   	this-> second = second;
   }
   DSPair<T,K>() = default;
    bool operator<(const DSPair<T,K>& that)const{
        return first<that.first;
    }
    bool operator>(const DSPair<T,K>& that)const{
        return first>that.first;
    }
    DSPair<T,K>& operator=(const DSPair<T,K>& that){
        first = that.first;
        second = that.second;
        return *this;
    }
    bool operator==(const DSPair<T,K>& that)const{
        return first == that.first ;
    }
    friend ostream& operator<<(ostream& os, const DSPair<T,K>& s){
        os<<s.first;
        os<<s.second;
        return os;
    }
    ostream& operator<<(ostream& os) const{
        os<<this->first;
        os<<this->second;
        return os;
    }
};

#endif //SEARCH_ENGINE_BEANS_DSPAIR_H
