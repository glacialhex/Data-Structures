# Malak + Nour - Insert, AddChild & AutoSuggest Functions

> **Authors:** Malak Mostafa Ali & Nour Mostafa Melegy  
> **Functions Covered:** `insert()`, `addchild()`, `autoSuggest()`

---

## Table of Contents
1. [addchild Function](#1-addchild-function)
2. [insert Function](#2-insert-function)
3. [autoSuggest Function](#3-autosuggest-function)

---

## 1. addchild Function

### Purpose
El `addchild` function beted5al child node gedeed taht parent mo3ayan. De el asas lel tree structure - men gherha mafee4 tree.

### Code with Line-by-Line Explanation

```cpp
void RadixTree::addchild(Node *parent, Node *childnode) {
```
- **Function signature:** Da method fel RadixTree class
- **Parameters:**
  - `parent`: El node ely hay-contain el child el gedeed
  - `childnode`: El node ely 3ayzeen nedafeha

```cpp
  child *newchild = new child(childnode->data[0], childnode);
```
- **What it does:** Bey-create `child` wrapper object gedeed
- **Parameters passed:**
  - `childnode->data[0]`: El first character bta3 el node data - used lel O(1) lookup
  - `childnode`: Pointer lel actual node
- **Leh caching el first char?** 3a4an mane7tagsh ne-dereference el node kol mara nedawer - performance optimization
- **Lw removed:** hanedter nedawer 3ala kol node w ne-access data[0] - O(n) per lookup badal O(1)

```cpp
  newchild->next = parent->children;
```
- **What it does:** El new child byeshawer 3ala el existing children list
- **Why prepend mesh append?** O(1) complexity - append kan hayd5alna n-traverse el whole list O(n)
- **Lw removed:** El new child hayb2a isolated - mafee4 link lel ba2y

```cpp
  parent->children = newchild;
```
- **What it does:** El parent byeshawer 3ala el new child as first child
- **Result:** El new child ba2a "head" of the children linked list
- **Lw removed:** El new child mesh linked lel parent 5ales - lost in memory

### Visual Example
```
Before: parent -> [child_A] -> [child_B] -> null
After:  parent -> [NEW] -> [child_A] -> [child_B] -> null
```

### Alternative Approaches
| Approach | Complexity | Why Not Used |
|----------|-----------|--------------|
| Append at end | O(n) | Slower - need to traverse |
| Array of children | O(1) amortized | Fixed size or reallocation overhead |
| Sorted linked list | O(n) insert | Not needed for our use case |

---

## 2. insert Function

### Purpose
El `insert` function beyt7andel kol el cases bta3t e insertion fel Radix Tree:
1. Empty tree
2. No matching prefix
3. Partial match (split needed)
4. Exact match
5. Continue traversal

### Code with Line-by-Line Explanation

```cpp
void RadixTree::insert(const char *word) {
```
- **Parameter:** `word` - el kelma ely 3ayzeen ned5alha (C-string)

#### Case 1: Empty Tree

```cpp
  if (empty()) {
    Node *newNode = new Node(word);
    newNode->ended = true;
    newNode->frequency = 1;
    newNode->timestamp = getCurrentTimestamp();
    addchild(myRoot, newNode);
    return;
  }
```
- **`empty()`**: Checks lw el tree fadya (myRoot->children == nullptr)
- **`new Node(word)`**: Bey-create node bel word kamla
- **`ended = true`**: El node de end of a valid word
- **`frequency = 1`**: First occurrence
- **`timestamp`**: For "recent searches" feature
- **`addchild(myRoot, newNode)`**: Da5alha taht el root
- **Lw removed:** Lw el tree fadya, mafee4 haga hay7sal - infinite loop

#### Setting Up Traversal

```cpp
  Node *currentNode = myRoot;
  const char *remaining = word;
```
- **`currentNode`**: Pointer byetna2el fel tree (starts at root)
- **`remaining`**: Pointer lel part ely lessa ma-insertena4
- **Leh pointer mesh copy?** Performance - copying strings is O(n)

#### Main Loop

```cpp
  while (true) {
```
- **Infinite loop:** Hay5rog b `return` lama yel2a el proper insertion point
- **Alternative:** Recursive approach - bas el iterative ahsan lel stack w readability

#### Finding Matching Child

```cpp
    child *ch = currentNode->children;
    Node *nextNode = nullptr;

    while (ch) {
      if (ch->firstChar == *remaining) {
        nextNode = ch->node;
        break;
      }
      ch = ch->next;
    }
```
- **`ch->firstChar == *remaining`**: Compare first char - O(1) comparison
- **`*remaining`**: El first char bta3 el remaining string
- **Why break?** La2ena el first character unique per child (Radix property)
- **Lw removed:** Msh hay-find matching children - kol kelma hatroo7 as new child

#### Case 2: No Matching Child

```cpp
    if (!nextNode) {
      Node *newNode = new Node(remaining);
      newNode->ended = true;
      newNode->frequency = 1;
      newNode->timestamp = getCurrentTimestamp();
      addchild(currentNode, newNode);
      return;
    }
```
- **When:** Mafee4 child byebda2 benafs el character
- **Action:** Create new node bel remaining string kolaha
- **Example:** Tree has "car", inserting "dog" → new sibling node "dog"

#### Checking Prefix Match

```cpp
    int prefix = SearchPrefix(remaining, nextNode);
    int nodeLen = strlen(nextNode->data);
    int remainingLen = strlen(remaining);
```
- **`prefix`**: Kam character matched
- **`nodeLen`**: Length bta3 el node data
- **`remainingLen`**: Length bta3 el remaining string

#### Case 3: Partial Match (Node Splitting)

```cpp
    if (prefix < nodeLen) {
```
- **When:** El match mesh complete lel node label
- **Example:** Node = "testing", inserting "test" → split to "test" + "ing"

```cpp
      Node *suffixNode = new Node(nextNode->data + prefix);
      suffixNode->ended = nextNode->ended;
      suffixNode->frequency = nextNode->frequency;
      suffixNode->timestamp = nextNode->timestamp;
      suffixNode->children = nextNode->children;
```
- **What:** Create node lel suffix (unmatched part)
- **`nextNode->data + prefix`**: Pointer arithmetic - starts from position `prefix`
- **Copying properties:** El suffix hayeb2a howa el original word end

```cpp
      char *newData = new char[prefix + 1];
      strncpy(newData, nextNode->data, prefix);
      newData[prefix] = '\0';
      delete[] nextNode->data;
      nextNode->data = newData;
```
- **Truncating:** El original node ba2a contains el prefix bass
- **Memory management:** Delete old data, allocate new smaller one
- **Lw mesh handled:** Memory leak

```cpp
      nextNode->ended = false;
      nextNode->frequency = 0;
      nextNode->children = nullptr;
      addchild(nextNode, suffixNode);
```
- **Reset:** El truncated node mesh end of word anymore
- **Link suffix:** El old data ba2a child

```cpp
      if (remaining[prefix] == '\0') {
        nextNode->ended = true;
        nextNode->frequency = 1;
        nextNode->timestamp = getCurrentTimestamp();
        return;
      }
```
- **When:** El new word ends exactly at split point
- **Example:** Node = "testing", inserting "test" → "test" becomes word end

```cpp
      Node *newWordNode = new Node(remaining + prefix);
      newWordNode->ended = true;
      newWordNode->frequency = 1;
      newWordNode->timestamp = getCurrentTimestamp();
      addchild(nextNode, newWordNode);
      return;
```
- **When:** El new word extends beyond split point
- **Example:** Tree has "test", inserting "team" → split to "te" with children "st" and "am"

#### Case 4: Exact Match

```cpp
    remaining += prefix;
    remainingLen = strlen(remaining);

    if (remainingLen == 0) {
      nextNode->ended = true;
      nextNode->frequency++;
      nextNode->timestamp = getCurrentTimestamp();
      return;
    }
```
- **When:** El word already exists or matches a node exactly
- **Action:** Mark as word end, increment frequency
- **Note:** `frequency++` means re-inserting the same word increases its count

#### Case 5: Continue Traversal

```cpp
    currentNode = nextNode;
  }
```
- **When:** El matched lel current node, bas fih remaining characters
- **Action:** Continue deeper into the tree

### Complexity Analysis
| Operation | Time | Space |
|-----------|------|-------|
| Best case (empty tree) | O(1) | O(m) where m = word length |
| Average case | O(m) | O(m) |
| Worst case (split) | O(m) | O(m) |

---

## 3. autoSuggest Function

### Purpose
El `autoSuggest` function beyt-print kol el words ely btebd2 b prefix mo3ayan lel console.

### Code with Line-by-Line Explanation

```cpp
void RadixTree::autoSuggest(const char *prefix) {
```
- **Parameter:** `prefix` - el prefix ely 3ayzeen suggestions lih

```cpp
  if (empty()) {
    cout << "Tree is empty. No suggestions available." << endl;
    return;
  }
```
- **Guard clause:** Lw el tree fadya, mafee4 7aga te-suggest
- **Why early return?** Clean code principle - fail fast

```cpp
  SuggestionList *suggestions = getSuggestions(prefix);
```
- **Delegation:** Bey-call `getSuggestions` ely bey-return linked list bel results
- **Why separate?** `getSuggestions` can be used by GUI, `autoSuggest` prints to console

```cpp
  if (suggestions->isEmpty()) {
    cout << "No suggestions found for the given prefix." << endl;
    delete suggestions;
    return;
  }
```
- **Check:** Lw mafee4 matches
- **Important:** `delete suggestions` - memory cleanup even on failure path

```cpp
  SuggestionNode *curr = suggestions->head;
  while (curr) {
    cout << curr->word << endl;
    curr = curr->next;
  }
  delete suggestions;
}
```
- **Traversal:** Loop over linked list
- **Print:** Each word on new line
- **Cleanup:** Delete the list when done

### Why Linked List Not Vector?
| Factor | Linked List | Vector |
|--------|-------------|--------|
| Project constraint | ✅ Required | ❌ Not allowed |
| Insert at end | O(1) with tail pointer | O(1) amortized |
| Memory | No reallocation | May reallocate |

### Alternative Design: Recursive Printing
Could directly print in `collectSuggestions` without intermediate list, but:
- Less flexible (can't sort, can't use in GUI)
- Couples collection with output
- Current design follows Single Responsibility Principle

---

## Summary

| Function | Purpose | Complexity |
|----------|---------|------------|
| `addchild` | Link child to parent | O(1) |
| `insert` | Add word to tree | O(m) |
| `autoSuggest` | Print suggestions | O(n) where n = results |

