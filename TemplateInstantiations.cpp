#include "MainWindowTemplate.h"
#include "HeapVisualizerTemplate.h"

/*
 * Explicit Template Instantiations for Qt MOC Compatibility
 * 
 * Qt's Meta-Object Compiler (MOC) cannot process template classes with Q_OBJECT
 * directly. To work around this limitation, we use explicit template instantiations
 * for the specific types we need (int and char).
 * 
 * This approach is the recommended Qt pattern when:
 * 1. You have a small, fixed set of template types
 * 2. The types are known at compile time
 * 3. You need Qt's signals/slots functionality
 * 
 * Alternative approaches (inheritance/composition) would significantly increase
 * code complexity without providing meaningful benefits for this use case.
 * 
 * If additional types are needed in the future, simply add their instantiations here.
 */

// Explicit template instantiations for int
template class MainWindowT<int>;
template class HeapVisualizerT<int>;
template class NodeItemT<int>;

// Explicit template instantiations for char
template class MainWindowT<char>;
template class HeapVisualizerT<char>;
template class NodeItemT<char>;
