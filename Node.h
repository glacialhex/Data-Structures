#pragma once
// Node.h - el node structure lel RadixTree
struct Node;
struct child {
  char firstChar; // awl harf bs mn el label - as7al lel searching
  Node *node;     // pointer lel child node nafsaha
  child *next;    // siblings - el e5wat bto3 el child
  child(char ch, Node *node);
};
struct Node {
  char *data;          // Dynamic pointer for label (saves memory)
  bool ended;          // isWord marker
  int frequency;       // 3aded marat el insertion
  long long timestamp; // timestamp
  child *children;     // pointer lel children
  Node();
  Node(const char *label);
  ~Node(); // Destructor to free data

  // Helper Functions - functions mosa3da
  bool isLeaf() const;       // check lw el node malhash children
  int getChildCount() const; // 3od el children
};
