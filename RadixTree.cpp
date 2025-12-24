// RadixTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstring>
#include "RadixTree.h"


RadixTree::RadixTree() {
	myRoot = nullptr;
}
RadixTree::~RadixTree() {
	//deletion 
}

bool RadixTree::empty() {
	return myRoot == nullptr;

}
/*int RadixTree::SearchPrefix(const char* word, Node* currentNode) {
	int i = 0;
	while (currentNode->data[i] != '\0' && word[i] != '\0' && word[i] == currentNode->data[i])
		i++;
	return i;
}*/
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
				return;
			}
			if (prefix == wordLen && prefix < nodeLen) {
				Node* oldnode = ch->node;//let oldnode pointer point at the node with the whole word
				Node* newnode = new Node(word); //node created for our word 
				newnode->ended = true;
				/*char temp[50]
				strcpy(temp, oldnode->data+pref);
				strcpy(oldnode->data,temp);
				or */
				Node* suffix = new Node(oldnode->data + prefix);
				suffix->ended = oldnode->ended;
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
				newNode->children = nullptr;

				addchild(ch->node, newNode);
				return;
			}
			//else {//case 3:partial overlap condition
				//int prefix = SearchPrefix(word, ch->node->data);
			if (prefix > 0 && prefix < nodeLen) {
				Node* newTop = new Node();
				strncpy_s(newTop->data, ch->node->data, prefix);
				newTop->data[prefix] = '\0';
				newTop->ended = false;
				Node* node1 = new Node();
				Node* node2 = new Node();
				strncpy_s(node1->data, ch->node->data + prefix, nodeLen - prefix);
				node1->data[nodeLen - prefix] = '\0';
				node1->ended = ch->node->ended;
				node1->children = ch->node->children;
				strncpy_s(node2->data, word + prefix, wordLen - prefix);
				node2->data[wordLen - prefix] = '\0';
				node2->ended = true;
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
/*int RadixTree::matchPrefix(const char* nodePrefix, const char* keySegment) {
	int i = 0;
	while (nodePrefix[i] != '\0' && keySegment[i] != '\0' && nodePrefix[i] == keySegment[i])
		i++;
	return i;
}*/

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

	//traverse children
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



Node* RadixTree::findNodeForPrefix(const char* prefix) {
	if (empty() || prefix == nullptr)
		return nullptr;

	Node* current = myRoot;
	int index = 0;//how much of prefix we matched
	int prefixLen = strlen(prefix);//length of prefix we're looking for

	int rootmatch = matchPrefix(current->data, prefix);//see how many characters match betweb roots data and prefix we're searching for
	//1st case root mismatch
	if (rootmatch == 0)
		return nullptr;//prefix does not exist
	// prefix ends inside or exactly at root
	if (rootmatch >= prefixLen)
		return current;// for example prefix is ca, root is cat so cat will be returned
	// prefix continues beyond root label
	if (rootmatch < strlen(current->data))//length of stored data bigger than the matched prefixes
		return nullptr;

	index = rootmatch; //to track how much is already matched of the prefix
	//we go down the tree 
	while (index < prefixLen) {
		child* ch = current->children;
		bool found = false;
		//searching among children
		while (ch != nullptr) {
			if (ch->firstChar == prefix[index]) {//compare first character to see if weshould continue with this child

				int matched = matchPrefix(ch->node->data, prefix + index);//prefix+index points to char starting from after matched point

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
				index += matched;//child node fully matches a part of the prefix, so continue traversal down this child.
				current = ch->node;
				found = true;
				break;
			}
			ch = ch->next;//If first child didn’t match, check next sibling.
		}
		// No matching child found
		if (!found)
			return nullptr;
	}

	// Prefix fully matched at node boundary
	return current;
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




