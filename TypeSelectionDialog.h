#ifndef TYPESELECTIONDIALOG_H
#define TYPESELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>

class TypeSelectionDialog : public QDialog {
    Q_OBJECT
    
public:
    enum HeapType {
        INTEGER,
        CHARACTER
    };
    
    explicit TypeSelectionDialog(QWidget* parent = nullptr);
    HeapType getSelectedType() const { return selectedType; }
    
private slots:
    void onOkClicked();
    
private:
    HeapType selectedType;
    QRadioButton* intRadio;
    QRadioButton* charRadio;
};

#endif // TYPESELECTIONDIALOG_H
