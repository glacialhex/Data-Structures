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
 *
 ******************************************************************************
 *
 *  Copyright Ⓒ 2025 - Faculty of Engineering, Ain Shams University
 *  All Rights Reserved to the students and the university.
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
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// ================ CORE FUNCTIONS ================

RadixTree::RadixTree() { // Nour
  // Optimization: Sentinel Root for unified logic
  myRoot = new Node(""); // Root is always empty container
}

// Added: Proper memory cleanup in destructor
RadixTree::~RadixTree() { // Lujain
  destroy(myRoot);
}

// Added: Recursive memory cleanup function
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
  // Used by matchPrefix but kept for legacy API compatibility if needed
  // internally
  return matchPrefix(word, currentNode->data);
}

void RadixTree::addchild(Node *parent, Node *childnode) { // Malak
  // first char stored, w childnode howa el node bta3 el child nafsaha
  child *newchild = new child(childnode->data[0], childnode);
  newchild->next =
      parent->children; // siblings bto3 el new child = children bto3 el parent
  parent->children = newchild; // parent children pointer updated lel new child
}

// Helper to split a node when partial match occurs
void RadixTree::splitNode(Node *parent, child *ch, int matchedLen) {
  Node *current = ch->node;
  // create new node holding the remaining suffix of current node
  Node *newChild = new Node(current->data + matchedLen);

  // copy properties to new suffix node
  newChild->ended = current->ended;
  newChild->frequency = current->frequency;
  newChild->timestamp = current->timestamp;
  newChild->children = current->children; // Suffix takes the children

  // Update current node to be the prefix
  current->data[matchedLen] = '\0'; // Truncate data
  current->ended = false;           // Prefix is not an end (usually)
  current->children = nullptr;      // Children moved to suffix

  // Add suffix as child of current
  addchild(current, newChild);
}

// ================ INSERT (OPTIMIZED) ================
// el insert ba2a iterative w generic (infinite depth support)

void RadixTree::insert(const char *word) {
  Node *current = myRoot;
  int keyLen = strlen(word);
  int keyIndex = 0;

  while (keyIndex < keyLen) {
    char edgeChar = word[keyIndex];
    child *ch = current->children;
    child *found = nullptr;

    // 1. Find edge matching the character
    while (ch != nullptr) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }

    // Case A: No edge found - create new child
    if (!found) {
      Node *newNode = new Node(word + keyIndex);
      newNode->ended = true;
      newNode->frequency = 1;
      newNode->timestamp = getCurrentTimestamp();
      addchild(current, newNode);
      return;
    }

    // Case B: Edge found - match prefix
    int matched = matchPrefix(found->node->data, word + keyIndex);
    int nodeLen = strlen(found->node->data);

    // Case B1: Partial Match (Split Node) - el kelma mo5talefa 3n el node
    if (matched < nodeLen) {
      splitNode(current, found, matched);
      // Now found->node is the prefix. We continue validation.
    }

    // Update keyIndex by matched length
    keyIndex += matched;

    // Case B2: Partial Key Match (New Branch needed)
    // If we split, we might still have key chars remaining
    if (keyIndex < keyLen) {
      // If we split above, found->node is now the prefix (len == matched)
      // If we didn't split (matched == nodeLen), we traverse down
      current = found->node;
      // Loop continues to find next edge or add new child
    } else {
      // Case B3: Key Fully Consumed (Word Ends Here)
      // Mark found node as ended
      found->node->ended = true;
      found->node->frequency++;
      found->node->timestamp = getCurrentTimestamp();
      return;
    }
  }
}

// ================ SEARCH (OPTIMIZED) ================

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

    while (ch) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }

    if (!found) {
      handleSearchFailure("No matching child found");
      return false;
    }

    int matched = matchPrefix(found->node->data, key + keyIndex);
    int nodeLen = strlen(found->node->data);

    // Strict Search: Mismatch if we don't match the FULL node label
    // Lazem n-match el node label kolaha exact
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
  // Legacy helper kept for compatibility if needed, but unused in new search
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

// ================ AUTOCOMPLETE (OPTIMIZED) ================

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

void RadixTree::getAutocompletions(const char *prefix) { // Jana - Refactored
  // Sentinel root simplified everything
  Node *current = myRoot;
  string builtString = "";
  int keyLen = strlen(prefix);
  int keyIndex = 0;

  while (keyIndex < keyLen) {
    char edgeChar = prefix[keyIndex];
    child *ch = current->children;
    child *found = nullptr;

    while (ch) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }

    if (!found) {
      cout << "No suggestions found." << endl;
      return;
    }

    int matched = matchPrefix(found->node->data, prefix + keyIndex);
    // If we consumed more of prefix than matched, mismatch
    if (keyIndex + matched < keyLen && matched < strlen(found->node->data)) {
      cout << "No suggestions found." << endl;
      return;
    }

    builtString += found->node->data;
    current = found->node;
    keyIndex += matched;
  }

  // Correct the built string if we overshot (not really needed if suffix is
  // collecting) Actually, builtString accumulates full node data. If prefix
  // matched partially into node, we wanted that full node for suggestions.
  // Example: Prefix "ca", Node "cart". Matched "ca". keyIndex done. builtstring
  // "cart". Correct.

  collectAllWords(current, builtString);
}

// ================ DELETION ================
// Updated to handle sentinel root

bool RadixTree::deleteWord(const char *word) { return deleteRec(myRoot, word); }

bool RadixTree::deleteRec(Node *&current, const char *word) {
  // If current is sentinel (root), finding child logic is slightly different
  // Actually Logic:
  // Recursively find the node.
  // Unmark ended.
  // If leaf -> delete (return true).

  // Simplification: We traverse down.
  // Since delete is complex with merging, keeping it simple for now.
  // Just unmarking is safe.

  // For now, let's just use Search logic to find and unmark,
  // and basic pruning if leaf.

  // Re-using search logic implies iteration.
  // But modification requires reference updates.

  // Let's implement a clean recursive delete that works with sentinel.

  if (*word == '\0') {
    if (current->ended) {
      current->ended = false;
      return current->children == nullptr; // Return true if leaf now
    }
    return false;
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
