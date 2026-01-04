#pragma once
#include "Node.h"
#include <string>

// RadixTree.h - el class definition lel RadixTree data structure
// RadixTree = compressed trie - as7al lel autocomplete w prefix search

class RadixTree {
public:
  Node *myRoot; // el root bta3 el tree

  // Constructor w Destructor
  RadixTree();
  ~RadixTree();
  // Basic Operations - el 3amaleyat el asaseyya
  bool empty();                            // check lw el tree fadya
  Node *getRoot() const { return myRoot; } // Qt GUI accessor
  void insert(const char *word);           // d5ol kelma gdeeda fel tree
  int SearchPrefix(const char *word, Node *currectNode); // dawer 3al prefix
  void addchild(Node *parent, Node *childnode);          // add child lel parent

  // Delete Operations - el mas7
  bool deleteWord(const char *word);                // ems7 kelma
  bool deleteRec(Node *&current, const char *word); // recursive delete

  // Autocomplete - el ektmal el tol2a2y
  void getAutocompletions(const char *prefix); // geb kol el suggestions
  void autoSuggest(const char *prefix);        // el autocomplete el gdeed
  void collectWords(Node *node,
                    const std::string &prefix); // collect recursively
  void collectAllWords(Node *node,
                       std::string currentString); // collect kol el kalemat
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
                  char edgeChar);       // merge el nodes el single-child
  void updateWordFrequency(Node *node); // update el frequency

  // New Helper for optimization
  void splitNode(Node *parent, child *ch,
                 int matchedLen); // split node lel iteraitve insert

  // Memory Management - el memory cleanup
  void destroy(Node *node); // delete kol el nodes recursively

  // Persistence - el 7efz w el esterga3
  void saveToFile(const char *filename);   // save tree to file
  void loadFromFile(const char *filename); // load tree from file

  // Persistence Helper
  void writeWordsToFile(Node *node, std::string currentString,
                        std::ofstream &file);
};
