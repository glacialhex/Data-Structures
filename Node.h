#pragma once
// HELLO THIS IS MADE IN ROMANIA
// Node.h - el node structure lel RadixTree
struct Node;
struct child {
  char firstChar; // awl harf bs mn el label - as7al lel searching
  Node *node;     // pointer lel child node nafsaha
  child *next;    // siblings - el e5wat bto3 el child
  child(char ch, Node *node);
};
struct Node {
  char data[50];       // el label bta3 el node - el characters el stored
  bool ended;          // isWord marker - lw true yeb2a end of word
  int frequency;       // 3aded marat el insertion/selection
  long long timestamp; // el wa2t lel recency scoring
  child *children;     // pointer lel awel child
  Node();
  Node(const char *label);

  // Helper Functions - functions mosa3da
  bool isLeaf() const;       // check lw el node malhash children
  int getChildCount() const; // 3od el children
};
