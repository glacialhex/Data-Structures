#pragma once
struct Node;
struct child {
	char firstChar; //by store awl harf bs mn el label 3shan yb2a as7al while searching
	Node* node;     //points to the child itself
		child* next;        //siblings
	child(char ch, Node* node);
};
struct Node {
	char data[50];      // store el label fl node
	bool ended;         // isWord marker - true if this node marks end of a complete word
	int frequency;      // frequency counter - incremented each time word is inserted/selected
	long long timestamp; // timestamp 3shan recency scoring (optional for ranking)
	child* children;
	Node();
	Node(const char* label);

	// Yousef Helmy: Helper Functions
	bool isLeaf() const;           // Check if node has no children
	int getChildCount() const;     // Count number of children
};

