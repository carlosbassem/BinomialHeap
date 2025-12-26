#include "MainWindowTemplate.h"
#include "HeapVisualizerTemplate.h"

// Explicit template instantiations for int
template class MainWindowT<int>;
template class HeapVisualizerT<int>;
template class NodeItemT<int>;

// Explicit template instantiations for char
template class MainWindowT<char>;
template class HeapVisualizerT<char>;
template class NodeItemT<char>;
