#include <iostream>
#include "Node.h"
Node::Node() {
	data[0] = 0;
	ended = false;
	frequency = 0;
	timestamp = 0;
	children = nullptr;
}
Node::Node(const char* label) {
	int i;
	for (i = 0; label[i] != '\0'; i++) {
		data[i] = label[i];
	}
	data[i] = '\0';
	ended = false;
	frequency = 0;
	timestamp = 0;
	children = nullptr;
}
// Yousef: Check if node is a leaf (has no children)
bool Node::isLeaf() const {
	return children == nullptr;
}

// Yousef: Count the number of children
int Node::getChildCount() const {
	int count = 0;
	child* ch = children;
	while (ch != nullptr) {
		count++;
		ch = ch->next;
	}
	return count;
}
child::child(char ch, Node* n) {
	firstChar = ch;
	node = n;
	next = nullptr;

}