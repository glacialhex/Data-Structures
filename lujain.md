# Lujain - Delete Function (Complete Deep Dive)

> **Author:** Lujain Mohamed Helmi  
> **Functions Covered:** `deleteWord()`, `deleteRec()`, `destroy()`

---

## Table of Contents
1. [deleteWord Function - Public Interface](#1-deleteword-function)
2. [deleteRec Function - Recursive Deletion Logic](#2-deleterec-function)
3. [destroy Function - Complete Tree Cleanup](#3-destroy-function)

---

## 1. deleteWord Function

### File Location
`RadixTree.cpp` - Line 382

### Purpose
Public interface for deleting a word from the tree. Acts as a simple wrapper that calls the recursive helper function.

### Complete Code

```cpp
bool RadixTree::deleteWord(const char *word) { 
  return deleteRec(myRoot, word); 
}
```

### Line-by-Line Deep Explanation

---

#### Line 1: Function Signature
```cpp
bool RadixTree::deleteWord(const char *word) {
```

**`bool`** - Return type
- Returns `true` if the word was successfully deleted
- Returns `false` if the word was not found in the tree
- Boolean is appropriate because deletion is a yes/no operation

**`RadixTree::`** - Scope resolution operator
- Indicates this function is a member of the RadixTree class
- Required because the implementation is in .cpp file, separate from class declaration

**`deleteWord`** - Function name
- Clear, verb-based name indicating the action
- Follows the naming pattern of other public methods (insert, search)
- Public API - users call this function

**`const char *word`** - Parameter
- **`const`:** Promise that the function won't modify the input string
- **`char *`:** C-style string (pointer to null-terminated character array)
- **`word`:** The complete word to be deleted from the tree
- **Example values:** "test", "hello", "programming"

---

#### Line 2: Delegation to Recursive Helper
```cpp
  return deleteRec(myRoot, word);
```

**`return`** - Return statement
- Passes the result directly from deleteRec back to the caller
- No post-processing needed

**`deleteRec`** - Recursive helper function
- Does the actual work of finding and deleting the word
- Defined as a separate function for recursion

**`myRoot`** - First argument
- The root node of the tree
- Starting point for the recursive traversal
- `myRoot` is a member variable of the RadixTree class

**`word`** - Second argument
- Passed unchanged to the recursive function
- Will be consumed character-by-character during recursion

### Why Use a Wrapper Function?

| Reason | Explanation |
|--------|-------------|
| **Encapsulation** | Users don't need to know about internal root node |
| **Simpler API** | Just pass the word, no tree internals exposed |
| **Consistency** | Matches the pattern of other public methods |
| **Flexibility** | Could add pre/post processing without changing public API |

---

## 2. deleteRec Function

### File Location
`RadixTree.cpp` - Lines 384-424

### Purpose
Recursively traverse the tree to find the target word, then delete it with proper cleanup. This function handles:
1. Finding the word in the tree
2. Unmarking it as a complete word (logical deletion)
3. Physically removing unnecessary nodes (if they become leaves)
4. Propagating deletion upward when branches become empty

### Complete Code

```cpp
bool RadixTree::deleteRec(Node *&current, const char *word) {

  if (*word == '\0') {
    if (current->ended) {
      current->ended = false;
      return current->children == nullptr;
    }
    return false;
  }

  child *ch = current->children;
  child *prev = nullptr;

  while (ch) {
    if (ch->firstChar == word[0]) {
      int matched = matchPrefix(ch->node->data, word);
      if (matched == strlen(ch->node->data)) {
        bool shouldDeleteChild = deleteRec(ch->node, word + matched);

        if (shouldDeleteChild) {
          if (prev)
            prev->next = ch->next;
          else
            current->children = ch->next;

          delete ch->node;
          delete ch;
          return current->children == nullptr && !current->ended &&
                 current != myRoot;
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

### Line-by-Line Deep Explanation

---

#### Line 1: Function Signature
```cpp
bool RadixTree::deleteRec(Node *&current, const char *word) {
```

**`bool`** - Return type
- Returns `true` if the caller should delete this node
- Returns `false` if this node should be kept
- This return value is crucial for propagating deletion upward

**`Node *&current`** - First parameter (CRITICAL)
- **`Node *`:** Pointer to a Node
- **`&`:** Reference to the pointer
- **Combined `*&`:** Reference to a pointer - allows modifying the pointer itself
- **Why reference?** Without it, we could modify what the pointer points to, but couldn't update the parent's child pointer when we delete a node
- **If changed to `Node *current`:** Would break linked list removal - parent would still point to deleted node

**`const char *word`** - Second parameter
- The remaining portion of the word to find
- Gets shorter with each recursive call
- When it becomes empty (`*word == '\0'`), we've found the target

---

#### Lines 3-9: Base Case - Word Fully Matched
```cpp
  if (*word == '\0') {
    if (current->ended) {
      current->ended = false;
      return current->children == nullptr;
    }
    return false;
  }
```

Let's break this down piece by piece:

---

##### Line 3: Check if Word is Consumed
```cpp
  if (*word == '\0') {
```

**`*word`** - Dereference the pointer
- Gets the character at the current position
- `word` is a pointer; `*word` is the character it points to

**`== '\0'`** - Compare to null terminator
- `'\0'` (backslash-zero) is the null character, value 0
- All C-strings end with this character
- If true: We've matched all characters of the target word

**When this is true:**
- We've traversed the tree, matching each character
- Current node is where the word should end
- Now check if it was actually marked as a word

---

##### Lines 4-6: Word Found - Perform Logical Deletion
```cpp
    if (current->ended) {
      current->ended = false;
      return current->children == nullptr;
    }
```

**`current->ended`** - Check word marker
- `ended` is a boolean flag on each node
- `true` means this node represents a complete word
- **Example:** "test" node has ended=true if "test" is a word in tree

**`current->ended = false`** - Logical deletion
- Unmark this node as a word end
- The word is now "deleted" logically
- Node structure might remain (if it has children)

**`return current->children == nullptr`** - Decide physical deletion
- **If no children (`== nullptr`):** Return `true` → parent should delete this node
- **If has children:** Return `false` → keep node, it's still part of other words' paths

**Why keep nodes with children?**
- Consider tree with "test" and "testing"
- Deleting "test" shouldn't break path to "testing"
- "test" node still needed, just not a word anymore

---

##### Lines 7-8: Word Not Found at This Position
```cpp
    return false;
  }
```

**`return false`** - Word doesn't exist
- We consumed all characters but `ended` was false
- The path exists but it's not a complete word
- **Example:** Tree has "testing" but not "test" - path to "test" exists but not marked as word

---

#### Lines 11-12: Setup for Child Search
```cpp
  child *ch = current->children;
  child *prev = nullptr;
```

**`child *ch`** - Iterator for children linked list
- Points to current child being examined
- Starts at first child (head of linked list)

**`current->children`** - Access children
- `->` operator: dereference pointer and access member
- Returns pointer to first child in linked list
- Could be `nullptr` if no children

**`child *prev`** - Previous child pointer
- Used to maintain linked list when removing a child
- Initialized to `nullptr` because there's no previous at start

**Why track `prev`?**
- To remove from linked list, need to update previous node's `next` pointer
- Without `prev`, couldn't re-link after deletion

---

#### Line 14: Loop Through Children
```cpp
  while (ch) {
```

**`while (ch)`** - Loop condition
- `ch` as condition: true if not nullptr, false if nullptr
- Continues while there are more children to check

**Loop purpose:**
- Find the child whose edge matches the first character of remaining word
- Each child represents a possible path

---

#### Line 15: Check First Character Match
```cpp
    if (ch->firstChar == word[0]) {
```

**`ch->firstChar`** - Cached first character
- Each child struct stores the first character of its node's data
- **Why cached?** Avoids `ch->node->data[0]` (two dereferences)
- Performance optimization: O(1) check instead of memory access

**`word[0]`** - First character of remaining word
- The character we're looking for at this level

**`==`** - Equality comparison
- If equal, this child is a potential match

---

#### Lines 16-17: Calculate Match Length
```cpp
      int matched = matchPrefix(ch->node->data, word);
      if (matched == strlen(ch->node->data)) {
```

**`matchPrefix(...)`** - Get number of matching characters
- Compares `ch->node->data` with `word` from start
- Returns count of matching characters

**`matched == strlen(ch->node->data)`** - Full match check
- Must match the ENTIRE node label to proceed
- **If true:** This is the correct path, continue recursion
- **If false:** Partial match, word doesn't exist with this exact path

**Why require full match?**
- Radix tree edges are labeled with complete strings
- Can only proceed through an edge if key contains that entire string

---

#### Line 18: Recursive Call
```cpp
        bool shouldDeleteChild = deleteRec(ch->node, word + matched);
```

**`deleteRec(...)`** - Recursive call
- Continue deletion in the child subtree
- This is where the recursive magic happens

**`ch->node`** - First argument
- The child node to recurse into
- Note: This is passed by reference (the function signature is `Node *&`)

**`word + matched`** - Second argument: Remaining word
- Pointer arithmetic: advances pointer by `matched` positions
- **Example:** word="testing", matched=4 → word+4 points to "ing"
- Passes only the unmatched portion to next call

**`shouldDeleteChild`** - Return value
- `true`: The child node should be physically deleted
- `false`: Keep the child node

---

#### Lines 20-28: Handle Child Deletion
```cpp
        if (shouldDeleteChild) {
          if (prev)
            prev->next = ch->next;
          else
            current->children = ch->next;

          delete ch->node;
          delete ch;
          return current->children == nullptr && !current->ended &&
                 current != myRoot;
        }
```

##### Line 20: Check If Should Delete
```cpp
        if (shouldDeleteChild) {
```
- Only perform physical deletion if recursive call said to

##### Lines 21-24: Unlink From Linked List
```cpp
          if (prev)
            prev->next = ch->next;
          else
            current->children = ch->next;
```

**`if (prev)`** - Check if there's a previous child
- **If yes:** This child is not first in list
- **If no (else):** This child IS first, need to update parent's pointer

**`prev->next = ch->next`** - Skip over current child
- Previous child now points to child after current
- Current child is "unlinked" from list
```
Before: prev -> ch -> ch->next
After:  prev -------> ch->next
```

**`current->children = ch->next`** - Update parent
- Current child was first in list
- Parent now points to second child (which becomes first)
- Handles case where `ch->next` is nullptr (only child)

##### Lines 26-27: Delete Memory
```cpp
          delete ch->node;
          delete ch;
```

**`delete ch->node`** - Delete the Node structure
- Frees the actual tree node
- Must happen before deleting ch (which contains the pointer)

**`delete ch`** - Delete the child wrapper
- Frees the child linked list node
- **Order matters?** Theoretically no, but deleting node first is conventional

##### Lines 28-29: Propagate Deletion Decision
```cpp
          return current->children == nullptr && !current->ended &&
                 current != myRoot;
```

**Return decision:** Should CURRENT node also be deleted?

**Three conditions (ALL must be true):**

1. **`current->children == nullptr`**
   - After removing child, no children remain
   - Node is now a leaf

2. **`!current->ended`**
   - Node is not a word end
   - No reason to keep it for word marking

3. **`current != myRoot`**
   - Never delete the root node
   - Root is the sentinel/anchor of the tree

**If all true:** Return `true` → parent should delete this node too
**If any false:** Return `false` → stop propagation

---

#### Line 30: Recursive Call Didn't Request Deletion
```cpp
        return false;
```
- Child was found but shouldn't be deleted
- Keep entire parent branch intact

---

#### Lines 32-34: Continue Child Search
```cpp
    prev = ch;
    ch = ch->next;
  }
```

**`prev = ch`** - Save current as previous
- Before moving forward, remember current child
- Needed for linked list unlinking

**`ch = ch->next`** - Move to next sibling
- Navigate to next child in linked list
- If null, loop will exit

---

#### Line 35: Word Not Found
```cpp
  return false;
}
```

**`return false`** - No match in tree
- Searched all children, none matched
- Word doesn't exist, nothing to delete

---

### Visual Deletion Example

```
Initial tree: "test" and "testing" both exist

        [root]
           |
        "test" [ended=true]
           |
        "ing" [ended=true]

Delete "testing":

Step 1: deleteRec(root, "testing")
  - Find child "test", matched=4
  - Recurse: deleteRec(testNode, "ing")

Step 2: deleteRec(testNode, "ing")
  - Find child "ing", matched=3
  - Recurse: deleteRec(ingNode, "")

Step 3: deleteRec(ingNode, "")
  - word[0] == '\0' → base case
  - ingNode->ended == true → set to false
  - ingNode->children == nullptr → return true (delete me)

Step 4: Back in Step 2
  - shouldDeleteChild == true
  - Delete "ing" node from memory
  - testNode->children becomes nullptr
  - Check: children==null? YES
  - Check: ended==false? NO (test is still a word!)
  - Return false (keep testNode)

Result:
        [root]
           |
        "test" [ended=true]
        
"testing" deleted, "test" preserved!
```

---

## 3. destroy Function

### File Location
`RadixTree.cpp` - Lines 30-42

### Purpose
Recursively delete all nodes in the tree. Used by the destructor to free all memory.

### Complete Code

```cpp
void RadixTree::destroy(Node *node) {
  if (!node)
    return;
  child *ch = node->children;
  while (ch) {
    destroy(ch->node);
    child *old = ch;
    ch = ch->next;
    delete old;
  }
  delete node;
}
```

### Line-by-Line Deep Explanation

---

#### Line 1: Function Signature
```cpp
void RadixTree::destroy(Node *node) {
```

**`void`** - Return type
- No return value
- Pure side-effect function (freeing memory)

**`Node *node`** - Parameter
- The root of subtree to destroy
- NOT passed by reference (unlike deleteRec)
- **Why not reference?** We don't need to modify parent's pointer

---

#### Lines 2-3: Base Case
```cpp
  if (!node)
    return;
```

**`!node`** - Null check
- `!` negates the pointer
- True if node is nullptr

**`return`** - Exit early
- Nothing to destroy if node is null
- Prevents crash from dereferencing null

---

#### Lines 4-9: Destroy Children First
```cpp
  child *ch = node->children;
  while (ch) {
    destroy(ch->node);
    child *old = ch;
    ch = ch->next;
    delete old;
  }
```

**`child *ch = node->children`** - Get first child
- Start at head of children linked list

**`while (ch)`** - Loop through children
- Continue while children remain

**`destroy(ch->node)`** - Recursive call
- Destroy child subtree BEFORE deleting child
- **Post-order traversal:** Children before parent
- **Critical:** Must destroy subtree before freeing node

**`child *old = ch`** - Save current pointer
- Need to delete after moving ch forward
- If we deleted first, couldn't access ch->next

**`ch = ch->next`** - Move to next sibling
- Save next pointer before deletion

**`delete old`** - Delete child wrapper
- Free the child struct memory
- Node was already freed by recursive destroy() call

---

#### Line 10: Delete Current Node
```cpp
  delete node;
}
```

**`delete node`** - Free current node
- All children already freed
- Safe to delete now

---

### Destructor

```cpp
RadixTree::~RadixTree() {
  destroy(myRoot);
}
```

**`~RadixTree()`** - Destructor
- Called automatically when RadixTree goes out of scope
- Or when `delete` is called on a RadixTree pointer

**`destroy(myRoot)`** - Recursive cleanup
- Frees all nodes starting from root
- Prevents memory leaks

---

### Traversal Order: Why Post-Order?

**Post-order:** Process children before parent

```
     A
    / \
   B   C
  / \
 D   E

Post-order: D, E, B, C, A
```

**Why required for deletion?**
1. If delete parent first, lose access to children
2. Children become orphaned memory (leak)
3. Post-order ensures we can access everything before deletion

---

## Complexity Summary

| Function | Time Complexity | Space Complexity |
|----------|-----------------|------------------|
| deleteWord | O(m) where m = word length | O(m) call stack |
| deleteRec | O(m) | O(m) call stack |
| destroy | O(n) where n = total nodes | O(h) where h = height |

---

## Edge Cases

| Case | Behavior |
|------|----------|
| Word not in tree | Returns false, no changes |
| Delete only word in tree | Node deleted, tree empty |
| Delete prefix of existing word | Keep node, mark ended=false |
| Delete word that is prefix | Keep node structure, unmark |
| Delete leaf word | Physical deletion, may propagate |
| Empty tree | Returns false immediately |

