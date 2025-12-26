#include "TypeSelectionDialog.h"

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
