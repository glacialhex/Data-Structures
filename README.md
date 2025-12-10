<div align="center">

# Radix Tree Implementation

[![C++](https://img.shields.io/badge/C++-11-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-064F8C.svg?style=flat&logo=cmake)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Educational-green.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Complete-brightgreen.svg)]()

**A C++ implementation of a Radix Tree (Patricia Trie)**  
*Optimized for efficient prefix-based string storage and retrieval*

[Features](#features) • [Quick Start](#quick-start) • [Usage](#usage-examples) • [Team](#team-structure)

</div>

---

## Overview

A **Radix Tree** (also known as **Patricia Trie** or **Compact Prefix Tree**) is a space-optimized trie data structure where nodes with single children are merged with their parents. This structure enables:

- **Fast Operations**: O(k) time complexity for search, insert, and delete
- **Space Efficiency**: Compressed storage of common prefixes
- **Prefix Matching**: Ideal for autocomplete, IP routing, and text indexing

### Application: Autocomplete System (Under Construction)

This project will be extended to implement a **Google-style autocomplete/autosuggest system** with ranked suggestions based on frequency and recency.

**Planned Features:**
- Prefix search with real-time suggestions
- Frequency-based ranking (most searched terms appear first)
- Top-K suggestions retrieval
- Clean, usable API layer

**Implementation Roadmap:**

| Task | Description | Status |
|------|-------------|--------|
| Prefix Search Engine | Implement `findNodeForPrefix(prefix)` for efficient prefix navigation | 🔄 Planned |
| Autocomplete Logic | DFS traversal to collect all words under a prefix node | 🔄 Planned |
| Ranking Engine | Add frequency counters, implement `getTopKSuggestions(prefixNode, K)` | 🔄 Planned |
| API Layer | Create `autocomplete(prefix)` and `autosuggest(prefix, K)` functions | 🔄 Planned |
| Tree Enhancements | Add frequency fields, scoring metrics, and helper functions | 🔄 Planned |

## Project Structure

```
Data-Structures/
├── Main.cpp          # Entry point with test cases
├── RadixTree.h       # Radix Tree class declaration
├── RadixTree.cpp     # Radix Tree implementation
├── Node.h            # Node and child structures
├── Node.cpp          # Node implementation
└── README.md         # Project documentation
```

## Features

### Implemented Operations

| Operation | Functions | Status |
|-----------|-----------|--------|
| **Insert** | `insert()`, `addchild()`, `SearchPrefix()` | ✅ Complete |
| **Search** | `search()`, `traverseEdge()`, `matchPrefix()`, `isKeyFullyMatched()`, `handleSearchFailure()` | ✅ Complete |
| **Delete** | `deleteWord()`, `deleteRec()` | ✅ Complete |

### Test Suite (Main.cpp)
- 16 insertion tests
- 26 search tests (words found + words not found)
- 5 deletion tests
- Automatic pass/fail reporting with summary

## Data Structures

### Node Structure
```cpp
struct Node {
    char data[50];      // Edge label (compressed prefix)
    bool ended;         // Marks end of valid word
    child* children;    // Linked list of children
};
```

### Child Structure
```cpp
struct child {
    char firstChar;     // First character of child's label
    Node* node;         // Pointer to child node
    child* next;        // Next sibling in linked list
};
```

## Quick Start

### Prerequisites
- C++11 compatible compiler (GCC 4.8+, Clang 3.3+, or MSVC 2015+)
- IDE: Visual Studio, CLion, or VS Code (optional)

### Build & Run

<details>
<summary><b>Using CLion (Recommended)</b></summary>

1. **Open Project**
   ```
   File → Open → Select Data-Structures folder
   ```

2. **Create CMakeLists.txt** (if not present)
   ```cmake
   cmake_minimum_required(VERSION 3.10)
   project(RadixTree)
   set(CMAKE_CXX_STANDARD 11)
   add_executable(RadixTree Main.cpp RadixTree.cpp Node.cpp)
   ```

3. **Build & Run**
   ```
   Build:  Ctrl + F9  (Cmd + F9 on Mac)
   Run:    Shift + F10 (Ctrl + R on Mac)
   Debug:  Shift + F9  (Ctrl + D on Mac)
   ```

</details>

<details>
<summary><b>Using Visual Studio</b></summary>

1. Open Visual Studio
2. File → Open → Folder → Select Data-Structures folder
3. Right-click on `Main.cpp` → Set as Startup Item
4. Build: `Ctrl + Shift + B`
5. Run: `F5` (Debug) or `Ctrl + F5` (Run without debugging)

</details>

<details>
<summary><b>Using VS Code</b></summary>

1. Open the folder in VS Code
2. Install the C/C++ extension (Microsoft)
3. Press `Ctrl + Shift + B` to build (configure tasks.json if needed)
4. Or use the integrated terminal:
   ```bash
   g++ -o RadixTree.exe Main.cpp RadixTree.cpp Node.cpp
   ./RadixTree.exe
   ```

</details>

<details>
<summary><b>Using Command Line</b></summary>

**Linux/Mac (g++):**
```bash
g++ -o RadixTree Main.cpp RadixTree.cpp Node.cpp
./RadixTree
```

**Windows (g++/MinGW):**
```bash
g++ -o RadixTree.exe Main.cpp RadixTree.cpp Node.cpp
./RadixTree.exe
```

**Windows (MSVC - Developer Command Prompt):**
```cmd
cl /EHsc Main.cpp RadixTree.cpp Node.cpp /Fe:RadixTree.exe
RadixTree.exe
```

</details>

<details>
<summary><b>Troubleshooting</b></summary>

| Issue | Solution |
|-------|----------|
| `g++ not found` | Install MinGW (Windows) or GCC (Linux/Mac) |
| `cl not found` | Run from Visual Studio Developer Command Prompt |
| `strncpy_s not found` | Use MSVC compiler (this is Windows-specific) |
| Include errors | Make sure all .h and .cpp files are in the same folder |

</details>

## Usage Examples

### Basic Usage
```cpp
#include "RadixTree.h"
#include <iostream>

int main() {
    RadixTree tree;
    
    // Insert words
    tree.insert("test");
    tree.insert("team");
    tree.insert("tea");
    
    // Search for words
    if (tree.search("team")) {
        std::cout << "Found: team" << std::endl;
    }
    
    // Delete a word
    tree.deleteWord("tea");
    
    return 0;
}
```

### Running the Test Suite
The `Main.cpp` file contains a comprehensive test suite. When run, it outputs:
```
Test 1: Inserting words into Radix Tree
========================================
...

Test 4: Testing Search Operation
================================
  [PASS] Search 'test': FOUND
  [PASS] Search 'xyz': NOT FOUND
...

=========================================
TEST SUMMARY
=========================================
Total tests: 31
Passed: 31
Failed: 0

*** ALL TESTS PASSED! ***
```

## Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Insert    | O(k)           | O(k)             |
| Search    | O(k)           | O(1)             |
| Delete    | O(k)           | O(1)             |

*k = length of the key*

### Why Radix Tree?

Compared to a standard trie, a radix tree compresses chains of single-child nodes into one edge. This reduces memory usage by 40-60% while maintaining the same time complexity.

```
Standard Trie          Radix Tree
    (root)                (root)
      │                     │
      r                  "roman"
      │                  /     \
      o                "e"     "us"
      │                [✓]      [✓]
      m
      │
      a                Storing: "romane", "romanus"
      │                Nodes: 3 (vs 7 in trie)
      n
    /   \
  "e"   "us"
  [✓]    [✓]
```

## Team Structure

| Person | Responsibility | Status |
|--------|---------------|--------|
| Person 1 | Insertion Part A (Prefix Handling & Node Splitting) | ✅ Complete |
| Person 2 | Insertion Part B (Edge Management & Child Linking) | ✅ Complete |
| Person 3 | Search/Lookup Operation | ✅ Complete |
| Person 4 | Deletion Part A (Value Removal & Node Cleanup) | ✅ Complete |
| Person 5 | Deletion Part B (Node Merging & Path Compression) | ✅ Complete |

## Known Issues

| Issue | Severity | Status |
|-------|----------|--------|
| Destructor doesn't free memory | Medium | Planned |

## TODO

- [ ] Implement proper memory cleanup in destructor
- [ ] Add prefix search (find all words with prefix)
- [ ] Add performance benchmarks

---

<div align="center">

**Data Structures Course Project**

**Last Updated**: December 10, 2025

</div>

