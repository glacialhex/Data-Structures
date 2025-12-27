#include "Node.h"
#include <iostream>


// HELLO THIS IS MADE IN ROMANIA
// Node.cpp - implementation bta3 el Node w child structures

// Default constructor - b ye3mel node fadya
Node::Node() {
  data[0] = 0;        // empty string
  ended = false;      // mesh end of word
  frequency = 0;      // mafe4 insertions
  timestamp = 0;      // mafe4 timestamp
  children = nullptr; // mafe4 children
}
// Constructor b label - b ye-copy el label lel data
Node::Node(const char *label) {
  int i;
  // manual string copy - ne-copy character b character
  for (i = 0; label[i] != '\0'; i++) {
    data[i] = label[i];
  }
  data[i] = '\0'; // null terminator
  ended = false;
  frequency = 0;
  timestamp = 0;
  children = nullptr;
}
// isLeaf - check lw el node leaf (malhash children)
bool Node::isLeaf() const { return children == nullptr; }

// getChildCount - 3od kam child 3and el node
int Node::getChildCount() const {
  int count = 0;
  child *ch = children;
  while (ch != nullptr) {
    count++;
    ch = ch->next;
  }
  return count;
}
// child constructor - b ye-initialize el child structure
child::child(char ch, Node *n) {
  firstChar = ch; // awl character lel edge
  node = n;       // el node el linked beha
  next = nullptr; // mafe4 sibling ba3daha
}