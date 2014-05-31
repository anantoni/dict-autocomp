#ifndef __CLASS_DEFINITIONS_HPP_INCLUDED__ 
#define __CLASS_DEFINITIONS_HPP_INCLUDED__  

#define N 5
#define ALPHABET_SIZE 26
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;

class InnerNode;
class Trie;

class Recommendations {
private:
	bool isEmpty;
	string recommendations[N];
	string sortedRecommendations[N];
	int frequencies[N];
	int totalRecommendations;
public:
	Recommendations();
	void setRecommendation(int index, string word);
	void setFrequency(int index, int frequency);
	int printRecommendations();
	void incr();
	int getNumberOfRecommendations();
	string getRecommendation(int index);
};

class LeafNode {
private:
	int frequency; 
	InnerNode* parent;
public:
	LeafNode(InnerNode* parent, int frequency=0);
	InnerNode* getParent();
	int getFrequency();
	void incrFreq();
};

class InnerNode {
private:
	char label;
	InnerNode* children[ALPHABET_SIZE];  //array of ALPHABET_SIZE inner node ptrs
	InnerNode* parent;                   //pointer to parent
	LeafNode* mostFrequent[N];           //array of ptrs to N most frequent leaf nodes
	LeafNode* leaf;	                     //pointer to leaf
public:
	InnerNode(char label, InnerNode* parent=nullptr);
	void addInnerNode(InnerNode* innerNode);
	void addLeafNode(LeafNode* leafNode);	
	InnerNode* getChild(char index);
	InnerNode* getParent();
	char getLabel();
	LeafNode* getLeaf();
	void updateMostFrequent(LeafNode *leafNode);
	Recommendations getRecommendations(Trie* trie);
	
};

class Trie {
private:
	InnerNode* root;
public:
	Trie();
	void addWord(string word);
	void trieUpdateMostFrequent(LeafNode* leafNode);
	void printWords(InnerNode* innerNode, string word, ofstream& ofs);
	void printTrie(ofstream& ofs);
	Recommendations getRecommendations(string word);
	string composeWord(LeafNode* leaf);
	void updateWord(string word);
	void destroyChildren(InnerNode* node);
	~Trie();
};

#endif

