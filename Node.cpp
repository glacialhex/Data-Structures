#include "Node.h"
#include <cstring> // Needed for strlen, strcpy

// Node.cpp - implementation bta3 el Node w child structures

// Default constructor - b ye3mel node fadya
Node::Node() {
  data = new char[1]; // Allocate 1 byte for null terminator
  data[0] = '\0';     // empty string
  ended = false;      // mesh end of word
  frequency = 0;      // mafe4 insertions
  timestamp = 0;      // mafe4 timestamp
  children = nullptr; // mafe4 children
}

// Constructor b label - b ye-copy el label lel data
Node::Node(const char *label) {
  // Optimization: Dynamic memory allocation logic
  if (label) {
    data = new char[strlen(label) + 1];
    strcpy(data, label);
  } else {
    data = new char[1];
    data[0] = '\0';
  }

  ended = false;
  frequency = 0;
  timestamp = 0;
  children = nullptr;
}

// Destructor - clean up dynamic string
Node::~Node() {
  if (data)
    delete[] data;
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