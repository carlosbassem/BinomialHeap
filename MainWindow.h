#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "HeapVisualizer.h"
#include "binomial_heap.hpp"
#include "ValueConverter.h"

template <typename T>
class MainWindowT : public QMainWindow {
    
public:
    explicit MainWindowT(QWidget* parent = nullptr);
    virtual ~MainWindowT();
    
protected:
    // Virtual slot-like methods for derived classes to override
    virtual void onInsert() = 0;
    virtual void onExtractMin() = 0;
    virtual void onDisplayMin() = 0;
    virtual void onDecreaseKey() = 0;
    virtual void onDeleteValue() = 0;
    virtual void onClearHeap() = 0;
    virtual void onNodeRightClicked(T value) = 0;
    
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

// Non-template wrapper classes with Q_OBJECT for Qt MOC
class MainWindow : public MainWindowT<int> {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    
private slots:
    void onInsert() override;
    void onExtractMin() override;
    void onDisplayMin() override;
    void onDecreaseKey() override;
    void onDeleteValue() override;
    void onClearHeap() override;
    void onNodeRightClicked(int value) override;
};

class MainWindowChar : public MainWindowT<char> {
    Q_OBJECT
    
public:
    explicit MainWindowChar(QWidget* parent = nullptr);
    
private slots:
    void onInsert() override;
    void onExtractMin() override;
    void onDisplayMin() override;
    void onDecreaseKey() override;
    void onDeleteValue() override;
    void onClearHeap() override;
    void onNodeRightClicked(char value) override;
};

// Template implementation

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
    
    // Create visualizer based on type
    if constexpr (std::is_same_v<T, int>) {
        visualizer = new HeapVisualizer(this);
    } else if constexpr (std::is_same_v<T, char>) {
        visualizer = new HeapVisualizerChar(this);
    }
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
    
    // Note: Connections are made in the derived classes since they have proper signals/slots
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
    
    groupLayout->addWidget(insertLabel);
    groupLayout->addWidget(insertValueEdit);
    groupLayout->addWidget(insertButton);
    groupLayout->addSpacing(10);
    
    // Extract Min operation
    extractMinButton = new QPushButton("Extract Min", controlGroup);
    groupLayout->addWidget(extractMinButton);
    groupLayout->addSpacing(10);
    
    // Display Min operation
    displayMinButton = new QPushButton("Display Min", controlGroup);
    groupLayout->addWidget(displayMinButton);
    groupLayout->addSpacing(10);
    
    // Decrease Key operation
    QLabel* decreaseLabel = new QLabel("Decrease Key:", controlGroup);
    decreaseOldValueEdit = new QLineEdit(controlGroup);
    decreaseOldValueEdit->setPlaceholderText("Old value");
    decreaseNewValueEdit = new QLineEdit(controlGroup);
    decreaseNewValueEdit->setPlaceholderText("New value (smaller)");
    decreaseKeyButton = new QPushButton("Decrease Key", controlGroup);
    
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
    
    groupLayout->addWidget(deleteLabel);
    groupLayout->addWidget(deleteValueEdit);
    groupLayout->addWidget(deleteValueButton);
    groupLayout->addSpacing(10);
    
    // Clear Heap operation
    clearButton = new QPushButton("Clear All", controlGroup);
    groupLayout->addWidget(clearButton);
    
    // Add stretch to push everything to top
    groupLayout->addStretch();
    
    mainLayout->addWidget(controlGroup);
    
    return panel;
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

#endif // MAINWINDOW_H
