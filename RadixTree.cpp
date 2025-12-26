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
    // case1: empty tree
    if (empty()) {
        myRoot = new Node(word);
        myRoot->ended = true;
        myRoot->frequency = 1;
        myRoot->timestamp = getCurrentTimestamp();
        return;
    }

    // Special case: if root has data and first char doesn't match,
    // we need to create a new empty root
    if (myRoot->data[0] != '\0' && myRoot->data[0] != word[0]) {
        // Create new empty root
        Node* newRoot = new Node(""); // Empty string

        // Move current root to be a child
        addchild(newRoot, myRoot);

        // Create new node for the new word
        Node* newNode = new Node(word);
        newNode->ended = true;
        newNode->frequency = 1;
        newNode->timestamp = getCurrentTimestamp();
        addchild(newRoot, newNode);

        // Update root
        myRoot = newRoot;
        return;
    }

    Node* currentNode = myRoot;
    Node* parentNode = nullptr;
    child* currentChild = nullptr;
    const char* remaining = word;

    while (true) {
        int prefix = SearchPrefix(remaining, currentNode);
        int nodeLen = strlen(currentNode->data);
        int remainingLen = strlen(remaining);

        // Case 1: Need to split current node
        if (prefix < nodeLen) {
            // Split the current node
            Node* suffixNode = new Node(currentNode->data + prefix);
            suffixNode->ended = currentNode->ended;
            suffixNode->frequency = currentNode->frequency;
            suffixNode->timestamp = currentNode->timestamp;
            suffixNode->children = currentNode->children;

            // Truncate current node
            currentNode->data[prefix] = '\0';
            currentNode->ended = false;
            currentNode->frequency = 0;
            currentNode->children = nullptr;

            // Add suffix as child
            addchild(currentNode, suffixNode);

            // If word ends exactly here
            if (remaining[prefix] == '\0') {
                currentNode->ended = true;
                currentNode->frequency = 1;
                currentNode->timestamp = getCurrentTimestamp();
                return;
            }

            // Create node for remaining part of new word
            Node* newWordNode = new Node(remaining + prefix);
            newWordNode->ended = true;
            newWordNode->frequency = 1;
            newWordNode->timestamp = getCurrentTimestamp();
            addchild(currentNode, newWordNode);
            return;
        }

        // Move past matched prefix
        remaining += prefix;
        remainingLen = strlen(remaining);

        // Case 2: Exact match at current node
        if (remainingLen == 0) {
            currentNode->ended = true;
            currentNode->frequency++;
            currentNode->timestamp = getCurrentTimestamp();
            return;
        }

        // Look for child with matching first character
        child* ch = currentNode->children;
        Node* nextNode = nullptr;
        child* prevChild = nullptr;

        while (ch) {
            if (ch->firstChar == *remaining) {
                nextNode = ch->node;
                currentChild = ch;
                break;
            }
            prevChild = ch;
            ch = ch->next;
        }

        // Case 3: No matching child - add as new child
        if (!nextNode) {
            Node* newNode = new Node(remaining);
            newNode->ended = true;
            newNode->frequency = 1;
            newNode->timestamp = getCurrentTimestamp();
            addchild(currentNode, newNode);
            return;
        }

        // Case 4: Continue with matching child
        parentNode = currentNode;
        currentNode = nextNode;
    }
}

// ================= SEARCH =================

bool RadixTree::search(const char* word) {
    if (empty()) return false;

    Node* current = myRoot;
    const char* remaining = word;

    // If root has empty data, skip the prefix check
    if (current->data[0] != '\0') {
        int prefixLen = matchPrefix(current->data, remaining);
        if (prefixLen < strlen(current->data)) {
            return false;
        }
        remaining += prefixLen;
    }

    while (current) {
        // If we've consumed all characters
        if (*remaining == '\0') {
            return current->ended;
        }

        // Look for child with matching next character
        child* ch = current->children;
        Node* nextNode = nullptr;

        while (ch) {
            if (ch->firstChar == *remaining) {
                nextNode = ch->node;
                break;
            }
            ch = ch->next;
        }

        if (!nextNode) {
            return false;
        }

        // Check prefix match with next node
        int prefixLen = matchPrefix(nextNode->data, remaining);
        if (prefixLen < strlen(nextNode->data)) {
            return false;
        }

        remaining += prefixLen;
        current = nextNode;
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

void RadixTree::getAutoSuggestions(const char* prefix) {
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


