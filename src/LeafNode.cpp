/***************** LeafNode functions ******************/
/*******************************************************/
#include "class_definitions.hpp"
class InnerNode;

LeafNode::LeafNode(InnerNode* parent, int frequency) {
	this->frequency = frequency;
	this->parent = parent;
}

InnerNode* LeafNode::getParent() {
	return parent;
}

int LeafNode::getFrequency() {
	return frequency;
}

void LeafNode::incrFreq() {
	frequency++;
}
