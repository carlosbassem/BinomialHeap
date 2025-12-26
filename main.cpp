#include <QApplication>
#include "MainWindow.h"
#include "MainWindowTemplate.h"
#include "TypeSelectionDialog.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Show type selection dialog
    TypeSelectionDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QMainWindow* window = nullptr;
        
        if (dialog.getSelectedType() == TypeSelectionDialog::INTEGER) {
            window = new MainWindowT<int>();
        } else {
            window = new MainWindowT<char>();
        }
        
        if (window) {
            window->show();
            int result = app.exec();
            delete window;
            return result;
        }
    }
    
    return 0;
}

/* 
 * Legacy CLI Code - Preserved for Reference
 * 
 * The following code implements a command-line interface (CLI) for the binomial heap
 * with complexity testing capabilities. It has been disabled (#if 0) in favor of the
 * new Qt6-based GUI, but is kept here for:
 * - Reference and understanding of the original implementation
 * - Potential future CLI mode
 * - Complexity testing functions that may be useful
 * 
 * To enable this code, change #if 0 to #if 1 below and comment out the GUI main above.
 */
#if 0
#include <iostream>
#include "binomial_heap.hpp"
#include <vector>
#include <chrono>
using namespace std;
// Helper to measure execution time of a function

template<typename Func>
long long measureTime(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}


// -----------------------------------------------------
//  TEST INSERT COMPLEXITY
// -----------------------------------------------------
void testInsertComplexity(int n) {
    BinomialHeap<int> heap;
    vector<int> data(n);

    // Fill input
    for (int i = 0; i < n; i++) data[i] = rand();

    long long t = measureTime([&]() {
        for (int i = 0; i < n; i++)
            heap.insert(data[i]);
        });

    cout << "\nInsert " << n << " elements took " << t << " microseconds\n";
}


// -----------------------------------------------------
//  TEST GET-MIN COMPLEXITY
// -----------------------------------------------------
void testGetMinComplexity(int n) {
    BinomialHeap<int> heap;

    for (int i = 0; i < n; i++)
        heap.insert(rand());

    long long t = measureTime([&]() {
        heap.getMin();
        });

    cout << "getMin() on size " << n << " took " << t << " microseconds\n";
}


// -----------------------------------------------------
//  TEST EXTRACT-MIN COMPLEXITY
// -----------------------------------------------------
void testExtractMinComplexity(int n) {
    BinomialHeap<int> heap;

    for (int i = 0; i < n; i++)
        heap.insert(rand());

    long long t = measureTime([&]() {
        heap.extractMin();
        });

    cout << "extractMin() on size " << n << " took " << t << " microseconds\n";
}


// -----------------------------------------------------
//  TEST DECREASE-KEY COMPLEXITY
// -----------------------------------------------------
void testDecreaseKeyComplexity(int n) {
    BinomialHeap<int> heap;
    vector<int> values(n);

    for (int i = 0; i < n; i++) {
        heap.insert(i + 10);
        values[i] = i + 10;
    }

    long long t = measureTime([&]() {
        heap.decreaseKey(values[n / 2], values[n / 2] - 5);
        });

    cout << "decreaseKey() on size " << n << " took " << t << " microseconds\n";
}


// -----------------------------------------------------
//  TEST FIND KEY COMPLEXITY
// -----------------------------------------------------
void testFindKeyComplexity(int n) {
    BinomialHeap<int> heap;

    for (int i = 0; i < n; i++)
        heap.insert(i);

    long long t = measureTime([&]() {
        heap.findKey(n - 1);
        });

    cout << "findKey() on size " << n << " took " << t << " microseconds\n";
}


// -----------------------------------------------------
//  TEST UNION COMPLEXITY
// -----------------------------------------------------
void testUnionComplexity(int n) {
    BinomialHeap<int> A, B;

    for (int i = 0; i < n; i++) A.insert(rand());
    for (int i = 0; i < n; i++) B.insert(rand());

    long long t = measureTime([&]() {
        A.testUnion(B);
        });

    cout << "union() on two heaps of size "
        << n << " took " << t << " microseconds\n";
}


void printMenu() {
    cout << "\n=========== BINOMIAL HEAP MENU ===========\n";
    cout << "1. Insert\n";
    cout << "2. Get Minimum\n";
    cout << "3. Extract Minimum\n";
    cout << "4. Decrease Key\n";
    cout << "5. Find Key\n";
    cout << "6. Display Heap\n";
    cout << "7. Delete value\n";
    cout << "8. Clear Heap\n";
	cout << "9. Test complexity (not implemented)\n";
    cout << "0. Exit\n";
    cout << "==========================================\n";
    cout << "Choose an option: ";
}
void runComplexityTests() {
    vector<int> sizes = { 1000, 5000, 10000, 20000, 50000 };

    for (int n : sizes) {
        cout << "\n---- Testing size = " << n << " ----\n";
        testInsertComplexity(n);
        testGetMinComplexity(n);
        testExtractMinComplexity(n);
        testFindKeyComplexity(n);
        testDecreaseKeyComplexity(n);
        testUnionComplexity(n);
    }
}

// Old CLI main function (disabled)
void old_main() {
    BinomialHeap<int> heap;
    int choice, value, newValue;

    while (true) {
        printMenu();
        cin >> choice;

        switch (choice) {

            // ------------------------------------------
        case 1: // INSERT
            cout << "Enter value to insert: ";
            cin >> value;
            heap.insert(value);
            cout << "\nInserted " << value << ".\n";
            heap.displayHeap(&heap);
            break;

            // ------------------------------------------
        case 2: // GET MIN
            try {
                cout << "Minimum value = " << heap.getMin() << endl;
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            heap.displayHeap(&heap);
            break;

            // ------------------------------------------
        case 3: // EXTRACT MIN
            try {
                cout << "Extracted min = " << heap.extractMin() << endl;
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            heap.displayHeap(&heap);
            break;

            // ------------------------------------------
        case 4: // DECREASE KEY
            cout << "Enter current value: ";
            cin >> value;
            cout << "Enter new smaller value: ";
            cin >> newValue;
            try {
                heap.decreaseKey(value, newValue);
                cout << "Decreased key from " << value
                    << " to " << newValue << endl;
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            heap.displayHeap(&heap);
            
            break;

            // ------------------------------------------
        case 5: // FIND KEY
        {
            cout << "Enter value to find: ";
            cin >> value;
            BinomialNode<int>* result = heap.findKey(value);

            if (result)
                cout << "Key " << value << " FOUND in heap.\n";
            else
                cout << "Key " << value << " NOT FOUND.\n";

            heap.displayHeap(&heap);
            break;
        }

        // ------------------------------------------
        case 6: // DISPLAY HEAP
            heap.displayHeap(&heap);
            break;

            // ------------------------------------------
		case 7: // DELETE VALUE
            cout << "Enter value to delete: ";
            cin >> value;
            try {
                heap.deleteKey(value);
                cout << "Deleted key " << value << " from heap.\n";
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
            heap.displayHeap(&heap);
			break;
		case 8: // CLEAR HEAP
			heap.clear();
			heap.displayHeap(&heap);
            break;
			// ------------------------------------------
		case 9: // RUN COMPLEXITY TESTS
			runComplexityTests();
			break;
        case 0:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid option! Try again.\n";
        }
    }

}
#endif
