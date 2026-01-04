/******************************************************************************
 *                                                                            *
 *        ___   _____ _   _  _____ _   _  _____                               *
 *       / _ \ / ____| | | ||  ___| \ | |/ ____|                              *
 *      | |_| | (___ | | | || |__ |  \| | |  __                               *
 *      |  _  |\___ \| | | ||  __|| . ` | | |_ |                              *
 *      | | | |____) | |_| || |___| |\  | |__| |                              *
 *      |_| |_|_____/ \___/ |_____|_| \_|\_____|                              *
 *                                                                            *
 *                    Faculty of Engineering                                  *
 *                     Ain Shams University                                   *
 *                                                                            *
 ******************************************************************************
 *                                                                            *
 *  Copyright Ⓒ 2026 - Faculty of Engineering, Ain Shams University          *
 *  All Rights Reserved to the students and the university.                   *
 *                                                                            *
 *  Project: Radix Tree Data Structure                                        *
 *  Course:  CSE243 - Data Structures & Algorithms                            *
 *  Team:    Yousef, Nour, Malak, Jana, Lujain                                *
 *                                                                            *
 ******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include "RadixTree.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// ================ CORE FUNCTIONS ================

RadixTree::RadixTree() { // Nour
  // Optimization: Sentinel Root for unified logic
  myRoot = new Node(""); // Root is always empty container
}

// Proper memory cleanup in destructor
RadixTree::~RadixTree() { // Lujain
  destroy(myRoot);
}

// Recursive memory cleanup function
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
  // With Sentinel Node, tree is empty if root has no children
  return myRoot->children == nullptr;
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
  // mosta5dam fel matchPrefix - ma7toot lel legacy compatibility
  return matchPrefix(word, currentNode->data);
}

void RadixTree::addchild(Node *parent, Node *childnode) { // Malak
  // first char stored, w childnode howa el node bta3 el child nafsaha
  child *newchild = new child(childnode->data[0], childnode);
  newchild->next =
      parent->children; // siblings bto3 el new child = children bto3 el parent
  parent->children = newchild; // parent children pointer updated lel new child
}

// ================ INSERT (Cleaned) ================
// el insert ba2a iterative w generic (infinite depth support)

void RadixTree::insert(const char *word) {
  // Case 1: Empty tree - insert first word directly
  if (empty()) {
    Node *newNode = new Node(word);
    newNode->ended = true;
    newNode->frequency = 1;
    newNode->timestamp = getCurrentTimestamp();
    addchild(myRoot, newNode);
    return;
  }

  Node *currentNode = myRoot;
  const char *remaining = word;

  while (true) {
    // Look for child with matching first character
    child *ch = currentNode->children;
    Node *nextNode = nullptr;

    while (ch) {
      if (ch->firstChar == *remaining) {
        nextNode = ch->node;
        break;
      }
      ch = ch->next;
    }

    // Case 2: No matching child - add as new child
    if (!nextNode) {
      Node *newNode = new Node(remaining);
      newNode->ended = true;
      newNode->frequency = 1;
      newNode->timestamp = getCurrentTimestamp();
      addchild(currentNode, newNode);
      return;
    }

    // Found matching child - check prefix match
    int prefix = SearchPrefix(remaining, nextNode);
    int nodeLen = strlen(nextNode->data);
    int remainingLen = strlen(remaining);

    // Case 3: Need to split current node (partial match)
    if (prefix < nodeLen) {
      // Create suffix node with remaining data from matched node
      Node *suffixNode = new Node(nextNode->data + prefix);
      suffixNode->ended = nextNode->ended;
      suffixNode->frequency = nextNode->frequency;
      suffixNode->timestamp = nextNode->timestamp;
      suffixNode->children = nextNode->children;

      // Truncate current node data (proper memory management)
      char *newData = new char[prefix + 1];
      strncpy(newData, nextNode->data, prefix);
      newData[prefix] = '\0';
      delete[] nextNode->data;
      nextNode->data = newData;

      nextNode->ended = false;
      nextNode->frequency = 0;
      nextNode->children = nullptr;

      // Add suffix as child
      addchild(nextNode, suffixNode);

      // If word ends exactly at split point
      if (remaining[prefix] == '\0') {
        nextNode->ended = true;
        nextNode->frequency = 1;
        nextNode->timestamp = getCurrentTimestamp();
        return;
      }

      // Create node for remaining part of new word
      Node *newWordNode = new Node(remaining + prefix);
      newWordNode->ended = true;
      newWordNode->frequency = 1;
      newWordNode->timestamp = getCurrentTimestamp();
      addchild(nextNode, newWordNode);
      return;
    }

    // Move past matched prefix
    remaining += prefix;
    remainingLen = strlen(remaining);

    // Case 4: Exact match at current node
    if (remainingLen == 0) {
      nextNode->ended = true;
      nextNode->frequency++;
      nextNode->timestamp = getCurrentTimestamp();
      return;
    }

    // Case 5: Continue with matching child
    currentNode = nextNode;
  }
}

// ================ SEARCH (Cleaned ================

bool RadixTree::search(const char *key) {
  if (empty())
    return false;

  Node *current = myRoot;
  int keyLen = strlen(key);
  int keyIndex = 0;

  while (keyIndex < keyLen) {
    char edgeChar = key[keyIndex];
    child *ch = current->children;
    child *found = nullptr;

    while (ch) { // loop el child searching
      if (ch->firstChar ==
          edgeChar) { // firstChar m3molaha cache f struct child for efficiency
                      // and avoid defereincing repeatedly
        found = ch;
        break; // found fa stip searching
      }
      ch = ch->next;
    }

    if (!found) {
      handleSearchFailure("No matching child found");
      return false;
    }

    int matched = matchPrefix(found->node->data, key + keyIndex);
    int nodeLen =
        strlen(found->node->data); // check how many nodes found between node
                                   // data and remaining key

    // Strict Search: Mismatch if we don't match the FULL node label
    // Lazem n-match el node label kolaha tb2a exact
    if (matched < nodeLen) {
      handleSearchFailure("Key ended mid-edge or mismatch");
      return false;
    }

    keyIndex += matched;
    current = found->node;
  }

  if (current->ended)
    return true;
  handleSearchFailure("Key found but not marked as complete word");
  return false;
}

Node *RadixTree::traverseEdge(Node *node, const char *keySegment,
                              int &matchedLen) {
  // Legacy helper kept for compatibility if needed bsh msh used
  child *ch = node->children;
  while (ch) {
    if (ch->firstChar == keySegment[0]) {
      matchedLen = matchPrefix(ch->node->data, keySegment);
      return ch->node;
    }
    ch = ch->next;
  }
  return nullptr;
}

bool RadixTree::isKeyFullyMatched(Node *node, const char *key, int keyLen) {
  return false; // Legacy unused
}

void RadixTree::handleSearchFailure(const char *reason) {
  // cout << "Search failed: " << reason << endl; // Silent for optimization
}

// ================ AUTOCOMPLETE (Cleaned) ================

// Helper function - bygama3 el kalemat recursively fel linked list
void RadixTree::collectSuggestions(Node *node, const std::string &currentWord,
                                   SuggestionList *results) {
  if (!node)
    return;

  if (node->ended) {
    results->append(currentWord, node->frequency, node->timestamp);
  }

  child *ch = node->children;
  while (ch) {
    std::string nextWord = currentWord + ch->node->data;
    collectSuggestions(ch->node, nextWord, results);
    ch = ch->next;
  }
}

// getSuggestions - byrag3 linked list lel GUI - el autocomplete el asasi
SuggestionList *RadixTree::getSuggestions(const char *prefix) {
  SuggestionList *results = new SuggestionList();

  if (empty() || prefix == nullptr || prefix[0] == '\0') {
    return results;
  }

  Node *current = myRoot;
  std::string builtString = "";
  int prefixLength = strlen(prefix);
  int index = 0;

  // nam4y 3al children 3a4an yela2y el prefix
  while (index < prefixLength) {
    child *ch = current->children;
    Node *nextNode = nullptr;
    int matchedLen = 0;

    // dawer 3al child ely first char byetabe2
    while (ch) {
      if (ch->firstChar == prefix[index]) {
        nextNode = ch->node;
        matchedLen = matchPrefix(nextNode->data, prefix + index);
        if (matchedLen > 0) {
          break;
        }
      }
      ch = ch->next;
    }

    // mafe4 match - raga3 fadya
    if (nextNode == nullptr || matchedLen == 0) {
      return results;
    }

    // check lw el prefix mesh kamel 3al edge
    int nodeLen = strlen(nextNode->data);
    if (index + matchedLen < prefixLength && matchedLen < nodeLen) {
      return results; // el prefix ma5ale4 3al edge
    }

    builtString += nextNode->data;
    current = nextNode;
    index += matchedLen;
  }

  // gama3 kol el kalemat men hena
  collectSuggestions(current, builtString, results);
  return results;
}

// autoSuggest - el autocomplete lel console (bytba3)
void RadixTree::autoSuggest(const char *prefix) {
  if (empty()) {
    cout << "Tree is empty. No suggestions available." << endl;
    return;
  }

  SuggestionList *suggestions = getSuggestions(prefix);

  if (suggestions->isEmpty()) {
    cout << "No suggestions found for the given prefix." << endl;
    delete suggestions;
    return;
  }

  SuggestionNode *curr = suggestions->head;
  while (curr) {
    cout << curr->word << endl;
    curr = curr->next;
  }
  delete suggestions;
}

// Wrapper for compatibility
void RadixTree::getAutocompletions(const char *prefix) { autoSuggest(prefix); }

// getAllSuggestions - byrag3 kol el kalemat fel tree
SuggestionList *RadixTree::getAllSuggestions() {
  SuggestionList *results = new SuggestionList();
  if (empty())
    return results;

  // gama3 men kol el children bto3 el root
  child *ch = myRoot->children;
  while (ch) {
    std::string word = ch->node->data;
    collectSuggestions(ch->node, word, results);
    ch = ch->next;
  }
  return results;
}

// ================ DELETION ================
// Updated to handle sentinel root

bool RadixTree::deleteWord(const char *word) { return deleteRec(myRoot, word); }

bool RadixTree::deleteRec(Node *&current, const char *word) {

  if (*word == '\0') {        // whole word is consumed
    if (current->ended) {     // this is the word
      current->ended = false; // unmark the word-no longer valid
      return current->children ==
             nullptr; // if no children= leaf, if children= keep
    }
    return false; // reached the end bu word not found
  }

  child *ch = current->children;
  child *prev = nullptr;

  while (ch) {
    if (ch->firstChar == word[0]) {
      int matched = matchPrefix(ch->node->data, word);
      if (matched == strlen(ch->node->data)) {
        // Should match full node to proceed
        bool shouldDeleteChild = deleteRec(ch->node, word + matched);

        if (shouldDeleteChild) {
          // Delete child from linked list
          if (prev)
            prev->next = ch->next;
          else
            current->children = ch->next;

          delete ch->node; // Delete Node
          delete ch;       // Delete Child Wrapper
          return current->children == nullptr && !current->ended &&
                 current != myRoot;
        }
        return false;
      }
    }
    prev = ch;
    ch = ch->next;
  }
  return false;
}

// ================ FREQUENCY & TIMESTAMP ================

long long RadixTree::getCurrentTimestamp() { // Yousef
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void RadixTree::mergeNodes(Node *parent, char edgeChar) { // Yousef
  // Kept for structural maintenance, unused in strict iterative mode unless we
  // add compression
}

void RadixTree::updateWordFrequency(Node *node) { // Yousef
  if (node && node->ended) {
    node->frequency++;
    node->timestamp = getCurrentTimestamp();
  }
}

void RadixTree::writeWordsToFile(Node *node, string currentString,
                                 ofstream &file) {
  if (!node)
    return;

  // If node is a word end, save it to file
  if (node->ended) {
    file << currentString << endl;
  }

  // Recursively save children
  child *ch = node->children;
  while (ch) {
    writeWordsToFile(ch->node, currentString + string(ch->node->data), file);
    ch = ch->next;
  }
}

void RadixTree::saveToFile(const char *filename) {
  ofstream outFile(filename);
  if (!outFile) {
    cout << "Error opening file for writing: " << filename << endl;
    return;
  }

  if (myRoot) {
    writeWordsToFile(myRoot, "", outFile);
  }

  outFile.close();
  cout << "Tree saved to " << filename << endl;
}

void RadixTree::loadFromFile(const char *filename) {
  ifstream inFile(filename);
  if (!inFile) {
    cout << "Error opening file for reading: " << filename << endl;
    return;
  }

  string word;
  int count = 0;
  auto start = std::chrono::high_resolution_clock::now();

  while (getline(inFile, word)) {
    if (!word.empty()) {
      if (word.back() == '\r')
        word.pop_back(); // Handle Windows line endings
      insert(word.c_str());
      count++;
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  inFile.close();
  cout << "Loaded " << count << " words from " << filename << " in "
       << duration.count() << "ms" << endl;
}
