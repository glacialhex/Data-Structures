// RadixTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
#include <chrono>
#include "RadixTree.h"

using namespace std;

RadixTree::RadixTree() {
	myRoot = nullptr;
}
RadixTree::~RadixTree() {
	//deletion 
}

bool RadixTree::empty() {
	return myRoot == nullptr;

}
int RadixTree::matchPrefix(const char* nodePrefix, const char* keySegment) {
	int i = 0;
	while (nodePrefix[i] != '\0' && keySegment[i] != '\0' && nodePrefix[i] == keySegment[i])
		i++;
	return i;
}
int RadixTree::SearchPrefix(const char* word, Node* currentNode) {
	return matchPrefix(word, currentNode->data);
}
void RadixTree::addchild(Node* parent, Node* childnode) {
	child* newchild = new child(childnode->data[0], childnode);//saved first char, and childnode is the node of child itselt
	newchild->next = parent->children;//siblings of new child is now the chilren of the parent
	parent->children = newchild; //children of parent is now the new child, so when saying children the pointer now points to the new child
}
void RadixTree::insert(const char* word) {
	//case1:empty tree
	if (empty()) {
		myRoot = new Node(word);
		myRoot->ended = true;
		myRoot->frequency = 1;                    // yousef: Initialize frequency
		myRoot->timestamp = getCurrentTimestamp(); // yousef: Set timestamp
		return;
	}
	//case 3:common  prefix found
	//child* cur = myRoot->children;
	bool isPrefix = false;
	//Node* current = myRoot;
	Node* parentNode = myRoot;
	child* ch = parentNode->children;
	while (ch != nullptr) {
		int nodeLen = 0;
		while (ch->node->data[nodeLen] != '\0') nodeLen++;
		int wordLen = 0;
		while (word[wordLen] != '\0') wordLen++;
		int prefix = SearchPrefix(word, ch->node);
		if (prefix > 0) {// case exact match
			isPrefix = true;
			if (prefix == nodeLen && prefix == wordLen) {// this works becaus ethe serachprefix counts only equal prefixes so if they are same number as well then its the same word
				ch->node->ended = true;
				ch->node->frequency++;                    // yousef: Increment frequency
				ch->node->timestamp = getCurrentTimestamp(); // yousef: Update timestamp
				return;
			}
			if (prefix == wordLen && prefix < nodeLen) {
				Node* oldnode = ch->node;//let oldnode pointer point at the node with the whole word
				Node* newnode = new Node(word); //node created for our word 
				newnode->ended = true;
				newnode->frequency = 1;                    // yousef: Initialize frequency
				newnode->timestamp = getCurrentTimestamp(); // yousef: Set timestamp
				/*char temp[50]
				strcpy(temp, oldnode->data+pref);
				strcpy(oldnode->data,temp);
				or */
				Node* suffix = new Node(oldnode->data + prefix);
				suffix->ended = oldnode->ended;
				suffix->frequency = oldnode->frequency;    // yousef: Preserve frequency
				suffix->timestamp = oldnode->timestamp;    // yousef: Preserve timestamp
				suffix->children = oldnode->children;
				newnode->children = nullptr;
				addchild(newnode, suffix);
				ch->node = newnode;
				delete oldnode;
				return;

			}
				//case 2:common  prefix found
		    /*if (wordLen > nodeLen) {
				isPrefix = true;
				for (int i = 0; i < nodeLen; i++) {
					if (ch->node->data[i] != word[i]) {
						isPrefix = false;
						break;
					}
				}
			if (isPrefix) {
					int difference = wordLen - nodeLen;
					Node* newNode = new Node();
					for (int i = 0; i < difference; i++) {
						newNode->data[i] = word[nodeLen + i];
					}
				    newNode->data[difference] = '\0';
					newNode->ended = true;
					newNode->children = nullptr;
					child* newChild = new child(newNode->data[0], newNode);
					newChild->next = ch->node->children;
					ch->node->children = newChild;
					addchild(ch->node, newNode);
					return;
				}
			}*/
			if (prefix == nodeLen && wordLen > nodeLen) {

				Node* newNode = new Node(word + nodeLen);
				newNode->ended = true;
				newNode->frequency = 1;                    // yousef: Initialize frequency
				newNode->timestamp = getCurrentTimestamp(); // yousef: Set timestamp
				newNode->children = nullptr;

				addchild(ch->node, newNode);
				return;
			}
			//else {//case 3:partial overlap condition
				//int prefix = SearchPrefix(word, ch->node->data);
			if (prefix > 0 && prefix < nodeLen) {
				Node* newTop = new Node();
				strncpy(newTop->data, ch->node->data, prefix);
				newTop->data[prefix] = '\0';
				newTop->ended = false;
				Node* node1 = new Node();
				Node* node2 = new Node();
				strncpy(node1->data, ch->node->data + prefix, nodeLen - prefix);
				node1->data[nodeLen - prefix] = '\0';
				node1->ended = ch->node->ended;
				node1->frequency = ch->node->frequency;    // yousef: Preserve frequency
				node1->timestamp = ch->node->timestamp;    // yousef: Preserve timestamp
				node1->children = ch->node->children;
				strncpy(node2->data, word + prefix, wordLen - prefix);
				node2->data[wordLen - prefix] = '\0';
				node2->ended = true;
				node2->frequency = 1;                      // yousef: Initialize frequency
				node2->timestamp = getCurrentTimestamp();  // yousef: Set timestamp
				node2->children = nullptr;
				child* child1 = new child(node1->data[0], node1);
				child* child2 = new child(node2->data[0], node2);
				child1->next = child2;
				newTop->children = child1;
				//child* newChild= new child(newTop->data[0],newTop);
				//newChild->next = ch->next;
				//parentNode->children = newChild;
				addchild(parentNode, newTop);
				return;
			}
		}
		parentNode = ch->node;
		ch = ch->next;

	}

	//case 2:no common prefix found
		Node* newnode = new Node(word);// created a newnode pointer of type node, then made it point to a node that has the word
		newnode->ended = true;
		newnode->frequency = 1;                    // yousef: Initialize frequency
		newnode->timestamp = getCurrentTimestamp(); // yousef: Set timestamp
		newnode->children = nullptr;
		addchild(parentNode, newnode);
	}

	bool RadixTree::deleteRec(Node*& current, const char* word) {
		if (!current) return false;

		int prefix = SearchPrefix(word, current);
		int len = strlen(current->data);
		int wlen = strlen(word);

		// If the current node does NOT match the expected prefix → not found
		if (prefix == 0) return false;

		// ---------------------------------------------------------
		// CASE 1: The word ends exactly here
		// ---------------------------------------------------------
		if (prefix == wlen && prefix == len) {
			if (!current->ended) return false;  // word not stored

			current->ended = false;             // unmark the terminal flag

			// If no children → delete the node completely
			if (current->children == nullptr) {
				delete current;
				current = nullptr;
				return true;
			}

			// If exactly ONE child → merge it upward
			if (current->children && current->children->next == nullptr) {
				Node* childNode = current->children->node;

				// Merge labels
				strcat(current->data, childNode->data);
				current->ended = childNode->ended;
				current->children = childNode->children;

				delete childNode;
			}

			return true;
		}

		// ---------------------------------------------------------
		// CASE 2: The word continues into a child
		// ---------------------------------------------------------
		if (prefix == len && prefix < wlen) {

			char nextChar = word[prefix];
			child* prev = nullptr;
			child* ch = current->children;

			// Find the matching child in the linked list
			while (ch && ch->firstChar != nextChar) {
				prev = ch;
				ch = ch->next;
			}

			if (!ch) return false;  // child not found

			bool removed = deleteRec(ch->node, word + prefix);

			if (!removed) return false;

			// Child was deleted
			if (ch->node == nullptr) {
				// Remove child link
				if (prev == nullptr)
					current->children = ch->next;
				else
					prev->next = ch->next;

				delete ch;
				ch = nullptr;
			}

			// If current → only 1 child left and not terminal → merge
			if (!current->ended && current->children && current->children->next == nullptr) {
				Node* childNode = current->children->node;

				strcat(current->data, childNode->data);
				current->ended = childNode->ended;
				current->children = childNode->children;

				delete childNode;
			}

			return true;
		}

		return false;  // partial match that doesn't align with delete rules
	}
	bool RadixTree::deleteWord(const char* word) {
		if (!myRoot) return false;   // tree is empty
		return deleteRec(myRoot, word);  // call the recursive deletion
	}

	// ---------------------------------------------------------
	// Search / Lookup Operations
	// ---------------------------------------------------------

	//compares node prefix with key segment, returns length of match

	//traverses an edge and returns the child node if prefix matches fully
	Node* RadixTree::traverseEdge(Node* node, const char* keySegment, int& matchedLen) {
		child* ch = node->children;
		while (ch != nullptr) {
			if (ch->firstChar == keySegment[0]) {
				matchedLen = matchPrefix(ch->node->data, keySegment);
				int nodeLen = strlen(ch->node->data);

				if (matchedLen == nodeLen) {// full edge match → can continue traversal
					return ch->node;
				}
				return nullptr;// partial match → key doesn't exist in tree
			}
			ch = ch->next;
		}
		matchedLen = 0;
		return nullptr;
	}

	//checks if the key is fully matched at this node
	bool RadixTree::isKeyFullyMatched(Node* node, const char* key, int keyLen) {
		int nodeLen = strlen(node->data);
		return (keyLen == nodeLen) && node->ended;
	}

	//handles search failure (can be extended for logging/debugging)
	void RadixTree::handleSearchFailure(const char* reason) {
		std::cout << "Search failed: " << reason << std::endl; //added otpion to print or log the failure reason
	}

	//main search function → returns true if word exists in tree
	bool RadixTree::search(const char* key) {
		if (empty()) {
			handleSearchFailure("Tree is empty");
			return false;
		}

		int keyLen = strlen(key);
		int keyIndex = 0;

		//check root first
		int rootMatch = matchPrefix(myRoot->data, key);
		int rootLen = strlen(myRoot->data);

		if (rootMatch < rootLen) {
			handleSearchFailure("Root prefix mismatch");
			return false;
		}

		keyIndex += rootMatch;

		//if key fully matched at root
		if (keyIndex == keyLen) {
			if (myRoot->ended) {
				return true;
			}
			handleSearchFailure("Key is prefix of existing word but not marked as ended");
			return false;
		}

		//traverse children starting from root
		Node* current = myRoot;
		while (keyIndex < keyLen) {
			int matchedLen = 0;
			Node* nextNode = traverseEdge(current, key + keyIndex, matchedLen);

			if (nextNode == nullptr) {
				handleSearchFailure("No matching child found");
				return false;
			}

			keyIndex += matchedLen;
			current = nextNode;
		}

		//check if we've matched the entire key and it's marked as ended
		if (current->ended) {
			return true;
		}

		handleSearchFailure("Key found but not marked as complete word");
		return false;
	}
//NOUR
	void RadixTree :: collectWords(Node* node, string currentWord) {
		if (!node) return;
		currentWord += node->data;
		if (node->ended) {
			cout << currentWord <<endl;
		}
		child* ch = node->children;
		while (ch) {
			collectWords(ch->node, currentWord);
			ch = ch->next;
		}
	}
	void RadixTree::autoSuggest(const char* prefix) {
		if (empty()) {
			cout << "Tree is empty. No suggestions available." << std::endl;
			return;
		}
		Node* current = myRoot;
		int indexrn = 0;
		int prefixLength = strlen(prefix);
		int rootMatch = matchPrefix(current->data, prefix);
        int rootLen = strlen(current->data);
		if (rootMatch < rootLen && rootMatch < prefixLength) {
        cout << "No suggestions found for the given prefix." << endl;
        return;
    }
    indexrn += rootMatch;
		while (indexrn < prefixLength) {
			int matchedLen = 0;
			Node* nextNode = traverseEdge(current, prefix + indexrn, matchedLen);
			if (nextNode == nullptr) {
				cout << "No suggestions found for the given prefix." <<endl;
				return;
			}
			indexrn += matchedLen;
			current = nextNode;
		}
		string currentWord = "";
		for (int i = 0; i < indexrn; i++) {
			currentWord += prefix[i];
		}
		collectWords(current, currentWord);
	}

// =============================================================
// yousef: Helper Functions for Tree Enhancements + Cleanup
// =============================================================

// Get current timestamp for recency tracking
long long RadixTree::getCurrentTimestamp() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
}

// Merge single-child non-word nodes to compress the tree
// This merges a child into its parent if the child has only one child and is not a word endpoint
void RadixTree::mergeNodes(Node* parent, char edgeChar) {
	if (!parent) return;

	// Find the child with the given edge character
	child* ch = parent->children;
	while (ch != nullptr && ch->firstChar != edgeChar) {
		ch = ch->next;
	}

	if (!ch || !ch->node) return;

	Node* childNode = ch->node;

	// Only merge if child has exactly one child and is not a word endpoint
	if (childNode->getChildCount() == 1 && !childNode->ended) {
		child* grandchild = childNode->children;

		if (grandchild && grandchild->node) {
			// Combine labels: child's label + grandchild's label
			char combinedLabel[100];
			strcpy(combinedLabel, childNode->data);
			strcat(combinedLabel, grandchild->node->data);

			// Update grandchild's label to combined
			strcpy(grandchild->node->data, combinedLabel);

			// Move grandchild up to replace child
			ch->node = grandchild->node;
			ch->firstChar = combinedLabel[0];

			// Clean up old child node
			delete childNode;
		}
	}
}

// Update frequency and timestamp when a word is accessed/chosen
void RadixTree::updateWordFrequency(Node* node) {
	if (node && node->ended) {
		node->frequency++;
		node->timestamp = getCurrentTimestamp();
	}
}

// Increment frequency for an existing word in the tree
// Useful when user selects a suggestion (for ranking)
void RadixTree::incrementFrequency(const char* word) {
	if (empty() || !word) return;

	int keyLen = strlen(word);
	int keyIndex = 0;

	// Check root first
	int rootLen = strlen(myRoot->data);

	if (rootLen > 0) {
		int rootMatch = matchPrefix(myRoot->data, word);
		if (rootMatch < rootLen) return; // Word not found

		keyIndex += rootMatch;

		if (keyIndex == keyLen) {
			// Word matches at root
			updateWordFrequency(myRoot);
			return;
		}
	}

	// Traverse to find the word
	Node* current = myRoot;
	while (keyIndex < keyLen) {
		int matchedLen = 0;
		Node* nextNode = traverseEdge(current, word + keyIndex, matchedLen);

		if (nextNode == nullptr) return; // Word not found

		keyIndex += matchedLen;
		current = nextNode;
	}

	// Update frequency if we found the complete word
	updateWordFrequency(current);
}

// =============================================================
// malak: Prefix Search Engine (Core Prefix Navigation)
// =============================================================

// Find the node corresponding to a given prefix
// Returns pointer to the node where prefix ends, or nullptr if prefix not found
Node* RadixTree::findNodeForPrefix(const char* prefix) {
	if (empty() || prefix == nullptr)
		return nullptr;

	Node* current = myRoot;
	int index = 0;  // how much of prefix we matched
	int prefixLen = strlen(prefix);  // length of prefix we're looking for

	// See how many characters match between root's data and prefix we're searching for
	int rootmatch = matchPrefix(current->data, prefix);

	// 1st case: root mismatch
	if (rootmatch == 0)
		return nullptr;  // prefix does not exist

	// Prefix ends inside or exactly at root
	if (rootmatch >= prefixLen)
		return current;  // e.g., prefix is "ca", root is "cat" so cat will be returned

	// Prefix continues beyond root label
	if (rootmatch < strlen(current->data))  // length of stored data bigger than the matched prefixes
		return nullptr;

	index = rootmatch;  // to track how much is already matched of the prefix

	// We go down the tree
	while (index < prefixLen) {
		child* ch = current->children;
		bool found = false;

		// Searching among children
		while (ch != nullptr) {
			if (ch->firstChar == prefix[index]) {  // compare first character to see if we should continue with this child
				int matched = matchPrefix(ch->node->data, prefix + index);  // prefix+index points to char starting from after matched point

				// No match at all
				if (matched == 0)
					return nullptr;

				// Prefix ends inside this node
				if (index + matched >= prefixLen)
					return ch->node;

				// Partial mismatch before prefix ends
				if (matched < strlen(ch->node->data))
					return nullptr;

				// Full match, continue traversal
				index += matched;  // child node fully matches a part of the prefix, so continue traversal down this child
				current = ch->node;
				found = true;
				break;
			}
			ch = ch->next;  // If first child didn't match, check next sibling
		}

		// No matching child found
		if (!found)
			return nullptr;
	}

	// Prefix fully matched at node boundary
	return current;
}

#include <vector> 

// This is the recursive function that hunts for words
void RadixTree::collectAllWords(Node* node, string currentString, vector<string>& results) {
    // 1-If the node is empty stop
    if (node == nullptr) {
        return;
    }

    // 2-Check if the current path forms a valid word
    // If 'ended' is true, it means a word stops here. Add it to our list.
    if (node->ended) {
        results.push_back(currentString);
    }

    // 3-Explore the children (The "DFS" part)
    // Your children are stored in a Linked List, so we use a while loop.
    child* track = node->children;

    while (track != nullptr) {
        // RADIX LOGIC:
        // Unlike a normal Trie, we don't just add one letter.
        // We add the WHOLE string stored in the child node (track->node->data).
        string nextString = currentString + string(track->node->data);

        // Recursive call: Go deeper into this child
        collectAllWords(track->node, nextString, results);

        // Move to the next sibling in the linked list
        track = track->next;
    }
}

// This is the function the Main or GUI will actually call
vector<string> RadixTree::getAutocompletions(const char* prefix) {
    vector<string> results;

    Node* startNode = findNodeForPrefix(prefix);

    // If the prefix doesn't exist in the tree, return an empty list
    if (startNode == nullptr) {
        return results;
    }

    // Collect all words starting from that node
    // We pass the 'prefix' as the starting string
    collectAllWords(startNode, string(prefix), results);

    return results;
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


