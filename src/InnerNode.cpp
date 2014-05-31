/********************* InnerNode Function *************************/
/*****************************************************************/
#include "class_definitions.hpp"

InnerNode::InnerNode(char label, InnerNode* parent) {
	this->parent = parent;
	this->label = label;

	for (int i = 0; i<ALPHABET_SIZE; i++) 
		children[i] = nullptr;
	leaf = nullptr;
	for (int i = 0; i<5; i++)
		mostFrequent[i] = nullptr;
}

InnerNode* InnerNode::getChild(char index) {
	return children[(int)index];
}

void InnerNode::addInnerNode(InnerNode* innerNode) {
	children[innerNode->getLabel()-'a'] = innerNode;
}

void InnerNode::addLeafNode(LeafNode* leafNode) {
	leaf = leafNode;
}

char InnerNode::getLabel() {
	return label;
}

LeafNode* InnerNode::getLeaf() {
	return leaf;
}

InnerNode* InnerNode::getParent() {
	return parent;
}

void InnerNode::updateMostFrequent(LeafNode* leafNode) {
	for (int i=0; i<N; i++)
		if (mostFrequent[i] == leafNode)
			return;

	for (int i=0; i<N; i++) 
		if (mostFrequent[i] == nullptr) {
			mostFrequent[i] = leafNode;
			return;
		}

    for (int i=0; i<N; i++)
		if (mostFrequent[i]->getFrequency() < leafNode->getFrequency()) {
			mostFrequent[i] = leafNode;
			return;
		}	
}

Recommendations InnerNode::getRecommendations(Trie* trie) {
	Recommendations recommendations;
	for (int i=0; i<N; i++) 
	{
		if (mostFrequent[i] != nullptr) {
			recommendations.setRecommendation(i, trie->composeWord(mostFrequent[i]));
			recommendations.setFrequency(i, mostFrequent[i]->getFrequency());
			recommendations.incr();
		}
		else {
			recommendations.setRecommendation(i, "");
			recommendations.setFrequency(i, -1);
		}
	}
	//recommendations.printRecommendations();
	return recommendations;
}
