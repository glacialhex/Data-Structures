#pragma once
struct Node;
struct child {
	char firstChar; //by store awl harf bs mn el label 3shan yb2a as7al while searching
	Node* node;     //points to the child itself
	child* next;        //siblings
	child(char ch, Node* node);
};
struct Node {
	char data[50]; //dah 3shan a store el kalma elly fe elnode
	bool ended;
    int frequency;
	child* children;
	Node();
	Node(const char* label);
};

