#pragma once
#include "Node.h"
#include <string>
#include <vector>
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
std::vector<std::string> getAutocompletions(const char* prefix);
void collectAllWords(Node* node, std::string currentString, std::vector<std::string>& results);

	// Search / Lookup Operation

	bool search(const char* key);
	Node* traverseEdge(Node* node, const char* keySegment, int& matchedLen);
	int matchPrefix(const char* nodePrefix, const char* keySegment);
	bool isKeyFullyMatched(Node* node, const char* key, int keyLen);
	void handleSearchFailure(const char* reason);
	void collectWords(Node* node, std::string currentWord);
	void autoSuggest(const char* prefix, Node* current);
	// Yousef: Helper functions for tree enhancements
	long long getCurrentTimestamp();                    // Get current timestamp for recency tracking
	void mergeNodes(Node* parent, char edgeChar);       // Merge single-child non-word nodes
	void updateWordFrequency(Node* node);               // Update frequency when word is inserted/chosen
	void incrementFrequency(const char* word);          // Increment frequency for existing word

	// malak: Prefix Search Engine (Core Prefix Navigation)
	Node* findNodeForPrefix(const char* prefix);        // Find node where prefix ends (returns nullptr if not found)
};



