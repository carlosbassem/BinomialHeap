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
#include "HeapVisualizer.h"
#include "binomial_heap.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
private slots:
    // Heap operations
    void onInsert();
    void onExtractMin();
    void onDisplayMin();
    void onDecreaseKey();
    void onDeleteValue();
    void onClearHeap();
    void onNodeRightClicked(int value);
    
private:
    // UI Components
    HeapVisualizer* visualizer;
    
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
    BinomialHeap<int>* heap;
    
    // UI Setup
    void setupUI();
    QWidget* createControlPanel();
    void showMessage(const QString& title, const QString& message, bool isError = false);
};

#endif // MAINWINDOW_H
