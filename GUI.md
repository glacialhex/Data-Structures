# GUI - Qt GraphQL User Interface

> **Authors:** Ahead Team  
> **Files Covered:** `radix_tree_model.cpp`, `main_window.cpp`, `ghost_line_edit.cpp`

---

## Table of Contents
1. [Architecture Overview](#1-architecture-overview)
2. [RadixTreeModel Class](#2-radixtreemodel-class)
3. [Main Window](#3-main-window)
4. [GhostLineEdit Component](#4-ghostlineedit-component)
5. [Signal/Slot Communication](#5-signalslot-communication)

---

## 1. Architecture Overview

### Model-View-Controller Pattern

```
┌─────────────────┐      ┌─────────────────┐      ┌─────────────────┐
│   RadixTree     │◄────►│ RadixTreeModel  │◄────►│   MainWindow    │
│   (Core Logic)  │      │  (Qt Wrapper)   │      │   (View/UI)     │
└─────────────────┘      └─────────────────┘      └─────────────────┘
```

| Layer | Responsibility |
|-------|---------------|
| RadixTree | Core data structure logic |
| RadixTreeModel | Qt wrapper, signals, type conversion |
| MainWindow | User interface, event handling |

### Why Separation?
- **Reusability:** Core logic independent of UI framework
- **Testability:** Can test RadixTree without Qt
- **Maintainability:** Changes isolated to appropriate layer

---

## 2. RadixTreeModel Class

### Purpose
Wrap the C++ RadixTree in a Qt-compatible interface.

### Constructor

```cpp
RadixTreeModel::RadixTreeModel(QObject *parent)
    : QObject(parent), wordCount(0) {
  tree = new RadixTree();
}
```
- **`QObject(parent)`**: Qt parent-child memory management
- **`wordCount(0)`**: Initialize counter
- **`new RadixTree()`**: Create underlying data structure

### Destructor

```cpp
RadixTreeModel::~RadixTreeModel() { 
  delete tree; 
}
```
- **Cleanup:** Delete RadixTree when model destroyed

### Insert Method

```cpp
void RadixTreeModel::insert(const QString &word) {
  tree->insert(word.toStdString().c_str());
  wordCount++;
}
```
- **Type conversion:** QString → std::string → const char*
- **Why conversion?** RadixTree uses C-strings, Qt uses QString
- **Counter update:** Track word count for UI

### Search Method

```cpp
bool RadixTreeModel::search(const QString &word) {
  return tree->search(word.toStdString().c_str());
}
```
- **Direct delegation:** Just converts and calls

### getAutocompletions Method

```cpp
QVector<RadixTreeModel::Suggestion>
RadixTreeModel::getAutocompletions(const QString &prefix) {
  QVector<Suggestion> results;

  if (prefix.isEmpty() || tree->empty()) {
    emit suggestionsReady(results);
    return results;
  }
```
- **Guard clause:** Return empty if no input
- **`emit suggestionsReady`**: Signal even on empty (clears ghost text)

```cpp
  SuggestionList *treeSuggestions =
      tree->getSuggestions(prefix.toStdString().c_str());
```
- **Delegation:** Call core RadixTree function
- **Returns linked list:** Custom structure (no std::vector)

```cpp
  SuggestionNode *curr = treeSuggestions->head;
  while (curr) {
    results.append({QString::fromStdString(curr->word), 
                   curr->frequency, 
                   curr->timestamp});
    curr = curr->next;
  }
  delete treeSuggestions;
```
- **Conversion:** Linked list → QVector
- **Memory cleanup:** Delete linked list after conversion

```cpp
  std::sort(results.begin(), results.end());
  emit suggestionsReady(results);
  return results;
}
```
- **Sorting:** By frequency (defined in Suggestion struct)
- **Signal:** Notify UI of new suggestions

### File Operations

```cpp
void RadixTreeModel::loadFromFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  delete tree;
  tree = new RadixTree();
  wordCount = 0;
```
- **Reset:** Create fresh tree each load
- **Why delete old?** Clean slate, no duplicate words

```cpp
  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed().toLower();
    if (!line.isEmpty()) {
      insert(line);
    }
  }
```
- **Line-by-line:** Read and insert each word
- **`toLower()`**: Case normalization
- **`trimmed()`**: Remove whitespace

```cpp
  loadMetadata(filePath);
}
```
- **Metadata:** Restore frequencies from .meta file

---

## 3. Main Window

### Constructor Setup

```cpp
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Ahead - Autocomplete Engine");
  setMinimumSize(1000, 700);
```
- **Window properties:** Title and minimum size

```cpp
  model = new RadixTreeModel(this);
```
- **Create model:** Pass `this` as parent for Qt memory management

```cpp
  snowflakes = new SnowflakeWidget(this);
  snowflakes->start();
```
- **Visual effect:** Animated snowflakes background

```cpp
  setupUI();
  connectSignals();
}
```
- **Separation:** UI setup and signal connection in separate methods

### UI Setup Highlights

```cpp
searchBox = new GhostLineEdit();
searchBox->setFixedWidth(500);
```
- **Custom widget:** LineEdit with ghost text feature

```cpp
suggestionsList = new QListWidget();
suggestionsList->setStyleSheet(
    QString("QListWidget { background-color: %1; ... }")
        .arg(Colors::BG_MEDIUM, Colors::BORDER, ...));
```
- **Styling:** Dark theme with custom colors
- **`QString::arg()`**: String interpolation for colors

### Signal Connections

```cpp
void MainWindow::connectSignals() {
  connect(searchBox, &GhostLineEdit::textChanged, this,
          &MainWindow::onTextChanged);
```
- **Real-time:** Update suggestions as user types

```cpp
  connect(model, &RadixTreeModel::suggestionsReady,
          [this](const QVector<RadixTreeModel::Suggestion> &suggestions) {
            suggestionsList->clear();
            // ... populate list
          });
}
```
- **Lambda slot:** Inline handler for suggestions
- **Updates:**
  1. Clear old suggestions
  2. Sort by current mode (frequency/recent)
  3. Populate list widget
  4. Update ghost text

### Text Changed Handler

```cpp
void MainWindow::onTextChanged(const QString &text) {
  if (text.isEmpty()) {
    showRecentSearches();
    searchBox->clearGhost();
    return;
  }
  model->getAutocompletions(text);
}
```
- **Empty input:** Show recent searches instead
- **Non-empty:** Trigger autocomplete

### Search Enter Handler

```cpp
void MainWindow::onSearchEnterPressed() {
  QString word = searchBox->text().trimmed().toLower();
  if (!word.isEmpty()) {
    if (model->search(word)) {
      model->incrementFrequency(word);
      // Update recent searches
      // Show success message
    } else {
      // Show error - word not found
    }
  }
}
```
- **Validation:** Word must exist in tree
- **Frequency update:** Popular words rise to top
- **Visual feedback:** Green success or red error

---

## 4. GhostLineEdit Component

### Purpose
LineEdit that shows ghost text (autocomplete suggestion) as user types.

### Key Features

```cpp
class GhostLineEdit : public QLineEdit {
  Q_OBJECT
public:
  void setGhostText(const QString &suggestion);
  void clearGhost();
```
- **`setGhostText`**: Show suggestion in gray
- **`clearGhost`**: Remove ghost text

### Paint Event Override

```cpp
void GhostLineEdit::paintEvent(QPaintEvent *event) {
  QLineEdit::paintEvent(event);  // Normal painting
  
  if (!m_ghostText.isEmpty() && text().length() < m_ghostText.length()) {
    QPainter painter(this);
    painter.setPen(QColor("#6B7280"));  // Gray color
    // Draw ghost text after cursor
  }
}
```
- **Custom painting:** Draw ghost text after normal text
- **Conditional:** Only when ghost text available

### Key Press Handling

```cpp
void GhostLineEdit::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Space) {
    if (!m_fullSuggestion.isEmpty()) {
      setText(m_fullSuggestion);
      emit suggestionAccepted(m_fullSuggestion);
      clearGhost();
    }
  }
  QLineEdit::keyPressEvent(event);
}
```
- **Accept shortcuts:** Right arrow or Space accepts suggestion
- **Signal:** Notify that suggestion was accepted

---

## 5. Signal/Slot Communication

### Communication Flow

```
User Types → textChanged signal → onTextChanged slot
                                        ↓
                        model->getAutocompletions()
                                        ↓
                          suggestionsReady signal
                                        ↓
                      Lambda updates suggestionsList
                                        ↓
                      Ghost text updated on searchBox
```

### Key Signals

| Signal | Emitter | Purpose |
|--------|---------|---------|
| `textChanged` | GhostLineEdit | User typed something |
| `suggestionsReady` | RadixTreeModel | New suggestions available |
| `suggestionAccepted` | GhostLineEdit | User accepted ghost text |
| `addWordRequested` | SideMenu | User wants to add word |

### Why Signals/Slots?
| Factor | Direct Calls | Signals/Slots |
|--------|--------------|---------------|
| Coupling | Tight | Loose |
| Thread safety | Manual | Automatic |
| Qt convention | ❌ | ✅ |

---

## Complexity Analysis

| Operation | UI Response Time |
|-----------|-----------------|
| Character typed | < 16ms (60 FPS) |
| Suggestions load | O(n) where n = results |
| File load (370k words) | ~500ms |

---

## Summary

| Component | Purpose |
|-----------|---------|
| RadixTreeModel | Qt wrapper for RadixTree |
| MainWindow | Main application window |
| GhostLineEdit | Autocomplete input box |
| SideMenu | Word management panel |
| SnowflakeWidget | Visual effect |

### Key Design Decisions
1. **Model-View separation:** Core logic reusable
2. **Custom linked list:** No std::vector (project constraint)
3. **Real-time updates:** Autocomplete on every keystroke
4. **Dark theme:** Modern aesthetic with custom colors

