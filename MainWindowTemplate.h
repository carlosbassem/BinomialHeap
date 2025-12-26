#ifndef MAINWINDOW_TEMPLATE_H
#define MAINWINDOW_TEMPLATE_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QSplitter>
#include "HeapVisualizerTemplate.h"
#include "binomial_heap.hpp"
#include "ValueConverter.h"

template <typename T>
class MainWindowT : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindowT(QWidget* parent = nullptr);
    ~MainWindowT();
    
private slots:
    // Heap operations
    void onInsert();
    void onExtractMin();
    void onDisplayMin();
    void onDecreaseKey();
    void onDeleteValue();
    void onClearHeap();
    void onNodeRightClicked(T value);
    
private:
    // UI Components
    HeapVisualizerT<T>* visualizer;
    
    // Control panel widgets
    QLineEdit* insertValueEdit;
    QLineEdit* decreaseOldValueEdit;
    QLineEdit* decreaseNewValueEdit;
    QLineEdit* deleteValueEdit;
    
    QPushButton* insertButton;
    QPushButton* extractMinButton;
    QPushButton* displayMinButton;
    QPushButton* decreaseKeyButton;
    QPushButton* deleteValueButton;
    QPushButton* clearButton;
    
    // Data
    BinomialHeap<T>* heap;
    
    // UI Setup
    void setupUI();
    QWidget* createControlPanel();
    void showMessage(const QString& title, const QString& message, bool isError = false);
};

// Implementation

template <typename T>
MainWindowT<T>::MainWindowT(QWidget* parent)
    : QMainWindow(parent), heap(nullptr) {
    
    heap = new BinomialHeap<T>();
    setupUI();
    QString typeName = ValueConverter<T>::getTypeName();
    setWindowTitle(QString("Binomial Heap Visualizer - %1 Type").arg(typeName));
    resize(1400, 800);
}

template <typename T>
MainWindowT<T>::~MainWindowT() {
    delete heap;
}

template <typename T>
void MainWindowT<T>::setupUI() {
    // Central widget with splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create visualizer
    visualizer = new HeapVisualizerT<T>(this);
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
    QObject::connect(visualizer, &HeapVisualizerT<T>::nodeRightClicked,
            this, &MainWindowT<T>::onNodeRightClicked);
}

template <typename T>
QWidget* MainWindowT<T>::createControlPanel() {
    QWidget* panel = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(panel);
    
    // Group box for controls
    QString typeName = ValueConverter<T>::getTypeName();
    QGroupBox* controlGroup = new QGroupBox(QString("Heap Operations (%1)").arg(typeName), panel);
    QVBoxLayout* groupLayout = new QVBoxLayout(controlGroup);
    
    // Insert operation
    QLabel* insertLabel = new QLabel("Insert Value:", controlGroup);
    insertValueEdit = new QLineEdit(controlGroup);
    insertValueEdit->setPlaceholderText(ValueConverter<T>::getPlaceholder());
    insertButton = new QPushButton("Insert", controlGroup);
    QObject::connect(insertButton, &QPushButton::clicked, this, &MainWindowT<T>::onInsert);
    
    groupLayout->addWidget(insertLabel);
    groupLayout->addWidget(insertValueEdit);
    groupLayout->addWidget(insertButton);
    groupLayout->addSpacing(10);
    
    // Extract Min operation
    extractMinButton = new QPushButton("Extract Min", controlGroup);
    QObject::connect(extractMinButton, &QPushButton::clicked, this, &MainWindowT<T>::onExtractMin);
    groupLayout->addWidget(extractMinButton);
    groupLayout->addSpacing(10);
    
    // Display Min operation
    displayMinButton = new QPushButton("Display Min", controlGroup);
    QObject::connect(displayMinButton, &QPushButton::clicked, this, &MainWindowT<T>::onDisplayMin);
    groupLayout->addWidget(displayMinButton);
    groupLayout->addSpacing(10);
    
    // Decrease Key operation
    QLabel* decreaseLabel = new QLabel("Decrease Key:", controlGroup);
    decreaseOldValueEdit = new QLineEdit(controlGroup);
    decreaseOldValueEdit->setPlaceholderText("Old value");
    decreaseNewValueEdit = new QLineEdit(controlGroup);
    decreaseNewValueEdit->setPlaceholderText("New value (smaller)");
    decreaseKeyButton = new QPushButton("Decrease Key", controlGroup);
    QObject::connect(decreaseKeyButton, &QPushButton::clicked, this, &MainWindowT<T>::onDecreaseKey);
    
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
    QObject::connect(deleteValueButton, &QPushButton::clicked, this, &MainWindowT<T>::onDeleteValue);
    
    groupLayout->addWidget(deleteLabel);
    groupLayout->addWidget(deleteValueEdit);
    groupLayout->addWidget(deleteValueButton);
    groupLayout->addSpacing(10);
    
    // Clear Heap operation
    clearButton = new QPushButton("Clear All", controlGroup);
    QObject::connect(clearButton, &QPushButton::clicked, this, &MainWindowT<T>::onClearHeap);
    groupLayout->addWidget(clearButton);
    
    // Add stretch to push everything to top
    groupLayout->addStretch();
    
    mainLayout->addWidget(controlGroup);
    
    return panel;
}

template <typename T>
void MainWindowT<T>::onInsert() {
    bool ok;
    T value = ValueConverter<T>::fromString(insertValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<T>::getTypeName()), true);
        return;
    }
    
    try {
        heap->insert(value);
        visualizer->updateVisualization(true);
        insertValueEdit->clear();
        showMessage("Success", QString("Inserted value: %1").arg(ValueConverter<T>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::onExtractMin() {
    try {
        T minValue = heap->extractMin();
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Extracted minimum value: %1").arg(ValueConverter<T>::toString(minValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::onDisplayMin() {
    try {
        T minValue = heap->getMin();
        visualizer->highlightMinNode();
        int duration = HeapVisualizerT<T>::HIGHLIGHT_DURATION_MS / 1000;
        showMessage("Minimum Value", 
                   QString("Minimum value is: %1\n(Highlighted in red for %2 seconds)")
                   .arg(ValueConverter<T>::toString(minValue)).arg(duration));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::onDecreaseKey() {
    bool ok1, ok2;
    T oldValue = ValueConverter<T>::fromString(decreaseOldValueEdit->text(), &ok1);
    T newValue = ValueConverter<T>::fromString(decreaseNewValueEdit->text(), &ok2);
    
    if (!ok1 || !ok2) {
        showMessage("Error", QString("Please enter valid %1 values.").arg(ValueConverter<T>::getTypeName()), true);
        return;
    }
    
    try {
        heap->decreaseKey(oldValue, newValue);
        visualizer->updateVisualization(true);
        decreaseOldValueEdit->clear();
        decreaseNewValueEdit->clear();
        showMessage("Success", QString("Decreased key from %1 to %2")
                   .arg(ValueConverter<T>::toString(oldValue))
                   .arg(ValueConverter<T>::toString(newValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::onDeleteValue() {
    bool ok;
    T value = ValueConverter<T>::fromString(deleteValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<T>::getTypeName()), true);
        return;
    }
    
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        deleteValueEdit->clear();
        showMessage("Success", QString("Deleted value: %1").arg(ValueConverter<T>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::onClearHeap() {
    if (heap->isEmpty()) {
        showMessage("Information", "Heap is already empty.");
        return;
    }
    
    heap->clear();
    visualizer->updateVisualization(false);
    showMessage("Success", "Heap cleared successfully!");
}

template <typename T>
void MainWindowT<T>::onNodeRightClicked(T value) {
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Deleted node with value: %1").arg(ValueConverter<T>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

template <typename T>
void MainWindowT<T>::showMessage(const QString& title, const QString& message, bool isError) {
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

#endif // MAINWINDOW_TEMPLATE_H
