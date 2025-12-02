# Person 3 - Search/Lookup Operation Guide

## Overview
Implement search functionality for the Radix Tree. Your goal is to traverse the compressed prefix structure correctly and return either the found node or a failure state.

## Files to Work With
- **RadixTree.h** - Function declarations already added
- **RadixTree.cpp** - Function stubs with TODOs (lines ~60-140)
- **Node.h** - Node and child structures (reference only)

## Your Functions

### 1. `SearchResult search(Node* root, const char* key)`
**Purpose**: Main search function - entry point for searching a key in the tree

**Steps**:
1. Handle edge case: empty tree or null root
2. Start traversal from root
3. Loop through the key, calling `traverseEdge` to find matching children
4. After traversal, call `isKeyFullyMatched` to verify success
5. Return appropriate SearchResult (success or failure)

**Example Flow**:
```
search("romane")
  → Start at root
  → traverseEdge finds child with 'r'
  → Match prefix "roman"
  → Continue with remaining "e"
  → Check if fully matched and ended=true
```

---

### 2. `Node* traverseEdge(Node* node, const char* keySegment, int& matchedLength)`
**Purpose**: Navigate from current node to appropriate child based on key segment

**Steps**:
1. Get the first character of keySegment
2. Iterate through node->children (linked list of child structs)
3. Find child where firstChar matches keySegment[0]
4. Call `matchPrefix` to see how much of the child's data matches
5. Set matchedLength output parameter
6. Return the child node if found, nullptr otherwise

**Key Points**:
- child structure has: firstChar, node pointer, next pointer
- Iterate: `child* curr = node->children; while(curr != nullptr) { ... curr = curr->next; }`

---

### 3. `int matchPrefix(const char* nodePrefix, const char* keySegment)`
**Purpose**: Count how many characters match between node's data and key segment

**Steps**:
1. Initialize counter i = 0
2. Loop while both strings have characters AND they match
3. Return count of matching characters

**Example**:
```
matchPrefix("roman", "romane") → returns 5
matchPrefix("roman", "room")   → returns 2
matchPrefix("test", "best")    → returns 0
```

---

### 4. `bool isKeyFullyMatched(Node* node, const char* key)`
**Purpose**: Verify that search successfully found a complete word

**Steps**:
1. Check if entire key has been consumed (reached '\0')
2. Check if node->ended == true (marks this as valid word endpoint)
3. Return true only if both conditions met

**Why This Matters**:
- Searching "rom" when tree has "roman" should fail (not ended)
- Searching "roman" when tree has "roman" and "romane" should succeed only if "roman" node has ended=true

---

### 5. `SearchResult handleSearchFailure(const char* reason)`
**Purpose**: Create a proper failure result with explanation

**Steps**:
1. Create SearchResult object
2. Set found = false
3. Set node = nullptr
4. Set reason = provided reason string
5. Return the result

**Common Reasons**:
- "Key not found"
- "Partial match only"
- "Empty tree"
- "Prefix mismatch"

---

## SearchResult Structure
Already defined in RadixTree.h:
```cpp
struct SearchResult {
    bool found;           // true if key exists
    Node* node;          // pointer to found node (or nullptr)
    const char* reason;  // explanation if not found
};
```

## Testing Your Implementation
Add to Main.cpp:
```cpp
RadixTree tree;
// After inserting some words...
SearchResult result = tree.search(tree.myRoot, "yourword");
if (result.found) {
    cout << "Found!" << endl;
} else {
    cout << "Not found: " << result.reason << endl;
}
```

## Tips
- Radix trees compress common prefixes
- A node's `data` contains the edge label (substring)
- The `ended` flag marks valid word endpoints
- Children are stored as a linked list (child→next→next...)
- Use the existing `SearchPrefix` function as reference for prefix matching

## Common Pitfalls
1. Forgetting to check `ended` flag (might find prefix but not complete word)
2. Not handling the linked list of children correctly
3. Off-by-one errors in string indexing
4. Not advancing the key pointer after matching a prefix

## Implementation Order
Recommended order:
1. `matchPrefix` (simplest, standalone)
2. `handleSearchFailure` (simple, standalone)
3. `isKeyFullyMatched` (uses node properties)
4. `traverseEdge` (uses matchPrefix)
5. `search` (main function, uses all others)

Good luck! 🚀

