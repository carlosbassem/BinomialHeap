# Testing Guide: Binomial Heap Type Selection Feature

## Overview
This guide provides step-by-step instructions for testing the new char/int type selection feature once Qt6 is available.

## Prerequisites
- Qt6 installed (qt6-base-dev, qt6-base-dev-tools)
- CMake 3.16 or higher
- C++17 compatible compiler

## Build Instructions
```bash
cd /home/runner/work/BinomialHeap/BinomialHeap
mkdir -p build && cd build
cmake ..
make -j4
./bin/BinomialHeapVisualizer
```

## Test Plan

### Test 1: Type Selection Dialog
**Purpose**: Verify the type selection dialog appears and works correctly

**Steps**:
1. Launch the application
2. Verify a dialog appears with title "Select Heap Type"
3. Verify there are two radio buttons: "Integer (int)" and "Character (char)"
4. Verify "Integer" is selected by default
5. Click "Character", then "OK"
6. Verify the main window opens

**Expected Result**: Dialog appears with correct options and selecting either type opens the main window

---

### Test 2: Integer Mode - Basic Operations
**Purpose**: Verify all operations work correctly with integers

**Steps**:
1. Launch application and select "Integer"
2. Verify window title says "Binomial Heap Visualizer - Integer Type"
3. Insert values: 50, 30, 70, 20, 40, 60, 10
4. Verify all values appear in the visualization
5. Click "Display Min" - verify node with value 10 is highlighted in red
6. Wait 2 seconds - verify highlight disappears
7. Click "Extract Min" - verify message shows "Extracted minimum value: 10"
8. Verify 10 is removed from visualization
9. Click "Display Min" - verify node with value 20 is now highlighted

**Expected Result**: All operations complete successfully with correct visual feedback

---

### Test 3: Integer Mode - Decrease Key
**Purpose**: Verify decrease key operation works with integers

**Steps**:
1. Continue from Test 2 (heap has: 20, 30, 40, 50, 60, 70)
2. In "Decrease Key" section:
   - Old value: 70
   - New value: 15
3. Click "Decrease Key"
4. Verify success message: "Decreased key from 70 to 15"
5. Click "Display Min" - verify 15 is now the minimum

**Expected Result**: Value 70 is successfully decreased to 15 and becomes new minimum

---

### Test 4: Integer Mode - Delete and Clear
**Purpose**: Verify delete and clear operations

**Steps**:
1. Continue from Test 3
2. In "Delete Value" field, enter: 40
3. Click "Delete Value"
4. Verify success message and 40 is removed
5. Right-click on a node in the visualization
6. Select "Delete Node Value" from context menu
7. Verify that node is deleted
8. Click "Clear All"
9. Verify confirmation message and heap is empty

**Expected Result**: All delete operations work correctly

---

### Test 5: Integer Mode - Validation
**Purpose**: Verify input validation for integers

**Steps**:
1. Launch application and select "Integer"
2. Try to insert "abc" - verify error: "Please enter a valid Integer value."
3. Try to insert "12.5" - verify error: "Please enter a valid Integer value."
4. Try to insert "" (empty) - verify error: "Please enter a valid Integer value."
5. Try to insert "-5" - verify it succeeds
6. Try to insert "0" - verify it succeeds
7. Try to insert "999999" - verify it succeeds

**Expected Result**: Invalid inputs rejected with appropriate error messages, valid integers accepted

---

### Test 6: Character Mode - Basic Operations
**Purpose**: Verify all operations work correctly with characters

**Steps**:
1. Launch application and select "Character"
2. Verify window title says "Binomial Heap Visualizer - Character Type"
3. Verify placeholder text says "Enter single character"
4. Insert characters: m, a, z, c, k, b, x
5. Verify all characters appear in the visualization
6. Click "Display Min" - verify node with 'a' is highlighted
7. Click "Extract Min" - verify message shows "Extracted minimum value: a"
8. Verify 'a' is removed from visualization
9. Click "Display Min" - verify 'b' is now highlighted

**Expected Result**: All operations work correctly with character values

---

### Test 7: Character Mode - Decrease Key
**Purpose**: Verify decrease key with character comparison

**Steps**:
1. Continue from Test 6 (heap has: b, c, k, m, x, z)
2. In "Decrease Key" section:
   - Old value: m
   - New value: d
3. Click "Decrease Key"
4. Verify success message: "Decreased key from m to d"
5. Click "Display Min" - verify 'b' is still minimum (b < d)
6. Decrease Key:
   - Old value: d
   - New value: a
7. Verify 'a' is now the minimum

**Expected Result**: Character comparison works alphabetically (ASCII order)

---

### Test 8: Character Mode - Validation
**Purpose**: Verify input validation for characters

**Steps**:
1. Continue in Character mode
2. Try to insert "ab" (two characters) - verify error
3. Try to insert "xyz" (multiple characters) - verify error
4. Try to insert "" (empty) - verify error
5. Try to insert "5" (digit) - verify it succeeds
6. Try to insert "A" (uppercase) - verify it succeeds
7. Try to insert "@" (special char) - verify it succeeds

**Expected Result**: Only single characters accepted, multiple characters or empty input rejected

---

### Test 9: Character Mode - Delete Operations
**Purpose**: Verify delete operations with characters

**Steps**:
1. Build a heap with: f, d, h, b, j
2. Delete 'd' using the Delete Value field
3. Verify success and 'd' is removed
4. Right-click on 'h' in visualization
5. Select "Delete Node Value"
6. Verify 'h' is removed
7. Click "Clear All"
8. Verify heap is empty

**Expected Result**: All delete operations work correctly with characters

---

### Test 10: Visual Verification
**Purpose**: Verify visualization is correct for both types

**Steps**:
1. In Integer mode, create heap with: 8, 4, 12, 2, 6, 10, 14, 1
2. Verify binomial trees are displayed correctly
3. Verify "head" pointer is shown
4. Verify tree orders (B0, B1, B2, B3) are labeled
5. Verify parent-child arrows are double-headed
6. Verify sibling arrows are single-headed
7. Restart and do the same in Character mode with: h, d, l, b, f, j, n, a
8. Verify visualization looks identical (just with chars instead of ints)

**Expected Result**: Visualization correctly displays structure for both types

---

### Test 11: Edge Cases
**Purpose**: Test edge cases and error conditions

**Steps**:
1. Launch in Integer mode
2. Try "Extract Min" on empty heap - verify error
3. Try "Display Min" on empty heap - verify error
4. Insert one value (42)
5. Try to decrease 42 to 50 (invalid - new value is larger) - verify error
6. Try to decrease 99 (non-existent value) - verify error
7. Try to delete 99 (non-existent value) - verify error
8. Delete 42
9. Try "Clear All" on empty heap - verify info message

**Expected Result**: All error conditions handled gracefully with appropriate messages

---

### Test 12: Type Consistency
**Purpose**: Verify type is consistent throughout session

**Steps**:
1. Launch and select "Integer"
2. Insert several integers
3. Verify all operations work with integers
4. Close application
5. Launch again and select "Character"
6. Insert several characters
7. Verify all operations work with characters
8. Verify you cannot accidentally enter integers in char mode or vice versa

**Expected Result**: Each session maintains type consistency based on startup selection

---

## Success Criteria

✅ All 12 tests pass without errors
✅ Type selection dialog works correctly
✅ Integer mode accepts integers and rejects non-integers
✅ Character mode accepts single characters and rejects multi-char strings
✅ All heap operations work correctly in both modes
✅ Visualization updates correctly for both types
✅ Error handling is appropriate and user-friendly
✅ No crashes or unexpected behavior

## Known Limitations

1. **Type Selection**: Can only be done at startup; requires restart to change types
2. **Character Comparison**: Uses ASCII values, so 'A' < 'a' (uppercase before lowercase)
3. **No Type Mixing**: Cannot have both chars and ints in the same heap instance

## Reporting Issues

If any test fails, report with:
- Which test failed
- Steps to reproduce
- Expected vs actual behavior
- Error messages (if any)
- Screenshots of the issue

## Additional Manual Testing

Beyond the structured tests above, try:
- Inserting values in different orders (ascending, descending, random)
- Creating larger heaps (50+ elements)
- Rapidly clicking buttons
- Resizing the window during operations
- Using keyboard shortcuts if any
- Testing with special characters in char mode (!, @, #, etc.)

---

## Summary

This comprehensive test plan covers:
- ✅ Type selection functionality
- ✅ Integer mode operations
- ✅ Character mode operations
- ✅ Input validation for both types
- ✅ Visual rendering and updates
- ✅ Edge cases and error handling
- ✅ User experience consistency

Execute all tests to ensure the implementation meets requirements and provides a robust user experience.
