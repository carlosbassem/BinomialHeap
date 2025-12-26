#include "MainWindow.h"
#include <QSplitter>
#include <QDebug>
#include <QFont>

// TypeSelectionDialog implementation
TypeSelectionDialog::TypeSelectionDialog(QWidget* parent)
    : QDialog(parent), selectedType(INTEGER) {
    
    setWindowTitle("Select Heap Type");
    setModal(true);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title label
    QLabel* titleLabel = new QLabel("Choose the type for your Binomial Heap:", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    
    mainLayout->addSpacing(20);
    
    // Radio buttons
    intRadio = new QRadioButton("Integer (int)", this);
    charRadio = new QRadioButton("Character (char)", this);
    
    // Set default selection
    intRadio->setChecked(true);
    
    // Group the radio buttons
    QButtonGroup* typeGroup = new QButtonGroup(this);
    typeGroup->addButton(intRadio);
    typeGroup->addButton(charRadio);
    
    mainLayout->addWidget(intRadio);
    mainLayout->addWidget(charRadio);
    
    mainLayout->addSpacing(20);
    
    // OK button
    QPushButton* okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &TypeSelectionDialog::onOkClicked);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    setMinimumWidth(300);
}

void TypeSelectionDialog::onOkClicked() {
    if (intRadio->isChecked()) {
        selectedType = INTEGER;
    } else {
        selectedType = CHARACTER;
    }
    accept();
}

// MainWindow (int specialization) implementation
MainWindow::MainWindow(QWidget* parent) : MainWindowT<int>(parent) {
    // Connect signals
    QObject::connect(insertButton, &QPushButton::clicked, this, &MainWindow::onInsert);
    QObject::connect(extractMinButton, &QPushButton::clicked, this, &MainWindow::onExtractMin);
    QObject::connect(displayMinButton, &QPushButton::clicked, this, &MainWindow::onDisplayMin);
    QObject::connect(decreaseKeyButton, &QPushButton::clicked, this, &MainWindow::onDecreaseKey);
    QObject::connect(deleteValueButton, &QPushButton::clicked, this, &MainWindow::onDeleteValue);
    QObject::connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearHeap);
    
    // Connect visualizer signal - cast to correct type
    HeapVisualizer* typedVisualizer = static_cast<HeapVisualizer*>(visualizer);
    QObject::connect(typedVisualizer, &HeapVisualizer::nodeRightClicked,
            this, &MainWindow::onNodeRightClicked);
}

void MainWindow::onInsert() {
    bool ok;
    int value = ValueConverter<int>::fromString(insertValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<int>::getTypeName()), true);
        return;
    }
    
    try {
        heap->insert(value);
        visualizer->updateVisualization(true);
        insertValueEdit->clear();
        showMessage("Success", QString("Inserted value: %1").arg(ValueConverter<int>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onExtractMin() {
    try {
        int minValue = heap->extractMin();
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Extracted minimum value: %1").arg(ValueConverter<int>::toString(minValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDisplayMin() {
    try {
        int minValue = heap->getMin();
        visualizer->highlightMinNode();
        int duration = HeapVisualizerT<int>::HIGHLIGHT_DURATION_MS / 1000;
        showMessage("Minimum Value", 
                   QString("Minimum value is: %1\n(Highlighted in red for %2 seconds)")
                   .arg(ValueConverter<int>::toString(minValue)).arg(duration));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDecreaseKey() {
    bool ok1, ok2;
    int oldValue = ValueConverter<int>::fromString(decreaseOldValueEdit->text(), &ok1);
    int newValue = ValueConverter<int>::fromString(decreaseNewValueEdit->text(), &ok2);
    
    if (!ok1 || !ok2) {
        showMessage("Error", QString("Please enter valid %1 values.").arg(ValueConverter<int>::getTypeName()), true);
        return;
    }
    
    try {
        heap->decreaseKey(oldValue, newValue);
        visualizer->updateVisualization(true);
        decreaseOldValueEdit->clear();
        decreaseNewValueEdit->clear();
        showMessage("Success", QString("Decreased key from %1 to %2")
                   .arg(ValueConverter<int>::toString(oldValue))
                   .arg(ValueConverter<int>::toString(newValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDeleteValue() {
    bool ok;
    int value = ValueConverter<int>::fromString(deleteValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<int>::getTypeName()), true);
        return;
    }
    
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        deleteValueEdit->clear();
        showMessage("Success", QString("Deleted value: %1").arg(ValueConverter<int>::toString(value)));
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
        showMessage("Success", QString("Deleted node with value: %1").arg(ValueConverter<int>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

// MainWindowChar (char specialization) implementation
MainWindowChar::MainWindowChar(QWidget* parent) : MainWindowT<char>(parent) {
    // Connect signals
    QObject::connect(insertButton, &QPushButton::clicked, this, &MainWindowChar::onInsert);
    QObject::connect(extractMinButton, &QPushButton::clicked, this, &MainWindowChar::onExtractMin);
    QObject::connect(displayMinButton, &QPushButton::clicked, this, &MainWindowChar::onDisplayMin);
    QObject::connect(decreaseKeyButton, &QPushButton::clicked, this, &MainWindowChar::onDecreaseKey);
    QObject::connect(deleteValueButton, &QPushButton::clicked, this, &MainWindowChar::onDeleteValue);
    QObject::connect(clearButton, &QPushButton::clicked, this, &MainWindowChar::onClearHeap);
    
    // Connect visualizer signal - cast to correct type
    HeapVisualizerChar* typedVisualizer = static_cast<HeapVisualizerChar*>(visualizer);
    QObject::connect(typedVisualizer, &HeapVisualizerChar::nodeRightClicked,
            this, &MainWindowChar::onNodeRightClicked);
}

void MainWindowChar::onInsert() {
    bool ok;
    char value = ValueConverter<char>::fromString(insertValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<char>::getTypeName()), true);
        return;
    }
    
    try {
        heap->insert(value);
        visualizer->updateVisualization(true);
        insertValueEdit->clear();
        showMessage("Success", QString("Inserted value: %1").arg(ValueConverter<char>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindowChar::onExtractMin() {
    try {
        char minValue = heap->extractMin();
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Extracted minimum value: %1").arg(ValueConverter<char>::toString(minValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindowChar::onDisplayMin() {
    try {
        char minValue = heap->getMin();
        visualizer->highlightMinNode();
        int duration = HeapVisualizerT<char>::HIGHLIGHT_DURATION_MS / 1000;
        showMessage("Minimum Value", 
                   QString("Minimum value is: %1\n(Highlighted in red for %2 seconds)")
                   .arg(ValueConverter<char>::toString(minValue)).arg(duration));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindowChar::onDecreaseKey() {
    bool ok1, ok2;
    char oldValue = ValueConverter<char>::fromString(decreaseOldValueEdit->text(), &ok1);
    char newValue = ValueConverter<char>::fromString(decreaseNewValueEdit->text(), &ok2);
    
    if (!ok1 || !ok2) {
        showMessage("Error", QString("Please enter valid %1 values.").arg(ValueConverter<char>::getTypeName()), true);
        return;
    }
    
    try {
        heap->decreaseKey(oldValue, newValue);
        visualizer->updateVisualization(true);
        decreaseOldValueEdit->clear();
        decreaseNewValueEdit->clear();
        showMessage("Success", QString("Decreased key from %1 to %2")
                   .arg(ValueConverter<char>::toString(oldValue))
                   .arg(ValueConverter<char>::toString(newValue)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindowChar::onDeleteValue() {
    bool ok;
    char value = ValueConverter<char>::fromString(deleteValueEdit->text(), &ok);
    
    if (!ok) {
        showMessage("Error", QString("Please enter a valid %1 value.").arg(ValueConverter<char>::getTypeName()), true);
        return;
    }
    
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        deleteValueEdit->clear();
        showMessage("Success", QString("Deleted value: %1").arg(ValueConverter<char>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindowChar::onClearHeap() {
    if (heap->isEmpty()) {
        showMessage("Information", "Heap is already empty.");
        return;
    }
    
    heap->clear();
    visualizer->updateVisualization(false);
    showMessage("Success", "Heap cleared successfully!");
}

void MainWindowChar::onNodeRightClicked(char value) {
    try {
        heap->deleteKey(value);
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Deleted node with value: %1").arg(ValueConverter<char>::toString(value)));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}
