# Autocomplete System

An autocomplete/autosuggest application built on top of the Radix Tree data structure.

**Status:** 🔄 Under Construction

---

## Overview

This application extends the base Radix Tree implementation to provide Google-style autocomplete functionality with ranked suggestions based on word frequency and recency.

## Planned Features

- Real-time prefix-based suggestions
- Frequency-based ranking (most searched terms appear first)
- Top-K suggestions retrieval
- Clean, usable API

---

## Implementation Tasks

### 1. Prefix Search Engine (Core Prefix Navigation)

**Goal:** Upgrade and optimize the search side of the radix tree.

**Responsibilities:**
- Implement a robust `findNodeForPrefix(prefix)` function
- Make traversal efficient and correct
- Add safeguards so prefix search works in all cases
- Modify insertion logic only if required to support searching speed

**Why this matters:** This is the heart of autocomplete. Everything starts from the prefix node.

**Difficulty:** Medium  
**Time:** 2-3 days

**Output:** A function that returns a pointer to the prefix node.

---

### 2. Autocomplete Logic (Collect All Words Under Prefix)

**Goal:** Using the prefix node, collect ALL possible completions.

**Responsibilities:**
- Implement DFS traversal from the prefix node
- Build `collectAllWords(node, currentString, vector<string>& results)`
- Ensure it handles:
  - Deep nested children
  - Compressed labels
  - Merging node data properly
- Return a clean list of completions

**Difficulty:** Medium  
**Time:** 2-4 days

**Output:** Autocomplete result list

---

### 3. Autosuggest Ranking Engine (Frequency + Top K System)

**Goal:** This is the "smart brain" of the system.

**Responsibilities:**
- Add a frequency counter inside Node
- Update frequency every time a word is inserted OR chosen
- Add timestamps or priority fields (optional)
- Implement `getTopKSuggestions(prefixNode, K)`:
  - Collect words (uses function from Task 2)
  - Sort by frequency (or score = freq + recency)
  - Return top K suggestions

**Difficulty:** Medium/Hard  
**Time:** 4-6 days

**Output:** Sorted ranked suggestions, Google-style

---

### 4. Integrator + System API Layer (Glue Everything Together)

**Goal:** Create the high-level API that connects all components.

**Responsibilities:**
- Create the high-level API:
  - `vector<string> autocomplete(string prefix)`
  - `vector<string> autosuggest(string prefix, int K)`
- Connect Task 1, 2, and 3 logic together
- Handle edge cases:
  - Prefix not found
  - Empty radix tree
  - Prefix is a full word
- Optimize performance where necessary

**Difficulty:** Medium  
**Time:** 3-5 days

**Output:** A clean, usable, well-structured API

---

### 5. Radix Tree Enhancements + Cleanup (Heavy Worker)

**Goal:** Prepare the radix tree to support new features.

**Responsibilities:**
- Add frequency fields to Node struct
- Add "isWord" markers
- Add scoring metrics
- Add helper functions like:
  - `isLeaf(node)`
  - `mergeNodes(nodeA, nodeB)` (if needed)
- Clean insertion/search code if required for compatibility
- Help Task 4 integrate suggestion logic inside the tree

**Difficulty:** Medium/Heavy  
**Time:** 4-6 days

---

## API Reference (Planned)

```cpp
// Get all words that start with prefix
vector<string> autocomplete(string prefix);

// Get top K suggestions for prefix, ranked by frequency
vector<string> autosuggest(string prefix, int K);
```

## Node Structure (Enhanced)

```cpp
struct Node {
    char data[50];      // Edge label
    bool ended;         // Is this a complete word?
    int frequency;      // How many times this word was used (NEW)
    child* children;    // Linked list of children
};
```

---

## Progress

| Task | Description | Status |
|------|-------------|--------|
| 1 | Prefix Search Engine | 🔄 Not Started |
| 2 | Autocomplete Logic | 🔄 Not Started |
| 3 | Ranking Engine | 🔄 Not Started |
| 4 | API Layer | 🔄 Not Started |
| 5 | Tree Enhancements | 🔄 Not Started |

---

## Dependencies

This application requires the base Radix Tree implementation to be complete:
- ✅ Insert operation
- ✅ Search operation  
- ✅ Delete operation
