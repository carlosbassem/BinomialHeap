#ifndef BINOMIALHEAP_H_
#define BINOMIALHEAP_H_


#define DEBUG false
#include <iostream>

template <typename B>
class BinomialNode;

template <typename B>
class BinomialHeap
{

public:
	
	friend class BinomialNode<B>;

	BinomialHeap();
	BinomialHeap(const BinomialHeap& other);
	//BinomialHeap& operator=(const BinomialHeap& other);
	~BinomialHeap();
	void insert(B value);
	B getMin();
	
	B extractMin();
	void deleteKey(B value);
	
	void decreaseKey(B value, B newValue);
	BinomialNode<B>* findKey(B value);
	int size();
	bool isEmpty();
	void clear();
	void DisplayNode(BinomialNode<B>* node, int depth);
	void displayHeap(const BinomialHeap<B>* heap);
	void testUnion(BinomialHeap<B>& other) {
		head = unionHeap(head, other.head);
		other.head = nullptr; // avoid double free
	}
	
	// Public getter for visualization
	BinomialNode<B>* getHead() const { return head; }
	
protected:
	static BinomialNode<B>* unionHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB);
	static BinomialNode<B>* mergeHeap(BinomialNode<B>* heapA, BinomialNode<B>* heapB);
	static int order(BinomialNode<B>* heap);


private:

	BinomialNode<B>* head;
};

template <typename B>
class BinomialNode
{
public:
	friend class BinomialHeap<B>;
	BinomialNode(B v, BinomialNode<B>* p = nullptr);
	void deleteNode(BinomialNode<B>* node);
	void addChild(BinomialNode<B>* child);
	BinomialNode<B>* find(B v);
	//void addTo(BinomialHeap<B>* other);
	
	// Public getters for visualization
	B getValue() const { return value; }
	BinomialNode<B>* getSibling() const { return sibling; }
	BinomialNode<B>* getChild() const { return child; }
	BinomialNode<B>* getParent() const { return parent; }
	int getOrder() const { return order; }

private:
	B value;
	BinomialNode<B>* sibling, * child, * parent;
	int order;

};
#endif /* BINOMIALHEAP_H_ */
