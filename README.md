# 🚀 Ahead - Lightning-Fast Autocomplete

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Qt6](https://img.shields.io/badge/Qt-6.x-41CD52.svg)](https://www.qt.io/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![macOS](https://img.shields.io/badge/macOS-000000?style=flat&logo=apple&logoColor=white)]()
[![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white)]()

A high-performance autocomplete engine powered by an optimized **Radix Tree** (Patricia Trie) with a beautiful Qt6 GUI. Features ghost-text suggestions, frequency-based ranking, and real-time dictionary management.

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| ⚡ **Instant Autocomplete** | Sub-millisecond prefix matching |
| 👻 **Ghost Text** | Inline suggestions as you type (Tab/→ to accept) |
| 📊 **Frequency Ranking** | Most-used words appear first |
| 💾 **Persistence** | Word frequencies saved across sessions |
| 🎨 **Modern UI** | Dark theme with glassmorphism effects |
| ❄️ **Snowflake Effect** | Festive animated background |

---

## 🛠️ Building from Source

### Prerequisites

| Platform | Requirements |
|----------|--------------|
| **macOS** | Xcode Command Line Tools, Homebrew, Qt6 |
| **Windows** | Visual Studio 2019+, CMake, Qt6 |

### <img src="https://upload.wikimedia.org/wikipedia/commons/3/31/Apple_logo_white.svg" width="20" height="20" /> macOS

```bash
# 1. Install Qt6 via Homebrew
brew install qt@6

# 2. Clone and build
git clone https://github.com/glacialhex/Data-Structures.git
cd Data-Structures/qt_gui

# 3. Create build directory
mkdir build && cd build

# 4. Configure with CMake
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)

# 5. Build
cmake --build .

# 6. Run
open Ahead.app
```

### <img src="https://upload.wikimedia.org/wikipedia/commons/8/87/Windows_logo_-_2021.svg" width="20" height="20" /> Windows

```powershell
# 1. Install Qt6 from https://www.qt.io/download
# 2. Clone repository
git clone https://github.com/glacialhex/Data-Structures.git
cd Data-Structures\qt_gui

# 3. Create build directory
mkdir build
cd build

# 4. Configure (adjust Qt path as needed)
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2019_64"

# 5. Build
cmake --build . --config Release

# 6. Run
.\Release\Ahead.exe
```



---

## 📖 Usage

### Loading a Dictionary

1. Click **📂 Load Dictionary** in the side panel
2. Select a `.txt` file (one word per line)
3. Words are indexed immediately

**Sample dictionaries included:**
- `dictionary.txt` - 5,000+ English words
- `sample_tech_words.txt` - Technology terms
- `sample_university_words.txt` - Academic vocabulary

### Autocomplete

1. Start typing in the search box
2. Ghost text shows the best match
3. Press **Tab** or **→** to accept
4. Press **Enter** to confirm and boost frequency

### Managing Words

| Action | How |
|--------|-----|
| Add word | Type in side panel, click "Add Word" |
| Delete word | Select in list, click "Remove Selected" |
| Save dictionary | Click **💾 Save Dictionary** |

---

## 📁 Project Structure

```
Data-Structures/
├── RadixTree.cpp/h    # Core trie implementation
├── Node.cpp/h         # Node structure
├── Main.cpp           # CLI test harness
├── qt_gui/            # Qt6 GUI application
│   ├── main_window.*  # Main application window
│   ├── side_menu.*    # Collapsible side panel
│   ├── ghost_line_edit.*  # Autocomplete input
│   ├── snowflake_widget.* # Visual effects
│   ├── radix_tree_model.* # Qt model wrapper
│   ├── theme.h        # Color palette
│   └── assets/        # Logos and images
├── dictionary.txt     # Sample word list
└── README.md
```

---

## 🎓 Credits

**Ahead** was developed as a Data Structures project by students at:

| University | |
|------------|--|
| 🏛️ **Ain Shams University** | Faculty of Computer and Information Sciences |
| 🏛️ **University of East London** | School of Architecture, Computing and Engineering |

**Team Members:**
- Jana Tarek Nayel (24P0327)
- Lujain Mohamed Helmi (24P0284)
- Malak Mostafa Ali (24P0239)
- Nour Mostafa Melegy (24P0176)
- Yousef Mohamed Helmy (20P7954)

---

## 📄 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2025 Ahead Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

<p align="center">
  <sub>Built with ❤️ and C++17</sub>
</p>
