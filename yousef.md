# Yousef - Search Function

> **Author:** Yousef Mohamed Helmy  
> **Functions Covered:** `search()`, `matchPrefix()`, `handleSearchFailure()`

---

## Table of Contents
1. [Overview](#1-overview)
2. [matchPrefix Function](#2-matchprefix-function)
3. [search Function](#3-search-function)
4. [handleSearchFailure Function](#4-handlesearchfailure-function)

---

## 1. Overview

El search system byet-composed of:
1. **`matchPrefix`**: Core comparison logic
2. **`search`**: Main search algorithm
3. **`handleSearchFailure`**: Error handling (silent mode)

### Search vs Autocomplete
| Feature | search() | getSuggestions() |
|---------|----------|-----------------|
| Returns | boolean | list of words |
| Purpose | Check if word exists | Find all completions |
| Matching | Exact word | Prefix-based |

---

## 2. matchPrefix Function

### Purpose
Compare two strings and return how many characters match from the start.

### Code with Line-by-Line Explanation

```cpp
int RadixTree::matchPrefix(const char *nodePrefix, const char *keySegment) {
```
- **Parameters:**
  - `nodePrefix`: El data stored in the node
  - `keySegment`: El part of the search key to compare
- **Return:** Number of matching characters

```cpp
  int i = 0;
```
- **Counter:** Tracks matching position
- **Initialized to 0:** Start from first character

```cpp
  while (nodePrefix[i] != '\0' && keySegment[i] != '\0' &&
         nodePrefix[i] == keySegment[i])
    i++;
```
- **Three conditions (ALL must be true):**
  1. `nodePrefix[i] != '\0'`: Haven't reached end of node data
  2. `keySegment[i] != '\0'`: Haven't reached end of search key
  3. `nodePrefix[i] == keySegment[i]`: Characters match
- **Why all three?** Prevents buffer overrun and ensures valid comparison
- **Lw removed any:**
  - Without condition 1: Read past node data → undefined behavior
  - Without condition 2: Read past key → undefined behavior
  - Without condition 3: Would count mismatches

```cpp
  return i;
```
- **Return count:** How many characters matched
- **Examples:**
  - `matchPrefix("test", "testing")` → 4
  - `matchPrefix("testing", "team")` → 2 (just "te")
  - `matchPrefix("abc", "xyz")` → 0

### Why Not Use strcmp?
| Factor | matchPrefix | strcmp |
|--------|-------------|--------|
| Returns | Match length | -1, 0, or 1 |
| Partial matches | ✅ Handled | ❌ Can't detect |
| Our use case | ✅ Perfect fit | ❌ Not suitable |

---

## 3. search Function

### Purpose
Check if a word exists in the tree as a complete word.

### Code with Line-by-Line Explanation

```cpp
bool RadixTree::search(const char *key) {
```
- **Parameter:** `key` - el word to search for
- **Return:** `true` if found as complete word, `false` otherwise

```cpp
  if (empty())
    return false;
```
- **Guard clause:** Empty tree has no words
- **`empty()`**: Returns `myRoot->children == nullptr`
- **Lw removed:** Would crash trying to access null children

```cpp
  Node *current = myRoot;
  int keyLen = strlen(key);
  int keyIndex = 0;
```
- **`current`**: Navigation pointer, starts at root
- **`keyLen`**: Total length of search key
- **`keyIndex`**: Current position in key (how many chars matched so far)

```cpp
  while (keyIndex < keyLen) {
```
- **Main loop:** Continue until all characters matched
- **Exit:** When `keyIndex >= keyLen` → whole key processed

```cpp
    char edgeChar = key[keyIndex];
```
- **Current character:** The character looking for at this level
- **Used for:** O(1) child lookup via firstChar

```cpp
    child *ch = current->children;
    child *found = nullptr;
```
- **`ch`**: Iterator for children linked list
- **`found`**: Will hold the matching child (if any)

```cpp
    while (ch) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }
```
- **Child search:** Find child starting with needed character
- **`ch->firstChar`**: Cached first character for O(1) comparison
- **Why cache?** Avoids dereferencing `ch->node->data[0]` repeatedly
- **Why break?** Only one child can start with each character (Radix property)

```cpp
    if (!found) {
      handleSearchFailure("No matching child found");
      return false;
    }
```
- **No match:** Key character not found in any child
- **Example:** Searching "xyz" when tree only has "abc"
- **`handleSearchFailure`**: Logs reason (currently silent)

```cpp
    int matched = matchPrefix(found->node->data, key + keyIndex);
    int nodeLen = strlen(found->node->data);
```
- **`matched`**: How many characters of node data matched
- **`nodeLen`**: Total length of node data
- **`key + keyIndex`**: Pointer arithmetic - remaining part of key

```cpp
    if (matched < nodeLen) {
      handleSearchFailure("Key ended mid-edge or mismatch");
      return false;
    }
```
- **Strict matching:** Must match ENTIRE node label
- **Failure cases:**
  - Key ends before node data: "te" searching in "test" node
  - Mismatch mid-node: "teax" in "test" node (matches "te", fails at 'a' vs 's')
- **Why strict?** search() is for exact word lookup, not prefix matching

```cpp
    keyIndex += matched;
    current = found->node;
  }
```
- **Progress:**
  - Move keyIndex forward by matched amount
  - Move to matched child node
- **Loop continues:** Until whole key processed

```cpp
  if (current->ended)
    return true;
```
- **Final check:** Is current node marked as word end?
- **Why needed?** Node might exist but not be a complete word
- **Example:** "test" exists, searching "tes" → node found but not `ended`

```cpp
  handleSearchFailure("Key found but not marked as complete word");
  return false;
}
```
- **Path exists but not a word:** All characters matched, but not a valid word
- **Example:** Tree has "testing", searching "test" → path exists but "test" might not be `ended`

### Visual Search Example

```
Tree structure:
root → "te" → "st" [ended=true] → "ing" [ended=true]
              ↓
            "am" [ended=true]

Search("test"):
1. keyIndex=0, edgeChar='t', find child "te"
2. matched=2, nodeLen=2 ✓
3. keyIndex=2, edgeChar='s', find child "st"
4. matched=2, nodeLen=2 ✓
5. keyIndex=4, loop exits (keyIndex=4 >= keyLen=4)
6. current->ended = true → FOUND!

Search("tea"):
1. keyIndex=0, edgeChar='t', find child "te"
2. matched=2, nodeLen=2 ✓
3. keyIndex=2, edgeChar='a', looking for child starting with 'a'
4. Children: "st", "am" → found "am"? NO! "am"->firstChar='a'? NO!
   Wait - let me reconsider the tree structure...
   Actually each node has its own label, so from "te", children are "st" and "am"
5. Looking for 'a' at index 2 → child "am" has firstChar='a' ✓
6. But wait, "tea" needs to match... matched=1 ("a"), but need full match
   This is getting complex - let me simplify.

Actually: search("tea") would fail because:
- After matching "te", remaining is "a"
- Child "am" starts with 'a' but node data is "am" (2 chars)
- matched=1, nodeLen=2 → matched < nodeLen → FAIL
```

### Complexity Analysis

| Metric | Complexity | Explanation |
|--------|-----------|-------------|
| Time | O(m) | m = key length, visit at most m nodes |
| Space | O(1) | No additional storage, just pointers |

---

## 4. handleSearchFailure Function

### Purpose
Centralized error handling for search failures.

### Code

```cpp
void RadixTree::handleSearchFailure(const char *reason) {
  // cout << "Search failed: " << reason << endl; // Silent for optimization
}
```

- **Currently silent:** Commented out for production
- **Parameter:** Error message describing failure reason
- **Use cases:**
  - Debugging during development
  - Logging in production (if enabled)

### Why Silent?
| Factor | Verbose | Silent |
|--------|---------|--------|
| Performance | cout is slow | No overhead |
| User experience | Spam on miss | Clean output |
| Debugging | Helpful | Enable as needed |

---

## Edge Cases

| Case | Input | Result |
|------|-------|--------|
| Empty tree | "test" | false |
| Empty key | "" | false (or handle specially) |
| Exact match | "test" when "test" exists | true |
| Prefix only | "te" when only "test" exists | false |
| Path exists, not word | "tes" when "test" exists but not "tes" | false |
| Case sensitive | "Test" vs "test" | Depends on insert (currently case matters) |

---

## Alternative Approaches

### Recursive Search
```cpp
bool searchRec(Node* node, const char* key) {
  // Base cases and recursion
}
```
| Factor | Iterative (current) | Recursive |
|--------|-------------------|-----------|
| Stack usage | O(1) | O(m) |
| Readability | Clear | Also clear |
| Performance | ✅ Better | Stack overhead |

### Hash-Based Lookup
Store words in hash table for O(1) lookup:
| Factor | Radix Tree | Hash Table |
|--------|------------|------------|
| Exact search | O(m) | O(1) average |
| Prefix search | ✅ Supported | ❌ Not supported |
| Our choice | ✅ Used | Not suitable |

---

## Summary

| Function | Purpose | Complexity |
|----------|---------|------------|
| `matchPrefix` | Compare strings | O(min(a,b)) |
| `search` | Find exact word | O(m) |
| `handleSearchFailure` | Error logging | O(1) |

### Key Design Decisions
1. **Iterative over recursive:** Better stack usage
2. **Strict matching:** Entire node must match
3. **firstChar caching:** O(1) child lookup
4. **Silent errors:** Production performance

