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
 * IMPORTANT: If you need to add support for additional types in the future:
 * 1. Add the template instantiation here (e.g., "template class MainWindowT<float>;")
 * 2. Update ValueConverter.h with specializations for the new type
 * 3. Rebuild the project to ensure MOC processes the new instantiations
 * 
 * If you encounter compilation errors about missing template definitions or
 * undefined references to Qt meta-object functions, verify that all template
 * classes are explicitly instantiated here.
 */

// Explicit template instantiations for int
template class MainWindowT<int>;
template class HeapVisualizerT<int>;
template class NodeItemT<int>;

// Explicit template instantiations for char
template class MainWindowT<char>;
template class HeapVisualizerT<char>;
template class NodeItemT<char>;
