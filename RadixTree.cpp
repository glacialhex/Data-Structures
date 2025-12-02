// RadixTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "RadixTree.h"

	RadixTree::RadixTree(){
		myRoot = nullptr;
	}
	RadixTree::~RadixTree() {
		//deletion 
	}

	bool RadixTree::empty(){
		return childCount == 0;

	}
	int RadixTree::SearchPrefix(const char& word, Node* currectNode) {
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
		child* cur = root->children;
		bool foundPrefix = false;

		while (cur != nullptr) {
			int pref = SearchPrefix(cur->node->data, word);
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






// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
