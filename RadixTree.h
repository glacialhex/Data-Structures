#pragma once
#include "Node.h"

// Search result structure
struct SearchResult {
	bool found;
	Node* node;
	const char* reason; // failure reason if not found
	SearchResult() : found(false), node(nullptr), reason(nullptr) {}
};

class RadixTree
{
public:
	Node* myRoot;
	int childCount;
	RadixTree();
	~RadixTree();
	bool empty();
	void insert(const char* word);
	int SearchPrefix(const char* word, Node* currentNode);

	// Person 3 - Search/Lookup Operations
	SearchResult search(Node* root, const char* key);
	Node* traverseEdge(Node* node, const char* keySegment, int& matchedLength);
	int matchPrefix(const char* nodePrefix, const char* keySegment);
	bool isKeyFullyMatched(Node* node, const char* key);
	SearchResult handleSearchFailure(const char* reason);
};
