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
	void insert(RadixTree* T, const char& word);
	void SearchPrefix(const char& word, Node* currectNode);
};
