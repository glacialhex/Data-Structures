#pragma once
#include "Node.h"
#include <string>

// RadixTree.h - el class definition lel RadixTree data structure
// RadixTree = compressed trie - as7al lel autocomplete w prefix search

// SuggestionNode - linked list node lel suggestions (badal vector)
struct SuggestionNode {
  std::string word;
  int frequency;
  long long timestamp;
  SuggestionNode *next;

  SuggestionNode(const std::string &w, int f, long long t)
      : word(w), frequency(f), timestamp(t), next(nullptr) {}
};

// SuggestionList - linked list lel suggestions (badal vector)
struct SuggestionList {
  SuggestionNode *head;
  SuggestionNode *tail;
  int count;

  SuggestionList() : head(nullptr), tail(nullptr), count(0) {}

  ~SuggestionList() {
    SuggestionNode *curr = head;
    while (curr) {
      SuggestionNode *next = curr->next;
      delete curr;
      curr = next;
    }
  }

  // add suggestion lel end
  void append(const std::string &word, int frequency, long long timestamp) {
    SuggestionNode *newNode = new SuggestionNode(word, frequency, timestamp);
    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    count++;
  }

  bool isEmpty() const { return head == nullptr; }
  int size() const { return count; }
};

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

  // Autocomplete - el ektmal el telqa2y
  SuggestionList *
  getSuggestions(const char *prefix);  // geb el suggestions lel GUI
  SuggestionList *getAllSuggestions(); // geb kol el kalemat fel tree
  void
  getAutocompletions(const char *prefix); // geb kol el suggestions (console)
  void autoSuggest(const char *prefix);   // el autocomplete el gdeed

private:
  void collectSuggestions(Node *node, const std::string &currentWord,
                          SuggestionList *results); // bygama3 fel list

public:
  // Search / Lookup - el dawreyya
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

  // Memory Management - el memory cleanup
  void destroy(Node *node); // delete kol el nodes recursively

  // Persistence - el 7efz w el esterga3
  void saveToFile(const char *filename);   // save tree to file
  void loadFromFile(const char *filename); // load tree from file

  // Persistence Helper
  void writeWordsToFile(Node *node, std::string currentString,
                        std::ofstream &file);
};
