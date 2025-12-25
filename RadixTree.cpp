#include "RadixTree.h"
#include <iostream>
#include <chrono>
#include <cstring>
using namespace std;
//========MODIFIED======

// ================= Constructors =================

RadixTree::RadixTree() {
    myRoot = nullptr;
}

RadixTree::~RadixTree() {
    destroy(myRoot);
}

void RadixTree::destroy(Node* node) {
    if (!node) return;
    child* ch = node->children;
    while (ch) {
        destroy(ch->node);
        child* old = ch;
        ch = ch->next;
        delete old;
    }
    delete node;
}

bool RadixTree::empty() {
    return myRoot == nullptr;
}

// ================ Helper Functions ================

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
    child* newchild = new child(childnode->data[0], childnode);
    newchild->next = parent->children;
    parent->children = newchild;
}

// ================= INSERT =================

void RadixTree::insert(const char* word) {
    if (empty()) {
        myRoot = new Node(word);
        myRoot->ended = true;
        myRoot->frequency = 1;
        myRoot->timestamp = getCurrentTimestamp();
        return;
    }

    Node* parent = nullptr;
    Node* current = myRoot;
    const char* w = word;

    while (current) {
        int p = matchPrefix(current->data, w);
        
        if (p < strlen(current->data)) { 
            // split required
            Node* old = new Node(current->data + p);
            old->ended = current->ended;
            old->frequency = current->frequency;
            old->timestamp = current->timestamp;
            old->children = current->children;

            current->data[p] = '\0';
            current->ended = false;
            current->children = nullptr;

            addchild(current, old);

            if (*(w + p) == '\0') {
                current->ended = true;
                current->frequency++;
                current->timestamp = getCurrentTimestamp();
                return;
            }

            Node* nw = new Node(w + p);
            nw->ended = true;
            nw->frequency = 1;
            nw->timestamp = getCurrentTimestamp();
            addchild(current, nw);
            return;
        }

        if (p == strlen(w)) {
            current->ended = true;
            current->frequency++;
            current->timestamp = getCurrentTimestamp();
            return;
        }

        // move to child
        child* ch = current->children;
        parent = current;
        current = nullptr;

        while (ch) {
            if (ch->firstChar == *(w + p)) {
                current = ch->node;
                break;
            }
            ch = ch->next;
        }

        if (!current) {
            Node* nw = new Node(w + p);
            nw->ended = true;
            nw->frequency = 1;
            nw->timestamp = getCurrentTimestamp();
            addchild(parent, nw);
            return;
        }

        w += p;
    }
}

// ================= SEARCH =================

bool RadixTree::search(const char* key) {
    if (empty()) return false;
    Node* current = myRoot;
    const char* k = key;

    while (current) {
        int p = matchPrefix(current->data, k);
        if (p < strlen(current->data)) return false;
        if (p == strlen(k)) return current->ended;

        child* ch = current->children;
        current = nullptr;
        while (ch) {
            if (ch->firstChar == *(k + p)) {
                current = ch->node;
                break;
            }
            ch = ch->next;
        }
        if (!current) return false;
        k += p;
    }
    return false;
}

// ================= DELETE =================

bool RadixTree::deleteWord(const char* word) {
    return deleteRec(myRoot, word);
}

bool RadixTree::deleteRec(Node*& node, const char* word) {
    if (!node) return false;

    int p = matchPrefix(node->data, word);
    int nLen = strlen(node->data);
    int wLen = strlen(word);

    if (p == 0) return false;

    if (p == wLen && p == nLen) {
        if (!node->ended) return false;
        node->ended = false;

        if (!node->children) {
            delete node;
            node = nullptr;
            return true;
        }
        return true;
    }

    if (p == nLen && p < wLen) {
        child* prev = nullptr;
        child* ch = node->children;

        while (ch && ch->firstChar != word[p]) {
            prev = ch;
            ch = ch->next;
        }
        if (!ch) return false;

        bool removed = deleteRec(ch->node, word + p);
        if (!removed) return false;

        if (!ch->node) {
            if (!prev) node->children = ch->next;
            else prev->next = ch->next;
            delete ch;
        }

        if (!node->ended && node->children && node->children->next == nullptr) {
            Node* childNode = node->children->node;
            strcat(node->data, childNode->data);
            node->ended = childNode->ended;
            node->children = childNode->children;
            delete childNode;
        }
        return true;
    }
    return false;
}

// ================= AUTOCOMPLETE =================

void RadixTree::collectWords(Node* node, const char* prefix) {
    if (!node) return;

    char newPrefix[100];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, node->data);

    if (node->ended) cout << newPrefix << endl;

    child* ch = node->children;
    while (ch) {
        collectWords(ch->node, newPrefix);
        ch = ch->next;
    }
}

void RadixTree::getAutocompletions(const char* prefix) {
    if (empty()) {
        cout << "Tree empty.\n";
        return;
    }

    Node* current = myRoot;
    const char* p = prefix;

    int pre = matchPrefix(current->data, p);
    if (pre == 0 && strlen(current->data) > 0) {
        cout << "No suggestions.\n";
        return;
    }

    p += pre;
    while (strlen(p) > 0) {
        child* ch = current->children;
        current = nullptr;

        while (ch) {
            int m = matchPrefix(ch->node->data, p);
            if (m > 0) {
                current = ch->node;
                p += m;
                break;
            }
            ch = ch->next;
        }

        if (!current) {
            cout << "No suggestions.\n";
            return;
        }
    }

    collectWords(current, prefix);
}

// =============== Timestamp & Frequency ===============

long long RadixTree::getCurrentTimestamp() {
    return chrono::duration_cast<chrono::milliseconds>(
        chrono::system_clock::now().time_since_epoch()).count();
}

void RadixTree::updateWordFrequency(Node* node) {
    if (node && node->ended) {
        node->frequency++;
        node->timestamp = getCurrentTimestamp();
    }
}

void RadixTree::incrementFrequency(const char* word) {
    if (!myRoot) return;
    search(word); // search updates frequency
}
