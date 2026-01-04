# Yousef - Search Function (Complete Deep Dive)

> **Author:** Yousef Mohamed Helmy  
> **Functions Covered:** `search()`, `matchPrefix()`, `handleSearchFailure()`, `getCurrentTimestamp()`, `updateWordFrequency()`

---

## Table of Contents
1. [matchPrefix Function - Character-by-Character Comparison](#1-matchprefix-function)
2. [search Function - Complete Word Lookup](#2-search-function)
3. [handleSearchFailure Function](#3-handlesearchfailure-function)
4. [getCurrentTimestamp Function](#4-getcurrenttimestamp-function)
5. [updateWordFrequency Function](#5-updatewordfrequency-function)

---

## 1. matchPrefix Function

### File Location
`RadixTree.cpp` - Lines 56-63

### Purpose
This function compares two C-strings character by character from the beginning and returns how many characters match consecutively. This is the foundation of all traversal operations in a Radix Tree.

### Complete Code

```cpp
int RadixTree::matchPrefix(const char *nodePrefix, const char *keySegment) {
  int i = 0;
  while (nodePrefix[i] != '\0' && keySegment[i] != '\0' &&
         nodePrefix[i] == keySegment[i])
    i++;
  return i;
}
```

### Line-by-Line Deep Explanation

---

#### Line 1: Function Signature
```cpp
int RadixTree::matchPrefix(const char *nodePrefix, const char *keySegment) {
```

**`int`** - Return type
- Returns an integer representing the number of matching characters
- Range is 0 to min(strlen(nodePrefix), strlen(keySegment))
- 0 means no characters match (first character is different)

**`RadixTree::`** - Scope resolution operator
- Indicates this function belongs to the RadixTree class
- Required because the function is defined outside the class declaration in the .cpp file

**`matchPrefix`** - Function name
- Descriptive name indicating it matches prefixes
- Following naming convention: verb + noun

**`const char *nodePrefix`** - First parameter
- **Type:** Pointer to constant character array (C-string)
- **`const`:** Guarantees the function won't modify this string
- **Purpose:** Contains the data stored in the tree node (e.g., "testing")
- **Why pointer not copy?** Efficiency - copying strings is O(n), passing pointer is O(1)
- **Example value:** "test", "testing", "team"

**`const char *keySegment`** - Second parameter
- **Type:** Pointer to constant character array (C-string)
- **`const`:** Guarantees the function won't modify this string
- **Purpose:** Contains the part of the search key being compared
- **Why "segment"?** Because we pass portions of the key as we traverse deeper
- **Example value:** "test", "esting" (after matching "t"), etc.

---

#### Line 2: Counter Initialization
```cpp
  int i = 0;
```

**`int`** - Type declaration
- Using int for the counter (could also use size_t for unsigned)
- int is sufficient since word lengths are never negative

**`i`** - Variable name
- Common convention for loop counters
- Represents the current index being compared in both strings

**`= 0`** - Initialization
- Start comparing from the first character (index 0)
- **Critical:** C-strings are 0-indexed

**Why initialize here, not in loop?**
- Need the value of i after the loop ends
- If declared inside loop, would go out of scope

---

#### Line 3-4: The Comparison Loop
```cpp
  while (nodePrefix[i] != '\0' && keySegment[i] != '\0' &&
         nodePrefix[i] == keySegment[i])
```

This is the heart of the function. Let's break down each condition:

**`while`** - Loop type
- Continues as long as all conditions are true
- Could also use a for loop, but while is clearer here since we don't know iteration count

**`nodePrefix[i] != '\0'`** - Condition 1: Not at end of node data
- **`nodePrefix[i]`:** Access character at index i
- **`'\0'`:** Null terminator - signals end of C-string
- **`!=`:** Continue only if we haven't reached the end
- **Why needed?** Prevents reading past the end of node data
- **If removed:** Buffer overrun → undefined behavior, possible crash

**`&&`** - Logical AND
- All conditions must be true for loop to continue
- Short-circuit evaluation: if first condition false, others not checked
- This is important for safety and performance

**`keySegment[i] != '\0'`** - Condition 2: Not at end of key
- Same logic as condition 1, but for the search key
- **Why needed?** Prevents reading past the end of key
- **If removed:** Buffer overrun when key is shorter than node data

**`nodePrefix[i] == keySegment[i]`** - Condition 3: Characters match
- **`==`:** Comparison operator
- Compares the characters at position i in both strings
- Case-sensitive comparison (A ≠ a)
- **Why last?** Short-circuit evaluation - no point comparing if already past end

---

#### Line 5: Increment Counter
```cpp
    i++;
```

**`i++`** - Post-increment operator
- Increases i by 1 after the current iteration
- Only executes if all while conditions were true
- Moves to the next character position

**Why not `++i`?**
- In this context, both would work identically
- Post-increment is conventional for simple counters
- Pre-increment would also work since we don't use the value

---

#### Line 6: Return Result
```cpp
  return i;
```

**`return`** - Return statement
- Exits the function and returns the value
- No further code in this function executes

**`i`** - The match count
- After loop ends, i contains the number of matching characters
- Represents how many characters from position 0 matched

### Return Value Scenarios

| nodePrefix | keySegment | Return | Explanation |
|------------|-----------|--------|-------------|
| "test" | "testing" | 4 | All of "test" matches |
| "testing" | "test" | 4 | Key ends, 4 matched |
| "test" | "team" | 2 | "te" matches, 's' ≠ 'a' |
| "abc" | "xyz" | 0 | First chars differ |
| "" | "test" | 0 | Empty string, immediate end |
| "test" | "" | 0 | Empty key, immediate end |

### Why Not Use strcmp() or strncmp()?

| Factor | matchPrefix | strcmp |
|--------|-------------|--------|
| Returns | Match length (int) | -1, 0, or 1 |
| Partial matches | ✅ Returns count | ❌ Just says "different" |
| Our need | Need to know WHERE mismatch | Just need to know IF match |
| Use case | Perfect for Radix Trees | Good for sorting |

---

## 2. search Function

### File Location
`RadixTree.cpp` - Lines 193-241

### Purpose
Determine if a complete word exists in the Radix Tree. Returns true only if:
1. The entire path for the word exists
2. The final node is marked as a word end (`ended = true`)

### Complete Code

```cpp
bool RadixTree::search(const char *key) {
  if (empty())
    return false;

  Node *current = myRoot;
  int keyLen = strlen(key);
  int keyIndex = 0;

  while (keyIndex < keyLen) {
    char edgeChar = key[keyIndex];
    child *ch = current->children;
    child *found = nullptr;

    while (ch) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }

    if (!found) {
      handleSearchFailure("No matching child found");
      return false;
    }

    int matched = matchPrefix(found->node->data, key + keyIndex);
    int nodeLen = strlen(found->node->data);

    if (matched < nodeLen) {
      handleSearchFailure("Key ended mid-edge or mismatch");
      return false;
    }

    keyIndex += matched;
    current = found->node;
  }

  if (current->ended)
    return true;
  handleSearchFailure("Key found but not marked as complete word");
  return false;
}
```

### Line-by-Line Deep Explanation

---

#### Line 1: Function Signature
```cpp
bool RadixTree::search(const char *key) {
```

**`bool`** - Return type
- Boolean: can only be `true` or `false`
- `true`: Word exists in tree as complete word
- `false`: Word not found or exists only as prefix

**`RadixTree::`** - Class scope
- This function is a member of the RadixTree class

**`search`** - Function name
- Clear, descriptive name
- Follows common naming convention for lookup operations

**`const char *key`** - Parameter
- **`const`:** Function promises not to modify the input
- **`char *`:** C-style string (pointer to first character)
- **`key`:** The word being searched for
- **Example:** "test", "hello", "programming"

---

#### Lines 2-3: Empty Tree Check
```cpp
  if (empty())
    return false;
```

**`if`** - Conditional statement
- Checks a condition before proceeding

**`empty()`** - Member function call
- Returns true if tree has no words
- Implementation: `return myRoot->children == nullptr;`
- **Why check?** Avoid null pointer dereference in empty tree

**`return false`** - Early exit
- Empty tree cannot contain any words
- **Pattern:** "Guard clause" - fail fast for invalid input
- **If removed:** Would attempt to access nullptr, causing crash

---

#### Line 5: Initialize Current Node Pointer
```cpp
  Node *current = myRoot;
```

**`Node *`** - Type declaration
- Pointer to a Node object
- Allows navigation through tree without copying nodes

**`current`** - Variable name
- Represents the current position in tree traversal
- Will be updated as we move deeper

**`= myRoot`** - Initialization
- Start at the root of the tree
- `myRoot` is a member variable of RadixTree class
- Root is a sentinel node (doesn't contain word data)

---

#### Line 6: Calculate Key Length
```cpp
  int keyLen = strlen(key);
```

**`int`** - Type
- Integer to store the length
- `size_t` would be more precise but int is commonly used

**`keyLen`** - Variable name
- Stores the length of the search key
- Used to know when all characters have been matched

**`strlen(key)`** - Standard library function
- Returns number of characters before null terminator
- O(n) operation - traverses string once
- **Example:** strlen("test") returns 4

**Why calculate once?**
- strlen() is O(n) - traverses entire string
- Computing once avoids repeated traversals
- If called in loop: O(n*m) instead of O(n)

---

#### Line 7: Initialize Key Index
```cpp
  int keyIndex = 0;
```

**`int`** - Type
- Integer to track position in key

**`keyIndex`** - Variable name
- Tracks how many characters have been matched
- Starts at 0 (first character)

**`= 0`** - Initialization
- Begin matching from the first character

---

#### Lines 9: Main Loop
```cpp
  while (keyIndex < keyLen) {
```

**`while`** - Loop type
- Continues as long as condition is true
- Better than `for` here since we don't know step size in advance

**`keyIndex < keyLen`** - Condition
- Continue while there are unmatched characters
- Once keyIndex >= keyLen, all characters matched

**When loop ends:**
- `keyIndex >= keyLen`: All characters matched, go to final check
- Or `return false` occurred: Word not found

---

#### Line 10: Get Current Character
```cpp
    char edgeChar = key[keyIndex];
```

**`char`** - Type
- Single character (1 byte)

**`edgeChar`** - Variable name
- "Edge" refers to the connection between nodes
- In Radix Trees, edges are labeled with strings
- This char is what we're looking for

**`key[keyIndex]`** - Array access
- Gets the character at position keyIndex
- **Example:** key="test", keyIndex=0 → 't'

---

#### Lines 11-12: Initialize Child Search Variables
```cpp
    child *ch = current->children;
    child *found = nullptr;
```

**`child *ch`** - Iterator variable
- Pointer to child struct
- Used to traverse children linked list

**`current->children`** - Starting point
- Access children of current node
- `->`operator: dereference pointer and access member
- Returns pointer to first child (head of linked list)

**`child *found`** - Result variable
- Will hold the matching child if found
- Initialized to nullptr to indicate "not found yet"

**`nullptr`** - Null pointer literal (C++11)
- Modern way to represent null pointer
- Safer than NULL macro

---

#### Lines 14-19: Child Search Loop
```cpp
    while (ch) {
      if (ch->firstChar == edgeChar) {
        found = ch;
        break;
      }
      ch = ch->next;
    }
```

**`while (ch)`** - Loop condition
- `ch` is pointer - evaluates to true if not nullptr
- Continues while there are more children to check

**`ch->firstChar`** - Cached first character
- **Design decision:** Store first char for O(1) comparison
- Avoids `ch->node->data[0]` - two dereferences instead of one
- Performance optimization

**`== edgeChar`** - Comparison
- Check if this child matches what we're looking for
- Case-sensitive comparison

**`found = ch`** - Save the match
- Store the matching child pointer

**`break`** - Exit loop early
- No need to check other children once found
- In Radix Tree, only one child can start with each character

**`ch = ch->next`** - Move to next sibling
- Children are stored as linked list
- If no match, continue to next child

---

#### Lines 21-24: Handle No Match
```cpp
    if (!found) {
      handleSearchFailure("No matching child found");
      return false;
    }
```

**`!found`** - Check for null
- `!` negates the pointer check
- True if found is still nullptr

**`handleSearchFailure(...)`** - Error handling
- Centralized error handling function
- Currently silent (for performance)
- Could enable for debugging

**`return false`** - Word not found
- No child matches this character
- Word cannot exist in tree

**Example scenario:**
- Searching for "xyz" but tree only has words starting with "a", "b", "c"
- No child with firstChar == 'x' exists

---

#### Lines 26-27: Calculate Match Length
```cpp
    int matched = matchPrefix(found->node->data, key + keyIndex);
    int nodeLen = strlen(found->node->data);
```

**`matched`** - How many characters matched
- Calls matchPrefix function (explained above)
- **First arg:** Node's data (e.g., "test")
- **Second arg:** Remaining key to match

**`key + keyIndex`** - Pointer arithmetic
- Points to the remaining part of the key
- **Example:** key="testing", keyIndex=2 → "sting"
- Avoids creating new string - just moves pointer

**`nodeLen`** - Length of node data
- Used to check if we matched the entire node
- **Example:** Node data "test" → nodeLen = 4

---

#### Lines 29-32: Check for Partial Mismatch
```cpp
    if (matched < nodeLen) {
      handleSearchFailure("Key ended mid-edge or mismatch");
      return false;
    }
```

**`matched < nodeLen`** - Strict matching check
- If true: Didn't match the entire node label
- This means the key diverges from this path

**When this happens:**
1. **Key too short:** key="te", node="test" → matched=2, nodeLen=4
2. **Mismatch mid-node:** key="teax", node="test" → matched=2 (te), nodeLen=4

**Why strict?**
- search() finds exact words
- Prefix matching is for autocomplete (different function)

---

#### Lines 34-35: Progress to Next Level
```cpp
    keyIndex += matched;
    current = found->node;
```

**`keyIndex += matched`** - Update position
- Move index forward by matched characters
- **Example:** keyIndex=0, matched=4 → keyIndex=4

**`current = found->node`** - Navigate deeper
- Move to the matched child node
- Next iteration will search this node's children

---

#### Lines 38-41: Final Check
```cpp
  if (current->ended)
    return true;
  handleSearchFailure("Key found but not marked as complete word");
  return false;
}
```

**`current->ended`** - Word end marker
- Boolean flag on each node
- True means this node represents a complete word

**`return true`** - Success!
- Word exists in tree

**Why this check is needed:**
- Tree might have "testing" but not "test"
- Path to "test" exists (it's a prefix of "testing")
- But "test" node has ended=false

**Example:**
```
Tree contains: "testing"
Structure: root → "testing" [ended=true]
           
Searching "test":
- Match "test" (first 4 chars of "testing")
- But matched < nodeLen (4 < 7)
- Returns false at line 30

Searching "testing":
- Match full "testing" [ended=true]
- Returns true
```

---

## 3. handleSearchFailure Function

### File Location
`RadixTree.cpp` - Lines 261-263

### Complete Code

```cpp
void RadixTree::handleSearchFailure(const char *reason) {
  // cout << "Search failed: " << reason << endl; // Silent for optimization
}
```

### Line-by-Line Explanation

---

#### Line 1: Function Signature
```cpp
void RadixTree::handleSearchFailure(const char *reason) {
```

**`void`** - Return type
- Function returns nothing
- Side-effect only (logging)

**`const char *reason`** - Parameter
- Description of why search failed
- **Examples:** "No matching child found", "Key ended mid-edge"

---

#### Line 2: Commented Out Code
```cpp
  // cout << "Search failed: " << reason << endl; // Silent for optimization
```

**`//`** - Comment
- Code is disabled but preserved

**`cout << ...`** - Would print to console
- If enabled, shows detailed failure reasons

**Why commented out?**
- **Performance:** cout is slow (I/O operation)
- **User experience:** Too much output for normal use
- **When to enable:** During debugging

---

## 4. getCurrentTimestamp Function

### File Location
`RadixTree.cpp` - Lines 428-431

### Complete Code

```cpp
long long RadixTree::getCurrentTimestamp() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
```

### Line-by-Line Explanation

---

#### Line 1: Function Signature
```cpp
long long RadixTree::getCurrentTimestamp() {
```

**`long long`** - Return type
- 64-bit integer
- **Range:** Very large (prevents overflow)
- **Why needed?** Timestamps in milliseconds since 1970 are big numbers

---

#### Lines 2-4: Getting the Timestamp
```cpp
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
```

**`std::chrono`** - C++ chrono library
- Modern time handling (C++11)
- Type-safe, portable

**`system_clock::now()`** - Get current time
- Returns current time point
- Uses system's clock

**`time_since_epoch()`** - Duration since Jan 1, 1970
- Standard Unix epoch
- Returns duration object

**`duration_cast<milliseconds>`** - Convert to milliseconds
- Different clocks have different precisions
- Explicitly convert to milliseconds

**`.count()`** - Get numeric value
- Extracts the raw number from duration
- Returns long long

**Purpose:** Used for "recent searches" sorting feature

---

## 5. updateWordFrequency Function

### File Location
`RadixTree.cpp` - Lines 439-443

### Complete Code

```cpp
void RadixTree::updateWordFrequency(Node *node) {
  if (node && node->ended) {
    node->frequency++;
    node->timestamp = getCurrentTimestamp();
  }
}
```

### Line-by-Line Explanation

---

#### Line 1: Function Signature
```cpp
void RadixTree::updateWordFrequency(Node *node) {
```

**`void`** - No return value
**`Node *node`** - The node to update

---

#### Lines 2-5: Update Logic
```cpp
  if (node && node->ended) {
    node->frequency++;
    node->timestamp = getCurrentTimestamp();
  }
```

**`node && node->ended`** - Guard conditions
- `node`: Check not null
- `node->ended`: Check it's a valid word
- **Short-circuit:** If node is null, second check not evaluated

**`node->frequency++`** - Increment usage count
- Tracks how often word is used
- Used for sorting suggestions by popularity

**`node->timestamp = getCurrentTimestamp()`** - Update access time
- Record when word was last used
- Used for "recent" sorting option

---

## Complexity Summary

| Function | Time Complexity | Space Complexity |
|----------|-----------------|------------------|
| matchPrefix | O(min(a,b)) | O(1) |
| search | O(m) where m = key length | O(1) |
| handleSearchFailure | O(1) | O(1) |
| getCurrentTimestamp | O(1) | O(1) |
| updateWordFrequency | O(1) | O(1) |

