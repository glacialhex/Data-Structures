#pragma once

struct Node;

struct child {
    char firstChar;       // first character of node label
    Node* node;           // pointer to child node
    child* next;          // sibling pointer

    child(char ch, Node* n);
};

struct Node {
    char data[50];        // compressed label
    bool ended;           // marks end of valid word
    int frequency;        // usage frequency
    long long timestamp;  // last access time
    child* children;

    Node();
    Node(const char* label);

    bool isLeaf() const;      // no children
    int getChildCount() const; // count children
};

