#include "MainWindow.h"
#include <QSplitter>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), intHeap(nullptr), charHeap(nullptr), currentType(INT_TYPE) {
    
    intHeap = new BinomialHeap<int>();
    charHeap = new BinomialHeap<char>();
    setupUI();
    setWindowTitle("Binomial Heap Visualizer - Int/Char Support");
    resize(1400, 800);
}

MainWindow::~MainWindow() {
    delete intHeap;
    delete charHeap;
}

void MainWindow::setupUI() {
    // Central widget with splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create visualizer
    visualizer = new HeapVisualizer(this);
    visualizer->setHeap(intHeap);
    
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
    
    // Data Type Selection
    QLabel* typeLabel = new QLabel("Data Type:", controlGroup);
    QFont labelFont = typeLabel->font();
    labelFont.setBold(true);
    typeLabel->setFont(labelFont);
    
    typeButtonGroup = new QButtonGroup(controlGroup);
    intTypeRadio = new QRadioButton("Integer", controlGroup);
    charTypeRadio = new QRadioButton("Character", controlGroup);
    intTypeRadio->setChecked(true);
    
    typeButtonGroup->addButton(intTypeRadio, INT_TYPE);
    typeButtonGroup->addButton(charTypeRadio, CHAR_TYPE);
    
    connect(intTypeRadio, &QRadioButton::toggled, this, &MainWindow::onTypeChanged);
    connect(charTypeRadio, &QRadioButton::toggled, this, &MainWindow::onTypeChanged);
    
    groupLayout->addWidget(typeLabel);
    groupLayout->addWidget(intTypeRadio);
    groupLayout->addWidget(charTypeRadio);
    groupLayout->addSpacing(15);
    
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
    QString text = insertValueEdit->text().trimmed();
    
    if (text.isEmpty()) {
        showMessage("Error", "Please enter a value.", true);
        return;
    }
    
    try {
        if (currentType == INT_TYPE) {
            bool ok;
            int value = text.toInt(&ok);
            
            if (!ok) {
                showMessage("Error", "Please enter a valid integer value.", true);
                return;
            }
            
            intHeap->insert(value);
            visualizer->updateVisualization(true);
            insertValueEdit->clear();
            showMessage("Success", QString("Inserted value: %1").arg(value));
        } else { // CHAR_TYPE
            if (text.length() != 1) {
                showMessage("Error", "Please enter exactly one character.", true);
                return;
            }
            
            char value = text[0].toLatin1();
            charHeap->insert(value);
            visualizer->updateVisualization(true);
            insertValueEdit->clear();
            showMessage("Success", QString("Inserted value: %1").arg(value));
        }
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onExtractMin() {
    try {
        if (currentType == INT_TYPE) {
            int minValue = intHeap->extractMin();
            visualizer->updateVisualization(true);
            showMessage("Success", QString("Extracted minimum value: %1").arg(minValue));
        } else { // CHAR_TYPE
            char minValue = charHeap->extractMin();
            visualizer->updateVisualization(true);
            showMessage("Success", QString("Extracted minimum value: %1").arg(minValue));
        }
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDisplayMin() {
    try {
        if (currentType == INT_TYPE) {
            int minValue = intHeap->getMin();
            visualizer->highlightMinNode();
            int duration = HeapVisualizer::HIGHLIGHT_DURATION_MS / 1000;
            showMessage("Minimum Value", 
                       QString("Minimum value is: %1\n(Highlighted in red for %2 seconds)")
                       .arg(minValue).arg(duration));
        } else { // CHAR_TYPE
            char minValue = charHeap->getMin();
            visualizer->highlightMinNode();
            int duration = HeapVisualizer::HIGHLIGHT_DURATION_MS / 1000;
            showMessage("Minimum Value", 
                       QString("Minimum value is: %1\n(Highlighted in red for %2 seconds)")
                       .arg(minValue).arg(duration));
        }
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDecreaseKey() {
    QString oldText = decreaseOldValueEdit->text().trimmed();
    QString newText = decreaseNewValueEdit->text().trimmed();
    
    if (oldText.isEmpty() || newText.isEmpty()) {
        showMessage("Error", "Please enter both old and new values.", true);
        return;
    }
    
    try {
        if (currentType == INT_TYPE) {
            bool ok1, ok2;
            int oldValue = oldText.toInt(&ok1);
            int newValue = newText.toInt(&ok2);
            
            if (!ok1 || !ok2) {
                showMessage("Error", "Please enter valid integer values.", true);
                return;
            }
            
            intHeap->decreaseKey(oldValue, newValue);
            visualizer->updateVisualization(true);
            decreaseOldValueEdit->clear();
            decreaseNewValueEdit->clear();
            showMessage("Success", QString("Decreased key from %1 to %2").arg(oldValue).arg(newValue));
        } else { // CHAR_TYPE
            if (oldText.length() != 1 || newText.length() != 1) {
                showMessage("Error", "Please enter exactly one character for each field.", true);
                return;
            }
            
            char oldValue = oldText[0].toLatin1();
            char newValue = newText[0].toLatin1();
            
            charHeap->decreaseKey(oldValue, newValue);
            visualizer->updateVisualization(true);
            decreaseOldValueEdit->clear();
            decreaseNewValueEdit->clear();
            showMessage("Success", QString("Decreased key from %1 to %2").arg(oldValue).arg(newValue));
        }
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onDeleteValue() {
    QString text = deleteValueEdit->text().trimmed();
    
    if (text.isEmpty()) {
        showMessage("Error", "Please enter a value.", true);
        return;
    }
    
    try {
        if (currentType == INT_TYPE) {
            bool ok;
            int value = text.toInt(&ok);
            
            if (!ok) {
                showMessage("Error", "Please enter a valid integer value.", true);
                return;
            }
            
            intHeap->deleteKey(value);
            visualizer->updateVisualization(true);
            deleteValueEdit->clear();
            showMessage("Success", QString("Deleted value: %1").arg(value));
        } else { // CHAR_TYPE
            if (text.length() != 1) {
                showMessage("Error", "Please enter exactly one character.", true);
                return;
            }
            
            char value = text[0].toLatin1();
            charHeap->deleteKey(value);
            visualizer->updateVisualization(true);
            deleteValueEdit->clear();
            showMessage("Success", QString("Deleted value: %1").arg(value));
        }
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onClearHeap() {
    if (currentType == INT_TYPE) {
        if (intHeap->isEmpty()) {
            showMessage("Information", "Heap is already empty.");
            return;
        }
        intHeap->clear();
    } else { // CHAR_TYPE
        if (charHeap->isEmpty()) {
            showMessage("Information", "Heap is already empty.");
            return;
        }
        charHeap->clear();
    }
    
    visualizer->updateVisualization(false);
    showMessage("Success", "Heap cleared successfully!");
}

void MainWindow::onNodeRightClicked(int value) {
    try {
        if (currentType == INT_TYPE) {
            intHeap->deleteKey(value);
        } else { // CHAR_TYPE
            charHeap->deleteKey(static_cast<char>(value));
        }
        visualizer->updateVisualization(true);
        showMessage("Success", QString("Deleted node with value: %1").arg(
            currentType == INT_TYPE ? QString::number(value) : QString(static_cast<char>(value))));
    } catch (const std::exception& e) {
        showMessage("Error", e.what(), true);
    }
}

void MainWindow::onTypeChanged() {
    DataType newType = intTypeRadio->isChecked() ? INT_TYPE : CHAR_TYPE;
    
    if (newType != currentType) {
        switchHeapType(newType);
    }
}

void MainWindow::switchHeapType(DataType newType) {
    // Clear both heaps for clean switch
    intHeap->clear();
    charHeap->clear();
    
    currentType = newType;
    
    // Update visualizer to use the correct heap
    if (currentType == INT_TYPE) {
        visualizer->setHeap(intHeap);
    } else {
        visualizer->setHeap(charHeap);
    }
    
    // Update placeholders
    updatePlaceholders();
    
    // Clear input fields
    insertValueEdit->clear();
    decreaseOldValueEdit->clear();
    decreaseNewValueEdit->clear();
    deleteValueEdit->clear();
    
    showMessage("Type Changed", 
                QString("Switched to %1 mode. Previous heap data has been cleared.")
                .arg(currentType == INT_TYPE ? "Integer" : "Character"));
}

void MainWindow::updatePlaceholders() {
    if (currentType == INT_TYPE) {
        insertValueEdit->setPlaceholderText("Enter integer value");
        decreaseOldValueEdit->setPlaceholderText("Old integer value");
        decreaseNewValueEdit->setPlaceholderText("New integer value (smaller)");
        deleteValueEdit->setPlaceholderText("Integer value to delete");
    } else {
        insertValueEdit->setPlaceholderText("Enter single character");
        decreaseOldValueEdit->setPlaceholderText("Old character");
        decreaseNewValueEdit->setPlaceholderText("New character (smaller)");
        deleteValueEdit->setPlaceholderText("Character to delete");
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
