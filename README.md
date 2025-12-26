# BinomialHeap

A Qt6-based interactive visualization tool for Binomial Heaps that supports both **integer** and **character** data types.

## Features

- 🔢 **Dual Type Support**: Choose between integer or character data types at startup
- 🎨 **Interactive Visualization**: Real-time graphical representation of binomial heap structure
- ⚡ **Complete Operations**: Insert, Extract Min, Decrease Key, Delete, and more
- 🖱️ **Context Menu**: Right-click any node to delete it
- 🔴 **Minimum Highlighting**: Visual indication of the minimum value node

## Quick Start

1. **Launch the application**
2. **Select your data type** (Integer or Character) in the startup dialog
3. **Start building your heap** using the control panel operations

For detailed usage instructions, see [USAGE.md](USAGE.md).

For build instructions, see [BUILD.md](BUILD.md).

## Type Selection

When you start the application, you'll be prompted to choose between:
- **Integer (int)**: For numeric values
- **Character (char)**: For single characters

The selected type determines what kind of data you can insert and manipulate in the heap.

## Operations

- **Insert**: Add new values to the heap
- **Extract Min**: Remove and return the smallest value
- **Display Min**: Highlight the minimum value (2-second highlight)
- **Decrease Key**: Reduce a value in the heap
- **Delete Value**: Remove a specific value
- **Clear All**: Empty the entire heap

## Technology Stack

- **C++17**: Core implementation
- **Qt6**: GUI framework (Widgets module)
- **CMake**: Build system
- **Templates**: Generic programming for type flexibility

## Project Structure

- `binomial_heap.hpp/cpp`: Core binomial heap data structure (template-based)
- `MainWindow.h/cpp`: Main window with type-agnostic UI supporting both int and char types
- `HeapVisualizer.h/cpp`: Templated visualization widget for rendering the heap
- `TypeSelectionDialog`: Startup type selection dialog (defined in MainWindow.h)
- `ValueConverter.h`: Type conversion utilities for int and char types
- `main.cpp`: Application entry point with type selection

## License

See repository for license information.