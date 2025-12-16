#include "MainWindow.h"
#include <QSplitter>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), heap(nullptr) {
    
    heap = new BinomialHeap<int>();
    setupUI();
    setWindowTitle("Binomial Heap Visualizer");
    resize(1400, 800);
}

MainWindow::~MainWindow() {
    delete heap;
}

void MainWindow::setupUI() {
    // Central widget with splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create visualizer
    visualizer = new HeapVisualizer(this);
    visualizer->setHeap(heap);
    
    // Create control panel
    QWidget* controlPanel = createControlPanel();
    
    // Add widgets to splitter
    splitter->addWidget(visualizer);
    splitter->addWidget(controlPanel);
    
    // Set splitter proportions (visualizer takes more space)
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    setCentralWidget(splitter);
    
    // Connect signal from visualizer
    connect(visualizer, &HeapVisualizer::nodeRightClicked,
            this, &MainWindow::onNodeRightClicked);
}

QWidget* MainWindow::createControlPanel() {
    QWidget* panel = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    
    // Group box for controls
    QGroupBox* controlGroup = new QGroupBox("Heap Operations", panel);
    QVBoxLayout* groupLayout = new QVBoxLayout(controlGroup);
    
    // Insert operation
    QLabel* insertLabel = new QLabel("Insert Value:", controlGroup);
    insertValueEdit = new QLineEdit(controlGroup);
    insertValueEdit->setPlaceholderText("Enter integer value");
    insertButton = new QPushButton("Insert", controlGroup);
    connect(insertButton, &QPushButton::clicked, this, &MainWindow::onInsert);
    
    groupLayout->addWidget(insertLabel);
    groupLayout->addWidget(insertValueEdit);
    groupLayout->addWidget(insertButton);
    groupLayout->addSpacing(10);
    
    // Extract Min operation
    extractMinButton = new QPushButton("Extract Min", controlGroup);
    connect(extractMinButton, &QPushButton::clicked, this, &MainWindow::onExtractMin);
    groupLayout->addWidget(extractMinButton);
    groupLayout->addSpacing(10);
    
    // Display Min operation
    displayMinButton = new QPushButton("Display Min", controlGroup);
    connect(displayMinButton, &QPushButton::clicked, this, &MainWindow::onDisplayMin);
    groupLayout->addWidget(displayMinButton);
    groupLayout->addSpacing(10);
    
    // Decrease Key operation
    QLabel* decreaseLabel = new QLabel("Decrease Key:", controlGroup);
    decreaseOldValueEdit = new QLineEdit(controlGroup);
    decreaseOldValueEdit->setPlaceholderText("Old value");
    decreaseNewValueEdit = new QLineEdit(controlGroup);
    decreaseNewValueEdit->setPlaceholderText("New value (smaller)");
    decreaseKeyButton = new QPushButton("Decrease Key", controlGroup);
    connect(decreaseKeyButton, &QPushButton::clicked, this, &MainWindow::onDecreaseKey);
    
    groupLayout->addWidget(decreaseLabel);
    groupLayout->addWidget(decreaseOldValueEdit);
    groupLayout->addWidget(decreaseNewValueEdit);
    groupLayout->addWidget(decreaseKeyButton);
    groupLayout->addSpacing(10);
    
    // Delete Value operation
    QLabel* deleteLabel = new QLabel("Delete Value:", controlGroup);
    deleteValueEdit = new QLineEdit(controlGroup);
    deleteValueEdit->setPlaceholderText("Value to delete");
    deleteValueButton = new QPushButton("Delete Value", controlGroup);
    connect(deleteValueButton, &QPushButton::clicked, this, &MainWindow::onDeleteValue);
    
    groupLayout->addWidget(deleteLabel);
    groupLayout->addWidget(deleteValueEdit);
    groupLayout->addWidget(deleteValueButton);
    groupLayout->addSpacing(10);
    
    // Clear Heap operation
    clearButton = new QPushButton("Clear All", controlGroup);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearHeap);
    groupLayout->addWidget(clearButton);
    
    // Add stretch to push everything to top
    groupLayout->addStretch();
    
    mainLayout->addWidget(controlGroup);
    
    return panel;
}

void MainWindow::onInsert() {
    bool ok;
    int value = insertValueEdit->text().toInt(&ok);
    
    if (!ok) {
        showMessage("Error", "Please enter a valid integer value.", true);
        return;
    }
    
    try {
        heap->insert(value);
        visualizer->updateVisualization(true);
        insertValueEdit->clear();
        showMessage("Success", QString("Inserted value: %1").arg(value));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onExtractMin() {
    try {
        int minValue = heap->extractMin();
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Extracted minimum value: %1").arg(minValue));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDisplayMin() {
    try {
        int minValue = heap->getMin();
        visualizer->highlightMinNode();
        showMessage("Minimum Value", QString("Minimum value is: %1\n(Highlighted in red for 2 seconds)").arg(minValue));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDecreaseKey() {
    bool ok1, ok2;
    int oldValue = decreaseOldValueEdit->text().toInt(&ok1);
    int newValue = decreaseNewValueEdit->text().toInt(&ok2);
    
    if (!ok1 || !ok2) {
        showMessage("Error", "Please enter valid integer values.", true);
        return;
    }
    
    try {
        heap->decreaseKey(oldValue, newValue);
        visualizer->updateVisualization(true);
        decreaseOldValueEdit->clear();
        decreaseNewValueEdit->clear();
        showMessage("Success", QString("Decreased key from %1 to %2").arg(oldValue).arg(newValue));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDeleteValue() {
    bool ok;
    int value = deleteValueEdit->text().toInt(&ok);
    
    if (!ok) {
        showMessage("Error", "Please enter a valid integer value.", true);
        return;
    }
    
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        deleteValueEdit->clear();
        showMessage("Success", QString("Deleted value: %1").arg(value));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onClearHeap() {
    if (heap->isEmpty()) {
        showMessage("Information", "Heap is already empty.");
        return;
    }
    
    heap->clear();
    visualizer->updateVisualization(false);
    showMessage("Success", "Heap cleared successfully!");
}

void MainWindow::onNodeRightClicked(int value) {
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Deleted node with value: %1").arg(value));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::showMessage(const QString& title, const QString& message, bool isError) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    
    if (isError) {
        msgBox.setIcon(QMessageBox::Critical);
    } else {
        msgBox.setIcon(QMessageBox::Information);
    }
    
    msgBox.exec();
}
