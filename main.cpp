#include <QApplication>
#include <memory>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Show type selection dialog
    TypeSelectionDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        std::unique_ptr<QMainWindow> window;
        
        if (dialog.getSelectedType() == TypeSelectionDialog::INTEGER) {
            window = std::make_unique<MainWindow>();
        } else {
            window = std::make_unique<MainWindowChar>();
        }
        
        if (window) {
            window->show();
            return app.exec();
        }
    }
    
    return 0;
}

