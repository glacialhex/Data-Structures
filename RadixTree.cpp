// RadixTree.cpp : Radix Tree Implementation
// Team: Yousef, Nour, Malak, Jana, Lujain
//
#define _CRT_SECURE_NO_WARNINGS
#include "RadixTree.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>


using namespace std;

// ================ CORE FUNCTIONS ================

RadixTree::RadixTree() { // Nour
  myRoot = nullptr;
}

// HELLO THIS IS MADE IN ROMANIA
// Added: Proper memory cleanup in destructor (from lujain branch)
RadixTree::~RadixTree() { // Lujain
  destroy(myRoot);
}

// HELLO THIS IS MADE IN ROMANIA
// Added: Recursive memory cleanup function (from lujain branch - prevents
// memory leaks)
void RadixTree::destroy(Node *node) { // Lujain
  if (!node)
    return;
  child *ch = node->children;
  while (ch) {
    destroy(ch->node);
    child *old = ch;
    ch = ch->next;
    delete old;
  }
  delete node;
}

bool RadixTree::empty() { // Nour
  return myRoot == nullptr;
}

// ================ HELPER FUNCTIONS ================

int RadixTree::matchPrefix(const char *nodePrefix,
                           const char *keySegment) { // Yousef
  int i = 0;
  while (nodePrefix[i] != '\0' && keySegment[i] != '\0' &&
         nodePrefix[i] == keySegment[i])
    i++;
  return i;
}

int RadixTree::SearchPrefix(const char *word, Node *currentNode) { // Nour
  return matchPrefix(word, currentNode->data);
}

void RadixTree::addchild(Node *parent, Node *childnode) { // Malak
  child *newchild = new child(
      childnode->data[0],
      childnode); // saved first char, and childnode is the node of child itself
  newchild->next =
      parent
          ->children; // siblings of new child is now the children of the parent
  parent->children = newchild; // children of parent is now the new child
}

// ================ INSERT ================

void RadixTree::insert(const char *word) { // Nour + Malak
  // case1: empty tree
  if (empty()) {
    myRoot = new Node(word);
    myRoot->ended = true;
    myRoot->frequency = 1;                     // Yousef: Initialize frequency
    myRoot->timestamp = getCurrentTimestamp(); // Yousef: Set timestamp
    return;
  }

  // case 3: common prefix found
  bool isPrefix = false;
  Node *parentNode = myRoot;
  child *ch = parentNode->children;

  while (ch != nullptr) {
    int nodeLen = 0;
    while (ch->node->data[nodeLen] != '\0')
      nodeLen++;
    int wordLen = 0;
    while (word[wordLen] != '\0')
      wordLen++;
    int prefix = SearchPrefix(word, ch->node);

    if (prefix > 0) { // case exact match
      isPrefix = true;

      // Same word already exists
      if (prefix == nodeLen && prefix == wordLen) {
        ch->node->ended = true;
        ch->node->frequency++; // Yousef: Increment frequency
        ch->node->timestamp = getCurrentTimestamp(); // Yousef: Update timestamp
        return;
      }

      // New word is prefix of existing node
      if (prefix == wordLen && prefix < nodeLen) {
        Node *oldnode = ch->node;
        Node *newnode = new Node(word);
        newnode->ended = true;
        newnode->frequency = 1; // Yousef: Initialize frequency
        newnode->timestamp = getCurrentTimestamp(); // Yousef: Set timestamp

        Node *suffix = new Node(oldnode->data + prefix);
        suffix->ended = oldnode->ended;
        suffix->frequency = oldnode->frequency; // Yousef: Preserve frequency
        suffix->timestamp = oldnode->timestamp; // Yousef: Preserve timestamp
        suffix->children = oldnode->children;
        newnode->children = nullptr;
        addchild(newnode, suffix);
        ch->node = newnode;
        delete oldnode;
        return;
      }

      // New word extends beyond existing node
      if (prefix == nodeLen && wordLen > nodeLen) {
        Node *newNode = new Node(word + nodeLen);
        newNode->ended = true;
        newNode->frequency = 1; // Yousef: Initialize frequency
        newNode->timestamp = getCurrentTimestamp(); // Yousef: Set timestamp
        newNode->children = nullptr;
        addchild(ch->node, newNode);
        return;
      }

      // Partial overlap - need to split
      if (prefix > 0 && prefix < nodeLen) {
        Node *newTop = new Node();
        strncpy(newTop->data, ch->node->data, prefix);
        newTop->data[prefix] = '\0';
        newTop->ended = false;

        Node *node1 = new Node();
        Node *node2 = new Node();

        strncpy(node1->data, ch->node->data + prefix, nodeLen - prefix);
        node1->data[nodeLen - prefix] = '\0';
        node1->ended = ch->node->ended;
        node1->frequency = ch->node->frequency; // Yousef: Preserve frequency
        node1->timestamp = ch->node->timestamp; // Yousef: Preserve timestamp
        node1->children = ch->node->children;

        strncpy(node2->data, word + prefix, wordLen - prefix);
        node2->data[wordLen - prefix] = '\0';
        node2->ended = true;
        node2->frequency = 1; // Yousef: Initialize frequency
        node2->timestamp = getCurrentTimestamp(); // Yousef: Set timestamp
        node2->children = nullptr;

        child *child1 = new child(node1->data[0], node1);
        child *child2 = new child(node2->data[0], node2);
        child1->next = child2;
        newTop->children = child1;

        addchild(parentNode, newTop);
        return;
      }
    }
    parentNode = ch->node;
    ch = ch->next;
  }

  // case 2: no common prefix found
  Node *newnode = new Node(word);
  newnode->ended = true;
  newnode->frequency = 1;                     // Yousef: Initialize frequency
  newnode->timestamp = getCurrentTimestamp(); // Yousef: Set timestamp
  newnode->children = nullptr;
  addchild(parentNode, newnode);
}

// ================ DELETE ================

bool RadixTree::deleteRec(Node *&current, const char *word) { // Lujain + Jana
  if (!current)
    return false;

  int prefix = SearchPrefix(word, current);
  int len = strlen(current->data);
  int wlen = strlen(word);

  // If the current node does NOT match the expected prefix → not found
  if (prefix == 0)
    return false;

  // CASE 1: The word ends exactly here
  if (prefix == wlen && prefix == len) {
    if (!current->ended)
      return false; // word not stored

    current->ended = false; // unmark the terminal flag

    // If no children → delete the node completely
    if (current->children == nullptr) {
      delete current;
      current = nullptr;
      return true;
    }

    // If exactly ONE child → merge it upward
    if (current->children && current->children->next == nullptr) {
      Node *childNode = current->children->node;
      strcat(current->data, childNode->data);
      current->ended = childNode->ended;
      current->children = childNode->children;
      delete childNode;
    }

    return true;
  }

  // CASE 2: The word continues into a child
  if (prefix == len && prefix < wlen) {
    char nextChar = word[prefix];
    child *prev = nullptr;
    child *ch = current->children;

    // Find the matching child in the linked list
    while (ch && ch->firstChar != nextChar) {
      prev = ch;
      ch = ch->next;
    }

    if (!ch)
      return false; // child not found

    bool removed = deleteRec(ch->node, word + prefix);

    if (!removed)
      return false;

    // Child was deleted
    if (ch->node == nullptr) {
      if (prev == nullptr)
        current->children = ch->next;
      else
        prev->next = ch->next;
      delete ch;
      ch = nullptr;
    }

    // If current → only 1 child left and not terminal → merge
    if (!current->ended && current->children &&
        current->children->next == nullptr) {
      Node *childNode = current->children->node;
      strcat(current->data, childNode->data);
      current->ended = childNode->ended;
      current->children = childNode->children;
      delete childNode;
    }

    return true;
  }

  return false; // partial match that doesn't align with delete rules
}

bool RadixTree::deleteWord(const char *word) { // Lujain + Jana
  if (!myRoot)
    return false; // tree is empty
  return deleteRec(myRoot, word);
}

// ================ SEARCH ================

Node *RadixTree::traverseEdge(Node *node, const char *keySegment,
                              int &matchedLen) { // Yousef
  child *ch = node->children;
  while (ch != nullptr) {
    if (ch->firstChar == keySegment[0]) {
      matchedLen = matchPrefix(ch->node->data, keySegment);
      int nodeLen = strlen(ch->node->data);

      if (matchedLen == nodeLen) { // full edge match → can continue traversal
        return ch->node;
      }
      return nullptr; // partial match → key doesn't exist in tree
    }
    ch = ch->next;
  }
  matchedLen = 0;
  return nullptr;
}

bool RadixTree::isKeyFullyMatched(Node *node, const char *key,
                                  int keyLen) { // Yousef
  int nodeLen = strlen(node->data);
  return (keyLen == nodeLen) && node->ended;
}

void RadixTree::handleSearchFailure(const char *reason) { // Yousef
  std::cout << "Search failed: " << reason << std::endl;
}

bool RadixTree::search(const char *key) { // Yousef
  if (empty()) {
    handleSearchFailure("Tree is empty");
    return false;
  }

  int keyLen = strlen(key);
  int keyIndex = 0;

  // Check root first
  int rootMatch = matchPrefix(myRoot->data, key);
  int rootLen = strlen(myRoot->data);

  if (rootMatch < rootLen) {
    handleSearchFailure("Root prefix mismatch");
    return false;
  }

  keyIndex += rootMatch;

  // If key fully matched at root
  if (keyIndex == keyLen) {
    if (myRoot->ended) {
      return true;
    }
    handleSearchFailure(
        "Key is prefix of existing word but not marked as ended");
    return false;
  }

  // Traverse children starting from root
  Node *current = myRoot;
  while (keyIndex < keyLen) {
    int matchedLen = 0;
    Node *nextNode = traverseEdge(current, key + keyIndex, matchedLen);

    if (nextNode == nullptr) {
      handleSearchFailure("No matching child found");
      return false;
    }

    keyIndex += matchedLen;
    current = nextNode;
  }

  // Check if we've matched the entire key and it's marked as ended
  if (current->ended) {
    return true;
  }

  handleSearchFailure("Key found but not marked as complete word");
  return false;
}

// ================ AUTOCOMPLETE ================

void RadixTree::collectWords(Node *node, string currentWord) { // Nour
  if (!node)
    return;
  currentWord += node->data;
  if (node->ended) {
    cout << currentWord << endl;
  }
  child *ch = node->children;
  while (ch) {
    collectWords(ch->node, currentWord);
    ch = ch->next;
  }
}

void RadixTree::autoSuggest(const char *prefix) { // Nour
  if (empty()) {
    cout << "Tree is empty. No suggestions available." << std::endl;
    return;
  }
  Node *current = myRoot;
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
    Node *nextNode = traverseEdge(current, prefix + indexrn, matchedLen);
    if (nextNode == nullptr) {
      cout << "No suggestions found for the given prefix." << endl;
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

void RadixTree::collectAllWords(Node *node, string currentString) { // Jana
  if (node == nullptr)
    return;

  // If this node marks the end of a word, print
  if (node->ended) {
    cout << "- " << currentString << endl;
  }

  // Go through all children
  child *track = node->children;
  while (track != nullptr) {
    string nextString = currentString + string(track->node->data);
    collectAllWords(track->node, nextString);
    track = track->next;
  }
}

void RadixTree::getAutocompletions(const char *prefix) { // Jana
  if (myRoot == nullptr) {
    cout << "Tree is empty." << endl;
    return;
  }

  Node *current = myRoot;
  string builtString = "";

  int keyLen = strlen(prefix);
  int keyIndex = 0;

  // 1. Check Root first
  int rootMatch = matchPrefix(myRoot->data, prefix);

  if (rootMatch == 0 && keyLen > 0) {
    cout << "No suggestions found." << endl;
    return;
  }

  builtString += myRoot->data;
  keyIndex += rootMatch;

  // 2. Traverse down until whole prefix is covered
  while (keyIndex < keyLen) {
    int matchedLen = 0;
    Node *nextNode = traverseEdge(current, prefix + keyIndex, matchedLen);

    if (nextNode == nullptr) {
      cout << "No suggestions found." << endl;
      return;
    }

    builtString += nextNode->data;
    current = nextNode;
    keyIndex += matchedLen;
  }

  // 3. Collect and print all words below this point
  collectAllWords(current, builtString);
}

// ================ FREQUENCY & TIMESTAMP (Yousef) ================

long long RadixTree::getCurrentTimestamp() { // Yousef
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void RadixTree::mergeNodes(Node *parent, char edgeChar) { // Yousef
  if (!parent)
    return;

  child *ch = parent->children;
  while (ch != nullptr && ch->firstChar != edgeChar) {
    ch = ch->next;
  }

  if (!ch || !ch->node)
    return;

  Node *childNode = ch->node;

  // Only merge if child has exactly one child and is not a word endpoint
  if (childNode->getChildCount() == 1 && !childNode->ended) {
    child *grandchild = childNode->children;

    if (grandchild && grandchild->node) {
      char combinedLabel[100];
      strcpy(combinedLabel, childNode->data);
      strcat(combinedLabel, grandchild->node->data);
      strcpy(grandchild->node->data, combinedLabel);
      ch->node = grandchild->node;
      ch->firstChar = combinedLabel[0];
      delete childNode;
    }
  }
}

void RadixTree::updateWordFrequency(Node *node) { // Yousef
  if (node && node->ended) {
    node->frequency++;
    node->timestamp = getCurrentTimestamp();
  }
}

void RadixTree::incrementFrequency(const char *word) { // Yousef
  if (empty() || !word)
    return;

  int keyLen = strlen(word);
  int keyIndex = 0;
  int rootLen = strlen(myRoot->data);

  if (rootLen > 0) {
    int rootMatch = matchPrefix(myRoot->data, word);
    if (rootMatch < rootLen)
      return;

    keyIndex += rootMatch;

    if (keyIndex == keyLen) {
      updateWordFrequency(myRoot);
      return;
    }
  }

  Node *current = myRoot;
  while (keyIndex < keyLen) {
    int matchedLen = 0;
    Node *nextNode = traverseEdge(current, word + keyIndex, matchedLen);

    if (nextNode == nullptr)
      return;

    keyIndex += matchedLen;
    current = nextNode;
  }

  updateWordFrequency(current);
}

// ================ PREFIX SEARCH (Malak) ================

Node *RadixTree::findNodeForPrefix(const char *prefix) { // Malak
  if (empty() || prefix == nullptr)
    return nullptr;

  Node *current = myRoot;
  int index = 0;
  int prefixLen = strlen(prefix);

  int rootmatch = matchPrefix(current->data, prefix);

  if (rootmatch == 0)
    return nullptr;

  if (rootmatch >= prefixLen)
    return current;

  if (rootmatch < strlen(current->data))
    return nullptr;

  index = rootmatch;

  while (index < prefixLen) {
    child *ch = current->children;
    bool found = false;

    while (ch != nullptr) {
      if (ch->firstChar == prefix[index]) {
        int matched = matchPrefix(ch->node->data, prefix + index);

        if (matched == 0)
          return nullptr;

        if (index + matched >= prefixLen)
          return ch->node;

        if (matched < strlen(ch->node->data))
          return nullptr;

        index += matched;
        current = ch->node;
        found = true;
        break;
      }
      ch = ch->next;
    }

    if (!found)
      return nullptr;
  }

  return current;
}

// ================ END OF FILE ================
