/******************************************************************************
 *
 *        ___   _____ _   _  _____ _   _  _____
 *       / _ \ / ____| | | ||  ___| \ | |/ ____|
 *      | |_| | (___ | | | || |__ |  \| | |  __
 *      |  _  |\___ \| | | ||  __|| . ` | | |_ |
 *      | | | |____) | |_| || |___| |\  | |__| |
 *      |_| |_|_____/ \___/ |_____|_| \_|\_____|
 *
 *                    Faculty of Engineering
 *                     Ain Shams University
 *
 ******************************************************************************
 *
 *  Copyright Ⓒ 2025 - Faculty of Engineering, Ain Shams University
 *  All Rights Reserved.
 *
 *  Project: Radix Tree Data Structure
 *  Course:  CSE243 - Data Structures & Algorithms
 *  Team:    Yousef, Nour, Malak, Jana, Lujain
 *
 ******************************************************************************/

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
  // first char stored, w childnode howa el node bta3 el child nafsaha
  child *newchild = new child(childnode->data[0], childnode);
  newchild->next =
      parent->children; // siblings bto3 el new child = children bto3 el parent
  parent->children = newchild; // parent children pointer updated lel new child
}

// ================ INSERT ================
// el insert feha cases kteera, kol case handled differently

void RadixTree::insert(const char *word) { // Nour + Malak
  // case1: tree fadya - root gedeed created
  if (empty()) {
    myRoot = new Node(word);
    myRoot->ended = true;
    myRoot->frequency = 1;                     // frequency initialized
    myRoot->timestamp = getCurrentTimestamp(); // timestamp set
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

      // el kelma mawgooda already - frequency updated
      if (prefix == nodeLen && prefix == wordLen) {
        ch->node->ended = true;
        ch->node->frequency++;                       // frequency incremented
        ch->node->timestamp = getCurrentTimestamp(); // timestamp updated
        return;
      }

      // el kelma el gdeeda = prefix mn node mawgooda - split required
      // example: "testing" exists, "test" inserted = split
      if (prefix == wordLen && prefix < nodeLen) {
        Node *oldnode = ch->node;       // el node el 2adeema
        Node *newnode = new Node(word); // node lel kelma el gdeeda
        newnode->ended = true;
        newnode->frequency = 1;
        newnode->timestamp = getCurrentTimestamp();

        // el ba2y mn el kelma el 2adeema stored f suffix
        Node *suffix = new Node(oldnode->data + prefix);
        suffix->ended = oldnode->ended;
        suffix->frequency = oldnode->frequency; // frequency preserved
        suffix->timestamp = oldnode->timestamp; // timestamp preserved
        suffix->children = oldnode->children;
        newnode->children = nullptr;
        addchild(newnode, suffix);
        ch->node = newnode;
        delete oldnode;
        return;
      }

      // el kelma el gdeeda akbar mn el node - el ba2y inserted as child
      // example: "test" exists, "testing" inserted = "ing" added as child
      if (prefix == nodeLen && wordLen > nodeLen) {
        Node *newNode = new Node(word + nodeLen); // el ba2y bs
        newNode->ended = true;
        newNode->frequency = 1;
        newNode->timestamp = getCurrentTimestamp();
        newNode->children = nullptr;
        addchild(ch->node, newNode);
        return;
      }

      // Partial overlap - node split to 2 parts
      // example: "team" exists, "test" inserted - common = "te"
      if (prefix > 0 && prefix < nodeLen) {
        Node *newTop = new Node(); // node gedeed lel common prefix
        strncpy(newTop->data, ch->node->data, prefix);
        newTop->data[prefix] = '\0';
        newTop->ended = false; // mesh kelma kamla

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

  // case 2: mafe4 common prefix - el kelma inserted directly
  Node *newnode = new Node(word);
  newnode->ended = true;
  newnode->frequency = 1;
  newnode->timestamp = getCurrentTimestamp();
  newnode->children = nullptr;
  addchild(parentNode, newnode);
}

// ================ DELETE ================
// delete b recursion - tree traversal w node deletion

bool RadixTree::deleteRec(Node *&current, const char *word) { // Lujain + Jana
  if (!current)
    return false; // node mafe4 - mesh mawgood

  int prefix = SearchPrefix(word, current);
  int len = strlen(current->data);
  int wlen = strlen(word);

  // prefix = 0 means el kelma mesh mawgooda
  if (prefix == 0)
    return false;

  // CASE 1: el kelma ends exactly here
  if (prefix == wlen && prefix == len) {
    if (!current->ended)
      return false; // kelma mesh stored - invalid word

    current->ended = false; // flag unmarked - kelma deleted

    // mafe4 children - node deleted 5ales
    if (current->children == nullptr) {
      delete current;
      current = nullptr;
      return true;
    }

    // child wa7ed bs - merged ma3 el parent (tree compression)
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
// search = tree traversal w checking lw el kelma mawgooda

Node *RadixTree::traverseEdge(Node *node, const char *keySegment,
                              int &matchedLen) { // Yousef
  child *ch = node->children;
  while (ch != nullptr) {
    if (ch->firstChar == keySegment[0]) {
      matchedLen = matchPrefix(ch->node->data, keySegment);
      int nodeLen = strlen(ch->node->data);

      if (matchedLen == nodeLen) { // full match - traversal continues
        return ch->node;
      }
      return nullptr; // partial match - key mesh mawgood
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

  // root checked first
  int rootMatch = matchPrefix(myRoot->data, key);
  int rootLen = strlen(myRoot->data);

  if (rootMatch < rootLen) {
    handleSearchFailure("Root prefix mismatch"); // root mesh matching
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
