# Building the Binomial Heap Visualizer

## Prerequisites

### Qt6 Requirements
- Qt6 Base Development Files
- Qt6 Widgets Module
- CMake (version 3.16 or higher)
- C++17 compatible compiler

### Installing Dependencies

#### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y qt6-base-dev qt6-base-dev-tools cmake build-essential
```

#### Fedora/RHEL:
```bash
sudo dnf install qt6-qtbase-devel cmake gcc-c++
```

#### macOS (with Homebrew):
```bash
brew install qt@6 cmake
```

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/carlosbassem/BinomialHeap.git
cd BinomialHeap
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
make -j4
```

5. Run the application:
```bash
./bin/BinomialHeapVisualizer
```

## Project Structure

```
BinomialHeap/
├── binomial_heap.hpp              # Binomial heap template class declaration
├── binomial_heap_implementation.cpp # Binomial heap implementation
├── HeapVisualizer.h               # Visualization widget header
├── HeapVisualizer.cpp             # Visualization widget implementation
├── MainWindow.h                   # Main application window header
├── MainWindow.cpp                 # Main application window implementation
├── main.cpp                       # Application entry point
├── CMakeLists.txt                 # CMake build configuration
└── BUILD.md                       # This file
```

## Features

### Visualization
- **Node Representation**: Light blue circles with integer values
- **Parent-Child Relationships**: Double-headed black arrows
- **Sibling Relationships**: Single-headed arrows between tree roots
- **Layout Algorithm**: Automatic hierarchical positioning
- **Highlighting**: Red border on minimum node (2-second duration)

### Operations
- **Insert**: Add new values to the heap
- **Extract Min**: Remove and return the minimum value
- **Display Min**: Highlight the minimum value node
- **Decrease Key**: Reduce a value in the heap
- **Delete Value**: Remove a specific value
- **Clear All**: Empty the entire heap
- **Right-Click Menu**: Delete node by right-clicking

## Usage Example

1. Launch the application
2. Enter a value in the "Insert Value" field (e.g., 15)
3. Click "Insert" button
4. Repeat to add more values
5. Click "Display Min" to highlight the minimum value node
6. Right-click any node to delete it via context menu
7. Use other operations as needed

## Troubleshooting

### Qt6 not found
If CMake cannot find Qt6, you may need to set the Qt6 path:
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt6 ..
```

### Build errors
Ensure you have Qt6 (not Qt5) installed:
```bash
qmake --version  # Should show Qt version 6.x
```

### Runtime issues
Check that Qt libraries are in your library path:
```bash
export LD_LIBRARY_PATH=/path/to/qt6/lib:$LD_LIBRARY_PATH
```

## Development

### Adding New Features
The code is organized into three main components:
1. **Heap Logic** (`binomial_heap.hpp`, `binomial_heap_implementation.cpp`)
2. **Visualization** (`HeapVisualizer.h`, `HeapVisualizer.cpp`)
3. **UI Controls** (`MainWindow.h`, `MainWindow.cpp`)

### Modifying Visualization
To change node colors, edit `NodeItem::NodeItem()` in `HeapVisualizer.cpp`:
```cpp
setBrush(QBrush(QColor(173, 216, 230))); // Change RGB values
```

To adjust layout spacing, modify constants in `HeapVisualizer::calculateLayout()`:
```cpp
qreal childY = y + 120;  // Vertical spacing
currentX += 100;          // Horizontal spacing between trees
```

## License

See the main repository for license information.
