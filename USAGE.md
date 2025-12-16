# Binomial Heap Visualizer - Usage Guide

## Overview
The Binomial Heap Visualizer provides an interactive graphical interface for visualizing and manipulating a Min Binomial Heap data structure.

## Interface Layout

The application window consists of two main areas:

1. **Visualization Area** (Left): Displays the binomial heap structure
   - Nodes are shown as light blue circles with values inside
   - Parent-child relationships shown with double-headed arrows (↔)
   - Sibling relationships shown with single-headed arrows (→)
   - Multiple binomial trees arranged horizontally

2. **Control Panel** (Right): Contains operation buttons and input fields

## Operations

### Insert Value
**Purpose**: Add a new integer value to the heap

**Steps**:
1. Enter an integer value in the "Insert Value" field
2. Click the "Insert" button
3. A success message will appear
4. The visualization updates automatically

**Example**: Insert values 15, 8, 25, 3 to build a heap

### Extract Min
**Purpose**: Remove and return the minimum value from the heap

**Steps**:
1. Click the "Extract Min" button
2. A message shows the extracted minimum value
3. The heap restructures and visualization updates

**Note**: If the heap is empty, an error message appears

### Display Min
**Purpose**: Highlight the minimum value node without removing it

**Steps**:
1. Click the "Display Min" button
2. A message shows the minimum value
3. The corresponding node is highlighted with a **thick red border**
4. The highlight lasts for **2 seconds** before returning to normal

**Visual**: The minimum node's border changes from black (2px) to red (4px)

### Decrease Key
**Purpose**: Reduce the value of an existing node in the heap

**Steps**:
1. Enter the current value in "Old value" field
2. Enter the new smaller value in "New value" field
3. Click the "Decrease Key" button
4. The heap restructures to maintain the min-heap property
5. Visualization updates showing the new structure

**Requirements**:
- The old value must exist in the heap
- The new value must be smaller than the old value
- Violating these shows an error message

**Example**: If heap contains 15, decrease it to 5

### Delete Value
**Purpose**: Remove a specific value from the heap

**Steps**:
1. Enter the value to delete in the "Value to delete" field
2. Click the "Delete Value" button
3. The value is removed and heap restructures
4. Visualization updates

**Alternative**: Right-click any node to delete it via context menu

**Note**: If the value doesn't exist, an error message appears

### Right-Click Context Menu
**Purpose**: Quick node deletion

**Steps**:
1. Right-click on any node in the visualization
2. Select "Delete Node Value" from the context menu
3. The node is deleted immediately
4. Heap restructures and updates

### Clear All
**Purpose**: Empty the entire heap

**Steps**:
1. Click the "Clear All" button
2. All nodes are removed
3. Visualization shows empty canvas
4. Success message confirms the operation

## Understanding the Visualization

### Node Colors
- **Light Blue** (RGB: 173, 216, 230): Normal node state
- **Light Blue with Red Border**: Minimum node when "Display Min" is active

### Arrows
- **Double-Headed Arrows (↔)**: Indicate parent-child relationships
  - Arrow connects parent node to its child node
  - Represents the tree structure within each binomial tree

- **Single-Headed Arrows (→)**: Indicate sibling relationships
  - Arrow connects root nodes of different binomial trees
  - Shows the linked list of tree roots in the heap

### Layout
- **Horizontal Arrangement**: Multiple binomial trees are arranged side by side
- **Vertical Hierarchy**: Parent nodes are above their children
- **Spacing**: Automatic spacing ensures nodes don't overlap

### Binomial Trees
A binomial heap consists of a collection of binomial trees:
- **B₀**: Single node
- **B₁**: 2 nodes (1 root, 1 child)
- **B₂**: 4 nodes (1 root, 3 children in tree structure)
- **Bₖ**: 2ᵏ nodes

After inserting several values, you'll see multiple binomial trees of different orders.

## Example Workflow

### Building a Heap
```
1. Insert 15 → Shows single node (B₀)
2. Insert 8  → Shows tree with 2 nodes (B₁)
3. Insert 25 → Shows B₀ and B₁
4. Insert 3  → Shows single B₂ tree with 4 nodes
5. Insert 12 → Shows B₀ and B₂
6. Insert 20 → Shows B₁ and B₂
```

### Finding and Extracting Minimum
```
1. Click "Display Min" → Node with value 3 gets red border
2. Wait 2 seconds → Border returns to black
3. Click "Extract Min" → Value 3 is removed
4. Click "Display Min" → Next minimum is highlighted
```

### Modifying Values
```
1. Insert values: 50, 30, 40, 20
2. Decrease Key: old=50, new=10
3. Display Min → Shows 10 is now minimum
4. Delete Value: 30 → Removes 30 from heap
```

## Tips

1. **Start Simple**: Begin with 4-5 values to understand the structure
2. **Watch Reorganization**: Insert powers of 2 (2, 4, 8, 16) to see tree merging
3. **Use Display Min**: Frequently check minimum to understand heap property
4. **Try Different Orders**: Insert ascending, descending, and random values
5. **Context Menu**: Right-click is faster for deleting visible nodes

## Common Error Messages

- **"Please enter a valid integer value"**: Input field contains non-integer data
- **"Cannot get min from empty heap"**: Attempted operation on empty heap
- **"Key not found in heap"**: Specified value doesn't exist for decrease/delete
- **"New value must be smaller than current value"**: Decrease key constraint violated
- **"Heap is already empty"**: Attempted to clear an already empty heap

## Technical Notes

- The heap maintains the **min-heap property**: parent ≤ children
- Operations maintain **O(log n)** time complexity
- Visualization updates occur after each successful operation
- The implementation follows standard binomial heap algorithms
- All operations are exception-safe with proper error handling
