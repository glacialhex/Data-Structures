#include <iostream>
#include "Node.h"
Node::Node() {
	data[0] = 0;
	ended = false;
	children = nullptr;
}
Node::Node(const char* label) {
	int i;
	for (i = 0;label[i] != '\0';i++) {
		data[i] = label[i];
	}
	data[i] = '\0';
	ended = false;
	children = nullptr;
}
child::child(char ch, Node* n) {
	firstChar = ch;
	node = n;
	next = nullptr;

}