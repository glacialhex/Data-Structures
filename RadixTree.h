#pragma once
#include "Node.h"
#include <string>

// HELLO THIS IS MADE IN ROMANIA
// RadixTree.h - el class definition lel RadixTree data structure
// RadixTree = compressed trie - as7al lel autocomplete w prefix search

class RadixTree {
public:
  Node *myRoot; // el root bta3 el tree

  // Constructor w Destructor
  RadixTree();
  ~RadixTree();
  // Basic Operations - el 3amaleyat el asaseyya
  bool empty();                  // check lw el tree fadya
  void insert(const char *word); // d5ol kelma gdeeda fel tree
  int SearchPrefix(const char *word, Node *currectNode); // dawer 3al prefix
  void addchild(Node *parent, Node *childnode);          // add child lel parent

  // Delete Operations - el mas7
  bool deleteWord(const char *word);                // ems7 kelma
  bool deleteRec(Node *&current, const char *word); // recursive delete

  // Autocomplete - el ektmal el tol2a2y
  void getAutocompletions(const char *prefix); // geb kol el suggestions
  void collectAllWords(Node *node,
                       std::string currentString); // collect kol el kalemat
  void collectWords(Node *node, std::string currentWord); // collect recursively
  void autoSuggest(const char *prefix); // suggest words lel prefix
  // Search / Lookup - el dawran w el search
  bool search(const char *key); // dawer 3ala kelma
  Node *traverseEdge(Node *node, const char *keySegment,
                     int &matchedLen); // traverse edge wa7da
  int matchPrefix(const char *nodePrefix,
                  const char *keySegment); // 2aren el prefix
  bool isKeyFullyMatched(Node *node, const char *key,
                         int keyLen);           // check full match
  void handleSearchFailure(const char *reason); // handle el fail cases

  // Helper Functions - functions mosa3da
  long long getCurrentTimestamp(); // geb el wa2t el 7aly
  void mergeNodes(Node *parent,
                  char edgeChar);              // merge el nodes el single-child
  void updateWordFrequency(Node *node);        // update el frequency
  void incrementFrequency(const char *word);   // zawed el frequency
  Node *findNodeForPrefix(const char *prefix); // la2y el node lel prefix

  // Memory Management - el memory cleanup
  void destroy(Node *node); // delete kol el nodes recursively
};
