#include "binomial_heap.hpp"
#include <iostream>
using namespace std;

template <typename B>
BinomialHeap<B>::BinomialHeap() {
	head = nullptr;
}
template <typename B>
BinomialHeap<B>::BinomialHeap(const BinomialHeap<B>& other) {
	*this = other;
}
template <typename B>
BinomialHeap<B>& BinomialHeap<B>::operator=(const BinomialHeap<B>& other) {
	if (this != &other) {
		clear();

		if (other.head) {
			head = copyHeap(other.head, nullptr);
		}
		else {
			head = nullptr;
		}
	}
	return *this;
}
template <typename B>
BinomialNode<B>* BinomialHeap<B>::copyHeap(BinomialNode<B>* node, BinomialNode<B>* parent) {
	if (!node) return nullptr;

	BinomialNode<B>* newNode = new BinomialNode<B>(node->value, parent);
	newNode->order = node->order;

	if (node->child) {
		newNode->child = copyHeap(node->child, newNode);
	}

	if (node->sibling) {
		newNode->sibling = copyHeap(node->sibling, parent);
	}

	return newNode;
}
template <typename B>
BinomialHeap<B>::~BinomialHeap() {
	clear();
}
template <typename B>
void BinomialHeap<B>::insert(B value) {
	BinomialNode<B>* newNode = new BinomialNode<B>(value);
	head= unionHeap(newNode, head);
}
template <typename B>
B BinomialHeap<B>::getMin() {
	if (head == nullptr) {
		throw std::runtime_error("Cannot get min from empty heap");
	}

	B min = head->value;
	BinomialNode<B>* currentPtr = head, * nextPtr = nullptr;
	while (currentPtr != nullptr) {
		if (currentPtr->value < min) {
			min = currentPtr->value;
		}
		nextPtr = currentPtr->sibling;
		currentPtr = nextPtr;
	}
	return min;
}
template <typename B>
B BinomialHeap<B>::extractMin() {
	if (head == nullptr) {
		throw std::runtime_error("Cannot extract min from empty heap");
	}
	BinomialNode<B>* minPtr = head, * currentPtr = head, * prevPtr = nullptr, * minPrev = nullptr;
	B minVal = head->value;
	while (currentPtr != nullptr) {
		if (currentPtr->value < minVal) {
			minVal = currentPtr->value;
			minPrev = prevPtr;
			minPtr = currentPtr;
		}
		prevPtr = currentPtr;
		currentPtr = currentPtr->sibling;
	}
	if (minPrev != nullptr) {
		minPrev->sibling = minPtr->sibling;
	}
	else {
		head = minPtr->sibling;
	}
	BinomialNode<B>* child = minPtr->child; // (akeno prevPtr belzabt) el leftmost child bta3 el heap (3ashan el heap lama bn delete el head bta3ha btb2a heaps soghayara bas ma3kosa)
	BinomialNode<B>* newHead = nullptr; //3ayez el child el awalany yb2a el sibling bta3o null
	while (child != nullptr) {
		BinomialNode<B>* next = child->sibling;
		child->sibling = newHead; //betkhaly el child yeshawer 3ala ely ablo (newHead)
		child->parent = nullptr;
		newHead = child; //prevPtr = currentPtr
		child = next; //currentPtr = nextPtr
	}
	head = unionHeap(head, newHead);
	return minVal;
}
template <typename B>
void BinomialHeap<B>::deleteKey(B value) {
	BinomialNode<B>* node = findKey(value);
	while(!node){
		throw std::runtime_error("Value not found in heap");
	}
	decreaseKey(value, getMin() - 1);
	extractMin();

}
template <typename B>
void BinomialHeap<B>::decreaseKey(B value, B newValue) {
	BinomialNode<B>* node = findKey(value);

	if (!node) {
		throw std::runtime_error("Key not found in heap");
	}

	if (newValue >= node->value) {
		throw std::runtime_error("New value must be smaller than current value");
	}

	node->value = newValue;

	BinomialNode<B>* current = node;
	BinomialNode<B>* parent = current->parent;

	while (parent && current->value < parent->value) {
		// Swap 
		B temp = current->value;
		current->value = parent->value;
		parent->value = temp;

		current = parent;
		parent = current->parent;
	}
}
template <typename B>
BinomialNode<B>* BinomialHeap<B>::findKey(B value) {
	if (!head) {
		return nullptr;
	}
	BinomialNode<B>* currentRoot = head;

	while (currentRoot) {
		BinomialNode<B>* foundNode = currentRoot->find(value);

		if (foundNode) {
			return foundNode;
		}
		currentRoot = currentRoot->sibling;
	}
	return nullptr;
}
template <typename T>
BinomialNode<T>* BinomialNode<T>::find(T v) {
	if (value == v) {
		return this;
	}
	BinomialNode<T>* childPtr = child;
	while (childPtr) {
		// Recursively search this child's entire subtree
		BinomialNode<T>* found = childPtr->find(v);

		if (found) {
			return found;
		}
		childPtr = childPtr->sibling;
	}
	return nullptr;
}
template <typename B>
int BinomialHeap<B>::size() {
	int size = 0;
	BinomialNode<B>* curr = head;
	while(curr != nullptr){
		size = size + (1 << (curr->order)); // ba shift left by el order beta3 el node
		curr = curr->sibling;
	}
	return (size);
}
template <typename B>
bool BinomialHeap<B>::isEmpty() {
	return(head == 0);
}
template <typename B>
void BinomialNode<B>::deleteNode(BinomialNode<B>* node) {
	BinomialNode<B>* c = node->child;
	while (c != nullptr) {
		BinomialNode<B>* next = c->sibling;
		deleteNode(c);
		c = next;
	}
	delete node;
}
template <typename B>
void BinomialHeap<B>::clear() {
	if (isEmpty()) {
		cout<< "Heap is already empty"<< endl;
		return;
	}
	else {
		BinomialNode<B>* curr = head;
		while (curr != nullptr) {
			BinomialNode<B>* next = curr->sibling;
			curr->deleteNode(curr);
			curr = next;

		}

	}
	head = nullptr;
	cout << "Heap successfully emptied!" << endl;

}

template <typename B>
BinomialNode<B>* BinomialHeap<B>::unionHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB) {

	BinomialNode<B>* heapU = mergeHeap(heapA, heapB);
	if (!heapU) return nullptr;

	BinomialNode<B>* prev = nullptr;
	BinomialNode<B>* curr = heapU;
	BinomialNode<B>* next = curr->sibling;

	while (next != nullptr) {

		if (curr->order != next->order ||
			(next->sibling != nullptr && next->sibling->order == curr->order))
		{
			prev = curr;
			curr = next;
		}
		else {

			if (curr->value <= next->value) {
				// curr stays root
				curr->sibling = next->sibling;   
				next->sibling = nullptr;         
				curr->addChild(next);
			}
			else {
				// next becomes new root
				if (prev != nullptr)
					prev->sibling = next;
				else
					heapU = next;

				curr->sibling = nullptr;         
				next->addChild(curr);

				curr = next;
			}
		}
		next = curr->sibling;
	}
	return heapU;
}



template <typename B>
BinomialNode<B>* BinomialHeap<B>::mergeHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB) {
	if (!heapA) return heapB;
	if (!heapB) return heapA;
	BinomialNode<B>* head = nullptr;
	BinomialNode<B>* tail = nullptr;
	if (heapA->order <= heapB->order) {
		head = heapA;
		tail = heapA;
		heapA = heapA->sibling;
	}
	else {
		head = heapB;
		tail = heapB;
		heapB = heapB->sibling;
	}
	while (heapA && heapB) {
		if (heapA->order <= heapB->order) {
			tail->sibling = heapA;
			tail = heapA;
			heapA = heapA->sibling;
		}
		else {
			tail->sibling = heapB;
			tail = heapB;
			heapB = heapB->sibling;
		}
	}
	if (heapA) {
		tail->sibling = heapA;
	}
	else if (heapB) {
		tail->sibling = heapB;
	}

	
	return head;

}


template <typename B>
int BinomialHeap<B>::order(BinomialNode<B>* heap) {
	if (heap == nullptr)
		return -1;

	int i = 0;
	BinomialNode<B>* temp = heap->child;

	while (temp != nullptr) {
		i++;
		temp = temp->sibling;
	}

	return i;
}


// Binomial Node

template <typename B>
BinomialNode<B>::BinomialNode(B v, BinomialNode<B>* p): parent(p), value(v),order(0),child(nullptr),sibling(nullptr) {

	if(p != nullptr){
		if(v < p->value){
			cout << "Error: child value less than parent value!(constructor)" << endl;
			return;
		}
	}
}


template <typename B>
void BinomialNode<B>::addChild(BinomialNode<B>* c) {
	if (c != nullptr) {
		if(c->value < value){
			cout << "Error: child value less than parent value!(addChild func)" << endl;
			return;
		}
		if (child == nullptr) {
			
			child = c;
		}
		else {
			c->sibling = child;
			child = c;
		}
		c->parent = this;
		order++;
	}


}
template <typename B>
void BinomialHeap<B>::DisplayNode(BinomialNode<B>* node, int depth) {
	for (int i = 0; i < depth; i++) {
		cout << "  ";
	}
	cout << node->value << endl;
	BinomialNode<B>* child = node->child;
	while (child != nullptr) {
		DisplayNode(child, depth + 1);
		child = child->sibling;
	}
}
template <typename B>
void BinomialHeap<B>::displayHeap(const BinomialHeap<B>* heap) {
	if (heap->head == nullptr) {
		cout << "Heap is empty!" << endl;
		return;
	}
	BinomialNode<B>* curr = heap->head;
	while (curr != nullptr) {
		cout << "B" << curr->order << endl;
		DisplayNode(curr, 0);
		curr = curr->sibling;
	}
	cout << "Size of heap: " << size()<< endl;
}

template <typename B>
void BinomialHeap<B>::duplicateAndMergeRootTree(B rootValue) {
	// Find the root node with the given value
	BinomialNode<B>* currentRoot = head;
	BinomialNode<B>* targetRoot = nullptr;
	
	// Search through the root nodes (nodes at the head level)
	while (currentRoot) {
		if (currentRoot->value == rootValue) {
			targetRoot = currentRoot;
			break;
		}
		currentRoot = currentRoot->sibling;
	}
	
	if (!targetRoot) {
		throw std::runtime_error("Root node with specified value not found");
	}
	
	// Copy the entire tree rooted at targetRoot
	// Temporarily save the sibling pointer to avoid copying it
	BinomialNode<B>* originalSibling = targetRoot->sibling;
	targetRoot->sibling = nullptr;
	
	// Create a standalone copy (not connected to any other roots)
	BinomialNode<B>* copiedTree = copyHeap(targetRoot, nullptr);
	
	// Restore the original sibling pointer
	targetRoot->sibling = originalSibling;
	
	// Merge the copied tree back into the heap using union
	head = unionHeap(head, copiedTree);
}


template class BinomialNode<int>;
template class BinomialHeap<int>;

template class BinomialNode<char>;
template class BinomialHeap<char>;
