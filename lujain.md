# Lujain - Delete Function

> **Author:** Lujain Mohamed Helmi  
> **Functions Covered:** `deleteWord()`, `deleteRec()`, `destroy()`

---

## Table of Contents
1. [Overview](#1-overview)
2. [deleteWord Function](#2-deleteword-function)
3. [deleteRec Function](#3-deleterec-function)
4. [destroy Function](#4-destroy-function)

---

## 1. Overview

El deletion system byet-composed of:
1. **`deleteWord`**: Public wrapper function
2. **`deleteRec`**: Recursive deletion with cleanup logic
3. **`destroy`**: Complete tree cleanup (destructor helper)

### Key Concepts

| Concept | Explanation |
|---------|-------------|
| Logical deletion | Mark `ended = false`, keep node if has children |
| Physical deletion | Remove node from memory if it's a leaf |
| Propagation | Deletion may cascade up (remove empty branches) |

---

## 2. deleteWord Function

### Code with Line-by-Line Explanation

```cpp
bool RadixTree::deleteWord(const char *word) { 
  return deleteRec(myRoot, word); 
}
```
- **Purpose:** Public interface for deletion
- **Parameter:** `word` - el kelma ely 3ayzeen nem7aha
- **Return:** `true` if word was deleted, `false` if not found
- **Delegation:** Calls recursive helper starting from root

### Why Wrapper Function?
| Reason | Explanation |
|--------|-------------|
| Encapsulation | Users don't need to know about root |
| Simpler API | Just pass the word, internal details hidden |
| Consistent pattern | Same as other tree operations |

---

## 3. deleteRec Function

### Purpose
Recursively traverse to find the word, then delete it with proper cleanup of empty branches.

### Function Signature

```cpp
bool RadixTree::deleteRec(Node *&current, const char *word) {
```
- **`Node *&current`**: Reference to pointer - critical for modification
  - **Why reference?** Allows changing the pointer itself (for removal)
  - **Lw just pointer?** Could only modify what it points to, not remove from parent
- **`const char *word`**: Remaining part of word to find
- **Return:** `true` = delete this node, `false` = keep it

### Base Case: Word Consumed

```cpp
  if (*word == '\0') {
    if (current->ended) {
      current->ended = false;
      return current->children == nullptr;
    }
    return false;
  }
```

Let's break this down:

```cpp
  if (*word == '\0') {
```
- **Check:** Have all characters been matched?
- **`*word`**: Current character (first char of remaining string)
- **`'\0'`**: Null terminator = end of string

```cpp
    if (current->ended) {
      current->ended = false;
```
- **Found the word:** It exists and is marked as complete word
- **Logical deletion:** Unmark it as word end
- **Note:** Node still exists, just not a valid word anymore

```cpp
      return current->children == nullptr;
```
- **Decision:** Should parent delete this node?
  - **Has children?** Keep node (still part of other words' paths)
  - **No children?** Safe to delete (leaf node)
- **Example:** Deleting "test" when "testing" exists → keep node

```cpp
    return false;
  }
```
- **Word not found:** Reached a node but it's not marked as word
- **Example:** Deleting "tes" when only "test" exists

### Visual Example: Deletion Decisions

```
Case 1: Delete "test" (has child "ing")
Before: "test" [ended=true] → "ing" [ended=true]
After:  "test" [ended=false] → "ing" [ended=true]
Return: false (keep "test" node)

Case 2: Delete "testing" (leaf)
Before: "test" → "ing" [ended=true, no children]
After:  "test" → (deleted)
Return: true (remove "ing" node)
```

### Finding the Matching Child

```cpp
  child *ch = current->children;
  child *prev = nullptr;
```
- **`ch`**: Iterator for children linked list
- **`prev`**: Tracks previous node (needed for linked list removal)
- **Why track prev?** To re-link when removing from middle of list

```cpp
  while (ch) {
    if (ch->firstChar == word[0]) {
```
- **Loop:** Search all children
- **Match criterion:** First character matches
- **`ch->firstChar`**: Cached for O(1) comparison

```cpp
      int matched = matchPrefix(ch->node->data, word);
      if (matched == strlen(ch->node->data)) {
```
- **Full match check:** Must match entire node label
- **Why?** Radix tree nodes store multiple characters
- **Example:** Node "test", word "te" → matched=2, strlen=4 → not full match

```cpp
        bool shouldDeleteChild = deleteRec(ch->node, word + matched);
```
- **Recurse:** Continue with remaining word
- **`word + matched`**: Pointer arithmetic - skip matched characters
- **Return value:** Should this child be physically deleted?

### Physical Deletion of Child

```cpp
        if (shouldDeleteChild) {
          if (prev)
            prev->next = ch->next;
          else
            current->children = ch->next;
```
- **Linked list removal:**
  - **Has previous?** Re-link prev to next, skipping current
  - **First child?** Update parent's children pointer

```cpp
          delete ch->node;
          delete ch;
```
- **Memory cleanup:**
  - Delete the actual Node
  - Delete the child wrapper struct
- **Order matters?** No, but consistent practice

```cpp
          return current->children == nullptr && !current->ended &&
                 current != myRoot;
```
- **Propagation decision:** Should parent also delete current node?
- **Conditions (ALL must be true):**
  1. `children == nullptr`: No more children
  2. `!current->ended`: Not a word end
  3. `current != myRoot`: Never delete root
- **Lw removed:** Empty branches would remain in tree

```cpp
        }
        return false;
      }
    }
    prev = ch;
    ch = ch->next;
  }
  return false;
}
```
- **Continue search:** Move to next sibling
- **Not found:** Return false

### Complete Deletion Flow

```
Word to delete: "testing"
Tree: root → "test" [ended=true] → "ing" [ended=true]

Step 1: deleteRec(root, "testing")
  - Find child "test" (firstChar='t')
  - matched=4, strlen("test")=4 ✓
  - Recurse: deleteRec(testNode, "ing")

Step 2: deleteRec(testNode, "ing")
  - Find child "ing" (firstChar='i')
  - matched=3, strlen("ing")=3 ✓
  - Recurse: deleteRec(ingNode, "")

Step 3: deleteRec(ingNode, "")
  - word[0] == '\0' → base case
  - ingNode->ended=true → set to false
  - ingNode->children==nullptr → return true

Step 4: Back in step 2
  - shouldDeleteChild=true
  - Delete "ing" node
  - testNode->children becomes nullptr
  - testNode->ended=true → return false (keep it)

Result: "testing" deleted, "test" remains
```

---

## 4. destroy Function

### Purpose
Complete cleanup of all nodes in the tree (used by destructor).

### Code with Line-by-Line Explanation

```cpp
void RadixTree::destroy(Node *node) {
  if (!node)
    return;
```
- **Guard:** Handle null nodes gracefully
- **Base case:** Nothing to destroy

```cpp
  child *ch = node->children;
  while (ch) {
    destroy(ch->node);
    child *old = ch;
    ch = ch->next;
    delete old;
  }
```
- **Post-order traversal:** Delete children before parent
- **Why post-order?** Can't access children after parent deleted
- **Save `old`:** Need pointer to delete after moving to next

```cpp
  delete node;
}
```
- **Final cleanup:** Delete the node itself
- **Called by:** Destructor `~RadixTree()`

### Destructor

```cpp
RadixTree::~RadixTree() {
  destroy(myRoot);
}
```
- **Automatic cleanup:** When RadixTree goes out of scope
- **Prevents memory leaks:** All nodes freed

---

## Complexity Analysis

| Operation | Time | Space |
|-----------|------|-------|
| `deleteWord` | O(m) where m = word length | O(m) recursion stack |
| `destroy` | O(n) where n = total nodes | O(h) where h = height |

---

## Edge Cases

| Case | Behavior |
|------|----------|
| Word not in tree | Returns false, no changes |
| Delete prefix of existing word | Keep node, mark ended=false |
| Delete leaf word | Physical deletion, may propagate |
| Delete root (impossible) | Protected by `current != myRoot` |
| Empty tree | Returns false immediately |

---

## Alternative Approaches

### Iterative Deletion
Could use a stack instead of recursion:
| Factor | Recursive | Iterative |
|--------|-----------|-----------|
| Code clarity | ✅ Cleaner | More complex |
| Stack overflow | Possible for very deep trees | No |
| Current choice | ✅ Used (trees not deep) | Not used |

### Lazy Deletion
Just mark `ended=false`, never physically delete:
| Factor | Current Approach | Lazy |
|--------|-----------------|------|
| Memory | Reclaimed | Wasted |
| Complexity | O(m) | O(m) |
| Current choice | ✅ Used | Not used |

---

## Summary

| Function | Purpose | Key Feature |
|----------|---------|-------------|
| `deleteWord` | Public API | Simple wrapper |
| `deleteRec` | Core logic | Recursive with propagation |
| `destroy` | Cleanup | Post-order traversal |

