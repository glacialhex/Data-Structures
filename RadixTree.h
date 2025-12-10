#pragma once
#include "Node.h"
class RadixTree
{
public:
	Node* myRoot;
	int childCount;
	RadixTree();
	~RadixTree();
	bool empty();
	void insert(const char* word);
int SearchPrefix(const char* word, Node* currectNode);
void addchild(Node* parent, Node* childnode);
bool deleteWord(const char* word) ;
bool deleteRec(Node*& current, const char* word);

// Search / Lookup Operation

bool search(const char* key);
Node* traverseEdge(Node* node, const char* keySegment, int& matchedLen);
int matchPrefix(const char* nodePrefix, const char* keySegment);
bool isKeyFullyMatched(Node* node, const char* key, int keyLen);
void handleSearchFailure(const char* reason);
};


