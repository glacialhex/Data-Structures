# ⚡ Ahead - Advanced Autocomplete Engine
### Efficient Radix Tree Implementation for Real-Time Search

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B" alt="C++17">
  <img src="https://img.shields.io/badge/Qt-6.7-41CD52?style=for-the-badge&logo=qt" alt="Qt 6">
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge" alt="License">
</p>

---

## 📖 Project Overview
**Ahead** is a high-performance, real-time autocomplete application built on a custom C++ **Radix Tree** (Compressed Trie). It demonstrates **O(k)** search complexity, making it orders of magnitude faster than standard `std::map` or `BST` approaches for string operations. The project features a modern, responsive GUI built with **Qt 6**.

### 🚀 Key Features

*   **Lightning Fast Search**: Core engine uses a Radix Tree for **O(k)** lookups (where *k* is word length).
*   **Real-Time Suggestions**: Instant results update as you type.
*   **Smart Ranking**:
    *   **Most Used**: Sorts suggestions by frequency of use.
    *   **Recent**: Tracks session-based search history.
*   **Ghost Text**: Google-style inline text completion.
*   **Data Persistence**:
    *   Loads standard Dictionary text files.
    *   Saves learned metadata (frequencies) to `.meta` files.
*   **Cross-Platform**: Runs natively on **macOS** and **Windows**.

---

## 🛠️ Technical Architecture

### The Radix Tree
Unlike a standard Trie which stores one character per node, our Radix Tree **compresses** linear paths.
*   **Space Efficiency**: Shared prefixes (e.g., "internet", "internal") are stored only once.
*   **Search Efficiency**: Traverses fewer nodes. finding "Apple" takes just 1 step if the node `Root -> Apple` exists.

### Node Structure
```cpp
struct Node {
    char *data;          // Compressed string fragment
    bool ended;          // Marks if this path ends a valid word
    int frequency;       // Usage count for ranking
    long long timestamp; // Last access time for "Recent" sort
    child *children;     // Linked list of child edges
};
```

---

## 💻 Build Instructions

### Prerequisites
*   **C++ Compiler** (Clang, GCC, or MSVC) supporting C++17.
*   **CMake** (3.16+).
*   **Qt 6** (Core, Gui, Widgets modules).

### macOS
```bash
# 1. Install Dependencies
brew install cmake qt@6

# 2. Clone & Build
git clone https://github.com/glacialhex/Data-Structures.git
cd Data-Structures/qt_gui
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
make

# 3. Run
open Ahead.app
```

### Windows
```powershell
# 1. Install Qt6 via Online Installer
# 2. Clone & Build
git clone https://github.com/glacialhex/Data-Structures.git
cd Data-Structures/qt_gui
mkdir build; cd build
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.7.0\msvc2019_64"
cmake --build . --config Release
```

---

## 👥 Team & Credits

**Faculty of Engineering - Ain Shams University**  
*Computer Engineering Department - 2nd Year Data Structures Project*

**Developed By:**
*   **Yousef Kotp** (Software Engineering & GUI)
*   **Youssef El-Kholy** (Radix Tree Core)
*   **Seif El-Din** (Algorithms & Analysis)
*   **Adham Mohamed** (Testing & QA)

© 2025 Ahead Team. All Rights Reserved.
