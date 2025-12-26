#pragma once
#include "Node.h"
//=========MODIFIED=====

class RadixTree {
public:
    Node* myRoot;

    RadixTree();  //Nour
    ~RadixTree();  //nour

    bool empty();   //nour
    void insert(const char* word);  //nour - malak
    void addchild(Node* parent, Node* childnode); //malak
    int matchPrefix(const char* nodePrefix, const char* keySegment); //yousef
    int SearchPrefix(const char* word, Node* currectNode); //nour

    // search  //yousef
    bool search(const char* key);
    Node* traverseEdge(Node* node, const char* keySegment, int& matchedLen);
    bool isKeyFullyMatched(Node* node, const char* key, int keyLen);
    void handleSearchFailure(const char* reason);

    // deletion  //lujain
    bool deleteWord(const char* word);
    bool deleteRec(Node*& current, const char* word);

    // autocomplete
    void collectWords(Node* node, const char* prefix);  //nour
    void getAutocompletions(const char* prefix);  //jana

    // extras  //yousef
    long long getCurrentTimestamp();  
    void updateWordFrequency(Node* node);
    void incrementFrequency(const char* word);

    // internal memory cleanup
    void destroy(Node* node);  //lujain
};

