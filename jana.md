# Jana - Autocomplete (getSuggestions) Function

> **Author:** Jana Tarek Nayel  
> **Functions Covered:** `getSuggestions()`, `collectSuggestions()`

---

## Table of Contents
1. [Overview](#1-overview)
2. [collectSuggestions Function](#2-collectsuggestions-function)
3. [getSuggestions Function](#3-getsuggestions-function)
4. [SuggestionList Data Structure](#4-suggestionlist-data-structure)

---

## 1. Overview

El autocomplete system byet-composed of two main functions:
1. **`getSuggestions`**: Traverses the tree to find the prefix node, then collects all words
2. **`collectSuggestions`**: Recursively collects all words under a given node

### Why Two Functions?
| Function | Responsibility |
|----------|---------------|
| `getSuggestions` | Navigate to prefix location |
| `collectSuggestions` | Gather all words from that point |

This follows the **Single Responsibility Principle** - each function does one thing well.

---

## 2. collectSuggestions Function

### Purpose
Recursively traverse all descendants of a node and collect complete words into a linked list.

### Code with Line-by-Line Explanation

```cpp
void RadixTree::collectSuggestions(Node *node, const std::string &currentWord,
                                   SuggestionList *results) {
```
- **Parameters:**
  - `node`: El node el 7alya ely hanebda2 menha el collection
  - `currentWord`: El word ely etbana le7d delwa2ty (passed by reference for efficiency)
  - `results`: Linked list byetgama3 fiha el suggestions

```cpp
  if (!node)
    return;
```
- **Guard clause:** Lw el node null, mafeesh 7aga te-collect
- **When null?** Lw 7ad call el function b invalid pointer
- **Lw removed:** Segmentation fault lama yhawel ye-access null->ended

```cpp
  if (node->ended) {
    results->append(currentWord, node->frequency, node->timestamp);
  }
```
- **Check:** Hel el node de end of a valid word?
- **`ended`**: Boolean flag byefro2 ben "car" as word vs "car" as prefix of "carpet"
- **`append`**: Byed5al el word fel linked list
- **Parameters saved:**
  - `currentWord`: El complete word
  - `frequency`: Kam mara el word et-used
  - `timestamp`: Emta el word et-access lel recent searches feature
- **Lw removed:** Words marked as `ended` won't be collected

```cpp
  child *ch = node->children;
```
- **Get children:** Pointer lel first child
- **Note:** Children are stored as linked list, not array

```cpp
  while (ch) {
```
- **Loop:** Iterate over all children
- **Why while not for?** Linked list doesn't have indices

```cpp
    std::string nextWord = currentWord + ch->node->data;
```
- **Build word:** Concatenate current path with child's label
- **Example:** currentWord = "te", child->data = "st" → nextWord = "test"
- **Why new string?** Need separate copy for each branch (recursion)

```cpp
    collectSuggestions(ch->node, nextWord, results);
```
- **Recursion:** Call same function on child
- **DFS:** Depth-First Search pattern
- **Results accumulate:** Same `results` list passed down

```cpp
    ch = ch->next;
  }
}
```
- **Next sibling:** Move to next child in linked list
- **End:** When ch becomes null, all children processed

### Visual Example

```
Tree structure:
        [root]
          |
        "te"
       /    \
    "st"    "am"
     |
   "ing"

Calling collectSuggestions on "te" node with prefix "te":
1. "te" is not ended → don't add
2. Visit child "st" → word = "test"
   - "test" is ended → ADD "test"
   - Visit child "ing" → word = "testing"
     - "testing" is ended → ADD "testing"
3. Visit child "am" → word = "team"
   - "team" is ended → ADD "team"

Final results: ["test", "testing", "team"]
```

### Complexity Analysis
| Metric | Value | Explanation |
|--------|-------|-------------|
| Time | O(n) | Must visit every descendant node |
| Space | O(h) | Call stack depth = tree height |
| Result space | O(k) | k = number of words found |

---

## 3. getSuggestions Function

### Purpose
Navigate to the node matching the prefix, then collect all words from there.

### Code with Line-by-Line Explanation

```cpp
SuggestionList *RadixTree::getSuggestions(const char *prefix) {
```
- **Return type:** Pointer to linked list (caller must delete)
- **Parameter:** The prefix to search for

```cpp
  SuggestionList *results = new SuggestionList();
```
- **Create result container:** Empty linked list
- **Why heap allocation?** Needs to survive function return
- **Important:** Caller responsible for `delete`

```cpp
  if (empty() || prefix == nullptr || prefix[0] == '\0') {
    return results;
  }
```
- **Guard clauses:**
  - `empty()`: Tree has no words
  - `prefix == nullptr`: Invalid pointer
  - `prefix[0] == '\0'`: Empty string
- **Return empty list:** Consistent return type even on failure

```cpp
  Node *current = myRoot;
  std::string builtString = "";
  int prefixLength = strlen(prefix);
  int index = 0;
```
- **`current`**: Navigation pointer, starts at root
- **`builtString`**: Accumulates the actual path taken (may differ from prefix due to compression)
- **`prefixLength`**: How many chars need to match
- **`index`**: Current position in prefix string

```cpp
  while (index < prefixLength) {
```
- **Main loop:** Continue until entire prefix is matched
- **Exit conditions:** Either match complete OR no match found

```cpp
    child *ch = current->children;
    Node *nextNode = nullptr;
    int matchedLen = 0;
```
- **Setup for child search:**
  - `ch`: Iterator for children
  - `nextNode`: Will hold matching child
  - `matchedLen`: How many chars matched

```cpp
    while (ch) {
      if (ch->firstChar == prefix[index]) {
        nextNode = ch->node;
        matchedLen = matchPrefix(nextNode->data, prefix + index);
        if (matchedLen > 0) {
          break;
        }
      }
      ch = ch->next;
    }
```
- **Find matching child:**
  1. Compare first character (O(1) check - el `ch->firstChar` cached)
  2. If match, calculate full prefix match length
  3. Break if found (only one child can start with each char)
- **Lw removed:** Will never find children, always return empty

```cpp
    if (nextNode == nullptr || matchedLen == 0) {
      return results;
    }
```
- **No match found:** Return empty list
- **Examples:**
  - Prefix "xyz" but no child starts with "x"
  - Child "test" but searching for "tea" (matchedLen = 2, but...)

```cpp
    int nodeLen = strlen(nextNode->data);
    if (index + matchedLen < prefixLength && matchedLen < nodeLen) {
      return results;
    }
```
- **Critical check:** Partial mismatch mid-edge
- **Condition explained:**
  - `index + matchedLen < prefixLength`: Still more prefix to match
  - `matchedLen < nodeLen`: Didn't match whole node
- **Example:** Node = "testing", prefix = "teax"
  - Matches "te" (2 chars), but node is "testing" (7 chars)
  - Still need "ax" but can't continue through "sting"
- **Lw removed:** Would incorrectly collect words under partial matches

```cpp
    builtString += nextNode->data;
    current = nextNode;
    index += matchedLen;
  }
```
- **Progress:**
  - `builtString`: Add full node label (may extend beyond prefix)
  - `current`: Move to matched child
  - `index`: Advance prefix position

```cpp
  collectSuggestions(current, builtString, results);
  return results;
}
```
- **Collection:** Now at the prefix node, collect all descendants
- **Why `builtString`?** Because of compression, actual path may differ from input prefix
- **Example:** Prefix = "te", but first node might be "test" → builtString = "test"

### Edge Cases

| Case | Input | Result |
|------|-------|--------|
| Empty tree | "test" | Empty list |
| No match | "xyz" | Empty list |
| Exact node match | "test" = node | Words under "test" |
| Prefix inside node | "te" for node "test" | Still works - matches 2 chars |
| Prefix extends past node | "testing" for "test" node | Continue to children |

---

## 4. SuggestionList Data Structure

### Why Custom Linked List?

**Project Constraint:** `std::vector` was not allowed to be used.

### Structure Definition

```cpp
struct SuggestionNode {
  std::string word;      // El kelma kamla
  int frequency;         // Kam mara et-used
  long long timestamp;   // Emta et-access
  SuggestionNode *next;  // Link lel next node
};

struct SuggestionList {
  SuggestionNode *head;  // First element
  SuggestionNode *tail;  // Last element (for O(1) append)
  int count;             // Number of elements
};
```

### Key Methods

| Method | Purpose | Complexity |
|--------|---------|------------|
| `append()` | Add to end | O(1) - using tail pointer |
| `isEmpty()` | Check if empty | O(1) |
| `size()` | Get count | O(1) |

### Append Implementation Explained

```cpp
void append(const std::string &word, int frequency, long long timestamp) {
  SuggestionNode *newNode = new SuggestionNode(word, frequency, timestamp);
  if (!head) {
    head = tail = newNode;  // First element
  } else {
    tail->next = newNode;   // Link from current tail
    tail = newNode;         // Update tail
  }
  count++;
}
```

### Alternative: Array-based List
| Factor | Linked List | Array |
|--------|-------------|-------|
| Insert O(1) | ✅ With tail | ✅ Amortized (may resize) |
| Memory | Per-node overhead | Contiguous, cache-friendly |
| Deletion | O(1) if have pointer | O(n) shift |
| Project | ✅ Required | ❌ Not allowed |

---

## Summary

| Function | Purpose | Time Complexity |
|----------|---------|-----------------|
| `collectSuggestions` | Gather words recursively | O(n) all descendants |
| `getSuggestions` | Navigate + collect | O(m + n) where m=prefix len, n=results |

### Key Design Decisions

1. **Separation of concerns:** Navigate vs Collect
2. **Custom linked list:** Project requirement (no vector)
3. **Tail pointer:** O(1) append operations
4. **String building:** Handles compressed nodes correctly

