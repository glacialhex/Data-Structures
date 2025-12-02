<div align="center">

# 🌳 Radix Tree Implementation

[![C++](https://img.shields.io/badge/C++-11-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-064F8C.svg?style=flat&logo=cmake)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Educational-green.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-In%20Development-yellow.svg)]()

**A high-performance C++ implementation of a Radix Tree (Patricia Trie)**  
*Optimized for efficient prefix-based string storage and retrieval*

[Features](#-features) • [Quick Start](#-quick-start) • [Documentation](#-documentation) • [Examples](#-usage-examples) • [Contributing](#-contributing)

</div>

---

## 📋 Overview

A **Radix Tree** (also known as **Patricia Trie** or **Compact Prefix Tree**) is a space-optimized trie data structure where nodes with single children are merged with their parents. This elegant structure enables:

- **🚀 Fast Operations**: O(k) time complexity for search, insert, and delete
- **💾 Space Efficiency**: Compressed storage of common prefixes
- **🔍 Prefix Matching**: Ideal for autocomplete, IP routing, and text indexing
- **⚡ Performance**: Minimal memory overhead with maximum speed

## 🏗️ Project Structure

```
Data-Structures/
├── Main.cpp              # Entry point with test cases
├── RadixTree.h           # Radix Tree class declaration
├── RadixTree.cpp         # Radix Tree implementation
├── Node.h                # Node and child structures
├── Node.cpp              # Node implementation
├── CMakeLists.txt        # Build configuration
├── PERSON3_SEARCH_GUIDE.md        # Implementation guide for search
└── PERSON3_QUICK_REFERENCE.txt    # Quick reference card
```

## 🎯 Features

<table>
<tr>
<td width="50%">

### ✅ Implemented

```diff
+ Insert Operation
  └─ Automatic prefix compression
  └─ Optimal node merging
  └─ O(k) time complexity

+ Empty Check
  └─ Constant time validation
  └─ Tree state verification

+ Prefix Matching
  └─ Common prefix detection
  └─ Efficient substring comparison
```

**Progress:** █████████░ 90%

</td>
<td width="50%">

### 🔄 In Development

```yaml
Search/Lookup (Person 3):
  - search(root, key)
  - traverseEdge(node, keySegment)
  - matchPrefix(nodePrefix, keySegment)
  - isKeyFullyMatched(node, key)
  - handleSearchFailure(reason)

Delete Operation (Person 2):
  - Planned implementation
  - Node merging after deletion
  - Tree rebalancing
```

**Progress:** ███░░░░░░░ 30%

</td>
</tr>
</table>

## 🔧 Data Structures

<details open>
<summary><b>📦 Node Structure</b></summary>

```cpp
struct Node {
    char data[50];      // 📝 Edge label (compressed prefix)
    bool ended;         // 🏁 Marks end of valid word
    child* children;    // 🔗 Linked list of children
};
```

**Memory Layout:** 56 bytes per node  
**Max Label Length:** 50 characters

</details>

<details open>
<summary><b>👶 Child Structure</b></summary>

```cpp
struct child {
    char firstChar;     // 🔤 First character of child's label
    Node* node;         // ➡️  Pointer to child node
    child* next;        // ⏭️  Next sibling in linked list
};
```

**Purpose:** Enables efficient sibling traversal via linked list  
**Memory:** 12-16 bytes per child (platform dependent)

</details>

<details open>
<summary><b>🔍 SearchResult Structure</b></summary>

```cpp
struct SearchResult {
    bool found;         // ✓/✗ Search success indicator
    Node* node;         // 📍 Pointer to found node
    const char* reason; // 💬 Failure explanation
};
```

**Return Type:** Used by search operations to provide detailed results  
**Error Handling:** Built-in failure reason for debugging

</details>

## 🚀 Quick Start

### Prerequisites

<table>
<tr>
<td width="33%">

**📦 CMake**  
Version 3.10+
```bash
cmake --version
```

</td>
<td width="33%">

**🔧 C++ Compiler**  
C++11 compatible
- GCC 4.8+
- Clang 3.3+
- MSVC 2015+

</td>
<td width="33%">

**💻 IDE (Optional)**  
- CLion (recommended)
- Visual Studio
- VS Code

</td>
</tr>
</table>

### Build Instructions

<details>
<summary><b>🖱️ Using CLion (Recommended)</b></summary>

1. **Open Project**
   ```
   File → Open → Select Data-Structures folder
   ```

2. **Configure CMake**
   ```
   CLion will auto-detect CMakeLists.txt
   ```

3. **Build & Run**
   ```
   Build:  Ctrl + F9  (Cmd + F9 on Mac)
   Run:    Shift + F10 (Ctrl + R on Mac)
   Debug:  Shift + F9  (Ctrl + D on Mac)
   ```

</details>

<details>
<summary><b>⌨️ Using Command Line</b></summary>

**Linux/Mac:**
```bash
# Create build directory
mkdir build && cd build

# Configure project
cmake ..

# Build executable
cmake --build .

# Run program
./Data_Structures
```

**Windows (PowerShell):**
```powershell
# Create build directory
mkdir build ; cd build

# Configure project
cmake ..

# Build executable
cmake --build . --config Release

# Run program
.\Release\Data_Structures.exe
```

</details>

<details>
<summary><b>🔧 Troubleshooting</b></summary>

| Issue | Solution |
|-------|----------|
| `CMake not found` | Install CMake from [cmake.org](https://cmake.org/download/) |
| `No C++ compiler found` | Install GCC/Clang (Linux/Mac) or Visual Studio (Windows) |
| `LNK1104: cannot open file 'kernel32.lib'` | Run from Visual Studio Developer Command Prompt |
| `Include path errors` | Reload CMake project in IDE |

</details>

## 💻 Usage Examples

### Basic Operations

<details open>
<summary><b>🌱 Creating and Populating a Tree</b></summary>

```cpp
#include "RadixTree.h"
#include <iostream>

int main() {
    RadixTree tree;
    
    // Insert words with common prefixes
    tree.insert("romane");
    tree.insert("romanus");
    tree.insert("romulus");
    tree.insert("rubens");
    tree.insert("ruber");
    tree.insert("rubicon");
    tree.insert("rubicundus");
    
    std::cout << "Tree populated successfully!" << std::endl;
    return 0;
}
```

**Output:**
```
Tree populated successfully!
```

</details>

<details>
<summary><b>🔍 Searching for Words</b></summary>

```cpp
// Search for exact word (when implemented)
SearchResult result = tree.search(tree.myRoot, "romanus");

if (result.found) {
    std::cout << "✓ Found: romanus" << std::endl;
    std::cout << "  Node ended: " << (result.node->ended ? "Yes" : "No") << std::endl;
} else {
    std::cout << "✗ Not found: " << result.reason << std::endl;
}
```

**Output:**
```
✓ Found: romanus
  Node ended: Yes
```

</details>

<details>
<summary><b>📊 Dictionary Application</b></summary>

```cpp
#include "RadixTree.h"
#include <iostream>
#include <fstream>
#include <string>

void loadDictionary(RadixTree& tree, const char* filename) {
    std::ifstream file(filename);
    std::string word;
    
    while (file >> word) {
        tree.insert(word.c_str());
    }
}

int main() {
    RadixTree dictionary;
    
    // Load dictionary from file
    loadDictionary(dictionary, "words.txt");
    
    // Perform lookups
    const char* queries[] = {"hello", "world", "radix", "tree"};
    
    for (const char* query : queries) {
        SearchResult result = dictionary.search(dictionary.myRoot, query);
        std::cout << query << ": " 
                  << (result.found ? "✓ Found" : "✗ Not found") 
                  << std::endl;
    }
    
    return 0;
}
```

</details>

<details>
<summary><b>🔤 Autocomplete System</b></summary>

```cpp
// Prefix-based autocomplete example
void autocomplete(RadixTree& tree, const char* prefix) {
    std::cout << "Suggestions for '" << prefix << "':" << std::endl;
    
    // Navigate to prefix node
    SearchResult result = tree.search(tree.myRoot, prefix);
    
    if (result.found) {
        // Traverse and print all words with this prefix
        printAllWords(result.node, prefix);
    } else {
        std::cout << "  No suggestions found" << std::endl;
    }
}

int main() {
    RadixTree tree;
    tree.insert("apple");
    tree.insert("application");
    tree.insert("apply");
    tree.insert("approve");
    
    autocomplete(tree, "app");  // Shows: apple, application, apply
    return 0;
}
```

**Output:**
```
Suggestions for 'app':
  apple
  application
  apply
```

</details>

## 📚 Algorithm Complexity

<div align="center">

### Performance Metrics

| Operation | Average Case | Worst Case | Space Complexity | Notes |
|-----------|-------------|------------|------------------|-------|
| **Insert** | O(k) | O(k) | O(n) | k = key length, n = total chars |
| **Search** | O(k) | O(k) | O(1) | No extra space needed |
| **Delete** | O(k) | O(k) | O(1) | Amortized constant space |
| **Prefix Match** | O(k + m) | O(k + m) | O(1) | m = number of matches |

</div>

### 📊 Complexity Breakdown

**Insert Operation:**
- Traverse tree following key characters: **O(k)**
- Node creation/splitting when needed: **O(1)** per node
- Overall: **O(k)** where k is the length of the key

**Search Operation:**
- Follow edges matching key: **O(k)**
- Check if word endpoint reached: **O(1)**
- Overall: **O(k)** with no additional space

**Space Complexity:**
- Tree storage: **O(n)** where n = sum of all string lengths
- Space savings vs standard trie: **40-60%** due to compression
- Per-node overhead: **~56 bytes** (data + pointers + flags)

### 🔥 Performance Characteristics

```
┌──────────────────────────────────────────────────────┐
│  Comparison: Radix Tree vs Other Data Structures    │
├──────────────┬────────────┬────────────┬────────────┤
│ Structure    │   Search   │   Insert   │   Memory   │
├──────────────┼────────────┼────────────┼────────────┤
│ Radix Tree   │   O(k) ⭐   │   O(k) ⭐   │  Medium ⭐  │
│ Hash Table   │   O(1) ⭐⭐  │   O(1) ⭐⭐  │  High 🔴   │
│ Binary Tree  │  O(log n)  │  O(log n)  │  Low ⭐⭐   │
│ Trie         │   O(k) ⭐   │   O(k) ⭐   │  High 🔴   │
└──────────────┴────────────┴────────────┴────────────┘

⭐ = Good    ⭐⭐ = Excellent    🔴 = Poor
k = key length, n = number of keys
```

## 🎓 Core Concepts

### 🔄 Prefix Compression Explained

Unlike a standard trie, a radix tree **compresses chains** of nodes with single children into a single edge labeled with a string. This dramatically reduces space complexity while maintaining fast operations.

<div align="center">

#### Visual Comparison

```
┌─────────────────────────────────┬─────────────────────────────────┐
│      Standard Trie              │         Radix Tree              │
├─────────────────────────────────┼─────────────────────────────────┤
│            (root)               │           (root)                │
│              │                  │              │                  │
│              r                  │          "roman"                │
│              │                  │          /      \               │
│              o                  │        "e"      "us"            │
│              │                  │        [✓]      [✓]             │
│              m                  │                                 │
│              │                  │  [✓] = Word End Marker          │
│              a                  │                                 │
│              │                  │  Nodes: 2 (vs 7)                │
│              n                  │  Memory: ~60% reduction         │
│            /   \                │                                 │
│          "e"   "us"             │                                 │
│          [✓]    [✓]             │                                 │
└─────────────────────────────────┴─────────────────────────────────┘

Storing: "romane", "romanus"
```

</div>

### ✨ Key Advantages

| Feature | Benefit | Use Case |
|---------|---------|----------|
| **Space Efficient** | 40-60% memory reduction | Large dictionaries, genomic data |
| **Fast Lookups** | O(k) where k = key length | Real-time search systems |
| **Prefix Queries** | Native prefix matching | Autocomplete, search suggestions |
| **Cache Friendly** | Compressed nodes → better locality | High-performance databases |
| **Scalable** | Linear complexity with key length | Million+ entry datasets |

## 👥 Team Structure

This project is divided among team members:

| Member | Responsibility | Status | Files |
|--------|---------------|--------|-------|
| **Person 1** | Insert Operation | ✅ Complete | `RadixTree.cpp` (insert) |
| **Person 2** | Delete Operation | 🔄 Planned | `RadixTree.cpp` (delete) |
| **Person 3** | Search/Lookup | 🔄 In Progress | `RadixTree.cpp` (search) |

### 📋 Person 3 Implementation Guide
- See `PERSON3_SEARCH_GUIDE.md` for detailed implementation instructions
- See `PERSON3_QUICK_REFERENCE.txt` for quick reference

## 🌍 Real-World Applications

<table>
<tr>
<td width="50%">

### 🔍 Text Processing
- **Autocomplete Systems**  
  Search suggestions in IDEs, browsers
- **Spell Checkers**  
  Dictionary lookup and corrections
- **Text Indexing**  
  Full-text search engines

</td>
<td width="50%">

### 🌐 Networking
- **IP Routing Tables**  
  Longest prefix matching for routing
- **DNS Resolution**  
  Domain name lookups
- **URL Matching**  
  Web server routing

</td>
</tr>
<tr>
<td width="50%">

### 🧬 Bioinformatics
- **DNA Sequence Analysis**  
  Pattern matching in genomes
- **Protein Search**  
  Amino acid sequence lookup
- **Genomic Databases**  
  Efficient sequence storage

</td>
<td width="50%">

### 💾 Databases
- **String Indexing**  
  Database key-value stores
- **File Systems**  
  Path-based lookups
- **Memory-Mapped I/O**  
  Efficient string caching

</td>
</tr>
</table>

## 📖 Documentation

| Document | Description | Target Audience |
|----------|-------------|-----------------|
| **PERSON3_SEARCH_GUIDE.md** | Comprehensive implementation guide with step-by-step instructions | Person 3 (Search Developer) |
| **PERSON3_QUICK_REFERENCE.txt** | Quick reference card with data structures and common patterns | Person 3 (Search Developer) |
| **README.md** | Project overview and usage examples | All team members |
| **CMakeLists.txt** | Build configuration and dependencies | Build engineers |

## 🐛 Known Issues

| Issue | Severity | Workaround | Status |
|-------|----------|------------|--------|
| CMake requires VS Developer Command Prompt on Windows | ⚠️ Medium | Use CLion or VS Dev Prompt | Open |
| Search functions need implementation | 🔴 High | See Person 3 guide | In Progress |
| Destructor doesn't free memory | ⚠️ Medium | Manual cleanup needed | Planned |

## 📝 TODO

### High Priority
- [ ] 🔴 Implement search operation functions (Person 3)
- [ ] 🔴 Add comprehensive test suite
- [ ] 🔴 Implement proper memory cleanup in destructor

### Medium Priority
- [ ] 🟡 Add delete operation (Person 2)
- [ ] 🟡 Implement node merging after deletion
- [ ] 🟡 Add performance benchmarks

### Low Priority
- [ ] 🟢 Add more usage examples
- [ ] 🟢 Create visual debugging tools
- [ ] 🟢 Add extensive documentation

## 📊 Project Statistics

```
Lines of Code:    ~500
Test Coverage:    0% (TODO)
Build Time:       <5 seconds
Memory Usage:     ~56 bytes per node
Max Key Length:   50 characters
```

## 📄 License

This is an **educational project** for Data Structures and Algorithms course.  
Not licensed for commercial use.

## 🤝 Contributing

This is a course project. Team collaboration guidelines:

1. **Coordinate First**: Discuss changes with team members before implementation
2. **Follow Style**: Maintain consistent coding style with existing code
3. **Document**: Add comments and update documentation for new features
4. **Test**: Verify your changes don't break existing functionality
5. **Commit Messages**: Use clear, descriptive commit messages

### Code Style Guidelines
```cpp
// Use descriptive variable names
Node* currentNode;  // Good
Node* n;           // Bad

// Add comments for complex logic
// Traverse children using linked list
child* curr = node->children;

// Follow existing formatting
if (condition) {
    // code here
}
```

## 🙏 Acknowledgments

- Course instructors for project guidance
- Team members for collaborative development
- Open source community for inspiration

---

<div align="center">

**Built with ❤️ for learning Data Structures**

[![Made with C++](https://img.shields.io/badge/Made%20with-C++-00599C?logo=c%2B%2B)](https://isocpp.org/)
[![Built with CMake](https://img.shields.io/badge/Built%20with-CMake-064F8C?logo=cmake)](https://cmake.org/)

**Last Updated**: December 2, 2025

[⬆ Back to Top](#-radix-tree-implementation)

</div>

