// RadixTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RadixTree.h"

	RadixTree::RadixTree(){
		myRoot = nullptr;
		childCount = 0;
	}
	RadixTree::~RadixTree() {
		//deletion 
	}

	bool RadixTree::empty(){
		return childCount == 0;

	}
	int RadixTree::SearchPrefix(const char* word, Node* currentNode) {
		int i = 0;
		while (currentNode->data[i] != '\0' && word[i] != '\0' && word[i] == currentNode->data[i])
			i++;
		return i;
	}
	void RadixTree::insert(const char* word) {
		//case1:empty tree
		if (empty()) {
			myRoot=new Node(word);
			myRoot->ended = true;
			return;
		}
		//case 3:common  prefix found
		child* cur = myRoot->children;
		bool foundPrefix = false;

		while (cur != nullptr) {
			int pref = SearchPrefix(word, cur->node);
			if (pref> 0) {
				foundPrefix = true;
				break;
			}
			cur = cur->next;
		}
	//case 2:no common prefix found
		if (!foundPrefix) {
			Node* oldRoot = myRoot;
			myRoot = new Node();
			child* oldChild = new child(oldRoot->data[0], oldRoot);
			Node* newNode = new Node(word);
			newNode->ended = true;
			child* newChild = new child(word[0], newNode);
			oldChild->next = newChild;
			myRoot->children = oldChild;
		}
	}

// ============================================
// Person 3 - Search/Lookup Operations
// ============================================

/**
 * Main search function - searches for a key in the radix tree
 * @param root: starting node for search
 * @param key: the string to search for
 * @return SearchResult with found status, node pointer, and failure reason
 */
SearchResult RadixTree::search(Node* root, const char* key) {
	// TODO: Implement main search logic
	// 1. Start from root
	// 2. Traverse edges using traverseEdge
	// 3. Check if key is fully matched
	// 4. Return success or failure result
	return SearchResult();
}

/**
 * Traverses an edge by matching the key segment with node's data
 * @param node: current node to traverse from
 * @param keySegment: remaining part of the key to match
 * @param matchedLength: output parameter - how many characters matched
 * @return next node to continue search, or nullptr if no match
 */
Node* RadixTree::traverseEdge(Node* node, const char* keySegment, int& matchedLength) {
	// TODO: Implement edge traversal
	// 1. Find child whose firstChar matches keySegment[0]
	// 2. Use matchPrefix to check how much matches
	// 3. Return appropriate child node or nullptr
	matchedLength = 0;
	return nullptr;
}

/**
 * Matches prefix between node's label and key segment
 * @param nodePrefix: the data stored in the node
 * @param keySegment: the remaining key to match
 * @return number of characters that match
 */
int RadixTree::matchPrefix(const char* nodePrefix, const char* keySegment) {
	// TODO: Implement prefix matching
	// Compare characters until mismatch or end of either string
	int i = 0;
	// while (nodePrefix[i] != '\0' && keySegment[i] != '\0' && nodePrefix[i] == keySegment[i])
	//     i++;
	return i;
}

/**
 * Checks if the key has been fully matched at this node
 * @param node: current node
 * @param key: original search key
 * @return true if key is fully matched AND node marks end of word
 */
bool RadixTree::isKeyFullyMatched(Node* node, const char* key) {
	// TODO: Implement full match check
	// 1. Check if we've consumed entire key
	// 2. Check if node->ended is true (marks end of valid word)
	return false;
}

/**
 * Creates a SearchResult for failed searches
 * @param reason: explanation of why search failed
 * @return SearchResult with found=false and reason set
 */
SearchResult RadixTree::handleSearchFailure(const char* reason) {
	// TODO: Create and return failure result
	SearchResult result;
	result.found = false;
	result.node = nullptr;
	result.reason = reason;
	return result;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
