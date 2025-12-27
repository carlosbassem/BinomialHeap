# Duplicate Root Tree Feature

## Overview
This feature allows users to duplicate an entire binomial tree (rooted at a root node) and merge it back into the heap with a simple left-click interaction.

## User Instructions

### How to Use
1. **Build the heap**: Insert values using the "Insert Value" field
2. **Identify root nodes**: Root nodes are the top-level nodes in each binomial tree, labeled with "B0", "B1", "B2", etc.
3. **Left-click on a root node**: Click on any root node in the visualization
4. **Select "Duplicate Root Tree"**: A context menu will appear with this option
5. **Observe the result**: The entire tree is duplicated and merged back into the heap

### Visual Cues
- **Root Nodes**: Top-level nodes of each binomial tree (horizontally aligned)
- **Non-Root Nodes**: Children within a tree (vertically below their parent)
- **"B0", "B1", "B2" labels**: Indicate binomial tree order (appear above root nodes)

## Technical Details

### Implementation
The feature leverages existing binomial heap operations:

1. **Copy Operation**: Uses `copyHeap(node, parent)` to create a deep copy of the entire subtree
2. **Union Operation**: Uses `unionHeap(heapA, heapB)` to merge the duplicate back into the heap
3. **Root Detection**: `isRootNode(node)` checks if a clicked node is in the root list

### Code Changes

#### Backend (binomial_heap.hpp & binomial_heap_implementation.cpp)
```cpp
void duplicateAndMergeRootTree(B rootValue);
```
- Finds the root node with the specified value
- Creates a copy using `copyHeap()`
- Merges it back using `unionHeap()`
- Throws exception if value is not a root node

#### Frontend (HeapVisualizer.h)
```cpp
void mousePressEvent(QMouseEvent* event) override;
bool isRootNode(BinomialNode<T>* node);
virtual void onRootNodeLeftClicked(T value) = 0;
```
- Handles left-click events
- Shows context menu only for root nodes
- Emits signal when user selects duplicate option

#### UI Integration (MainWindow.h & MainWindow.cpp)
```cpp
void onRootNodeLeftClicked(T value) override;
```
- Connected to visualizer signal
- Calls `duplicateAndMergeRootTree()`
- Updates visualization
- Shows success/error messages

## Example Scenario

### Initial State
```
Heap with 4 nodes:
B2
3
  ├─ 5
  │   └─ 10
  └─ 8
```

### After Duplicating Root Node [3]
```
Heap with 8 nodes:
B3
3
  ├─ 3 (duplicate)
  │   ├─ 5 (duplicate)
  │   │   └─ 10 (duplicate)
  │   └─ 8 (duplicate)
  ├─ 5 (original)
  │   └─ 10 (original)
  └─ 8 (original)
```

**Result**: Heap size doubles from 4 to 8 nodes

## Key Features
- ✅ Only works on root nodes (enforced by `isRootNode()` check)
- ✅ Uses existing copy constructor and union logic
- ✅ Maintains all binomial heap properties
- ✅ Works for both `int` and `char` heap types
- ✅ Right-click delete functionality remains independent
- ✅ User-friendly context menu interface
- ✅ Automatic visualization updates
- ✅ Success/error feedback messages

## Testing

### Manual Testing
1. Insert values: 5, 10, 3, 8
2. Left-click on root node [3]
3. Select "Duplicate Root Tree"
4. Verify heap size increases from 4 to 8
5. Verify visualization shows merged structure
6. Try left-clicking non-root nodes (no menu should appear)
7. Try right-clicking any node (delete menu should appear)

### Automated Testing
See `/tmp/test_duplicate_feature.cpp` for backend logic tests:
```bash
cd /tmp
g++ -std=c++17 -I/path/to/BinomialHeap test_duplicate_feature.cpp \
    /path/to/binomial_heap_implementation.cpp -o test_duplicate
./test_duplicate
```

## Error Handling
- **Non-root node value**: Throws exception "Root node with specified value not found"
- **Empty heap**: No nodes to click
- **Invalid operation**: Standard heap exceptions apply

## Compatibility
- **Qt Version**: Qt6 (tested with 6.4.2)
- **C++ Standard**: C++17
- **Heap Types**: int, char (template-based, easily extensible)
- **Platform**: Linux, macOS, Windows (Qt supported platforms)
