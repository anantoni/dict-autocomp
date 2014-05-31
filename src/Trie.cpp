/******************** Trie functions ***********************/
/***********************************************************/
#include "class_definitions.hpp"

Trie::Trie() {
	root = new InnerNode('/');
}

void Trie::trieUpdateMostFrequent(LeafNode* leafNode) {
	InnerNode* currentNode = leafNode->getParent();

	while (currentNode->getLabel() != '/') {
		currentNode->updateMostFrequent(leafNode);
		currentNode = currentNode->getParent();
	}
}

string Trie::composeWord(LeafNode* leaf) {
	string word = "";
	InnerNode* currentNode = leaf->getParent();
	
	while (currentNode->getLabel() != '/') {
		word += currentNode->getLabel();
		currentNode = currentNode->getParent();
	}
	return string(word.rbegin(), word.rend());
}

void Trie::addWord(string word) {
	InnerNode* currentNode = root;

	for(char& c : word) {
		//cout << c/*-'a'*/ << endl;
		if ( currentNode->getChild(c-'a') == nullptr ) 
			currentNode->addInnerNode(new InnerNode(c, currentNode));
		
		currentNode = currentNode->getChild(c-'a');
	}
	LeafNode* leafNode = new LeafNode(currentNode);
	currentNode->addLeafNode(leafNode);
	trieUpdateMostFrequent(leafNode);
}

void Trie::printTrie(ofstream& ofs) {
	string word = "";
	printWords(root, word, ofs);
}

void Trie::printWords(InnerNode* currentNode, string word, ofstream& ofs) {
	if (currentNode->getLabel() != '/')
		word += currentNode->getLabel();

	if (currentNode->getLeaf() != nullptr)
		ofs << word << endl;

    for (char i=0; i<ALPHABET_SIZE; i++) 
		if (currentNode->getChild(i) != nullptr) 
			printWords(currentNode->getChild(i), word, ofs);	
}

Recommendations Trie::getRecommendations(string word) {
	InnerNode* currentNode = root;
    if (word == "")
      return Recommendations();
    
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	for(char& c : word) {
		if ( currentNode->getChild(c-'a') == nullptr ) 
		    return Recommendations();
		
		currentNode = currentNode->getChild(c-'a');
	}
	return currentNode->getRecommendations(this);
}

void Trie::updateWord(string word) {
	InnerNode* currentNode = root;
	LeafNode* leafNode = nullptr;

	if (word == "")
		return;

	transform(word.begin(), word.end(), word.begin(), ::tolower);
	for(char& c : word) {
		if ( currentNode->getChild(c-'a') == nullptr ) 
			currentNode->addInnerNode(new InnerNode(c, currentNode));
		
		currentNode = currentNode->getChild(c-'a');
	}
	if (currentNode->getLeaf() == nullptr)
		leafNode = new LeafNode(currentNode, 1);
	else {
	    leafNode = currentNode->getLeaf();
		leafNode->incrFreq();
	}
	currentNode->addLeafNode(leafNode);
	trieUpdateMostFrequent(leafNode);
}

void Trie::destroyChildren(InnerNode* node) {
	if (node->getLeaf() != nullptr) {
		delete node->getLeaf();
		return;
	}
		
	for (int i=0; i<ALPHABET_SIZE; i++) 
		if (node->getChild(i) != nullptr) {
			destroyChildren(node->getChild(i));
			delete node->getChild(i);
		}
}

Trie::~Trie() {
	destroyChildren(root);
}


