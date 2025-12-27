#ifndef HEAPVISUALIZER_H
#define HEAPVISUALIZER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QMap>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QAction>
#include <cmath>
#include "binomial_heap.hpp"
#include "ValueConverter.h"

static constexpr qreal NODE_RADIUS = 25.0;

// Custom ellipse item that stores node value (templated)
template <typename T>
class NodeItemT : public QGraphicsEllipseItem {
public:
    NodeItemT(T value, qreal x, qreal y, qreal radius = 25);
    T getValue() const { return nodeValue; }
    void setHighlighted(bool highlight);
    
private:
    T nodeValue;
    QGraphicsTextItem* textItem;
    QPen normalPen;
    QPen highlightPen;
};

// Custom line item with arrowheads
class ArrowItem : public QGraphicsLineItem {
public:
    ArrowItem(QPointF start, QPointF end, bool doubleArrow = false);
    void updatePosition(QPointF start, QPointF end);
    
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    
private:
    bool isDoubleArrow;
    QPolygonF createArrowHead(QPointF start, QPointF end);
};

// Templated HeapVisualizer class (without Q_OBJECT for MOC compatibility)
template <typename T>
class HeapVisualizerT : public QGraphicsView {
    
public:
    explicit HeapVisualizerT(QWidget* parent = nullptr);
    virtual ~HeapVisualizerT();
    
    void setHeap(BinomialHeap<T>* heap);
    void updateVisualization(bool animate = true);
    void highlightMinNode();
    
    // Layout and timing constants
    static constexpr int HIGHLIGHT_DURATION_MS = 2000;  // 2 seconds
    static constexpr qreal MIN_NODE_WIDTH = 80.0;
    static constexpr qreal VERTICAL_SPACING = 120.0;
    static constexpr qreal TREE_GAP = 100.0;
    static constexpr qreal SCENE_MIN_X = -500.0;
    static constexpr qreal SCENE_MIN_Y = -300.0;
    static constexpr qreal SCENE_WIDTH = 3000.0;
    static constexpr qreal SCENE_HEIGHT = 1500.0;
    
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    
    // Virtual signal-like method for derived classes to override
    virtual void onNodeRightClicked(T value) = 0;
    virtual void onRootNodeLeftClicked(T value) = 0;
    
private:
    struct NodePosition {
        qreal x;
        qreal y;
        BinomialNode<T>* node;
    };
    
    BinomialHeap<T>* binomialHeap;
    QGraphicsScene* scene;
    QTimer* highlightTimer;
    NodeItemT<T>* currentHighlightedNode;
    
    QMap<BinomialNode<T>*, NodeItemT<T>*> nodeItemMap;
    QList<ArrowItem*> arrowItems;
    
    // Layout calculation
    void calculateLayout(BinomialNode<T>* root, qreal& currentX, qreal y, 
                         QMap<BinomialNode<T>*, NodePosition>& positions);
    void calculateSubtreeWidth(BinomialNode<T>* node, qreal& width);
    
    // Drawing methods
    void drawNode(BinomialNode<T>* node, const NodePosition& pos, bool animate);
    void drawConnections(BinomialNode<T>* node, 
                        const QMap<BinomialNode<T>*, NodePosition>& positions);
    void clearScene();
    
    // Animation
    void animateNodeToPosition(NodeItemT<T>* item, QPointF targetPos);
    
    // Helper
    NodeItemT<T>* findNodeItemAt(QPointF scenePos);
    bool isRootNode(BinomialNode<T>* node);
};

// Non-template wrapper classes with Q_OBJECT for Qt MOC
class HeapVisualizer : public HeapVisualizerT<int> {
    Q_OBJECT
    
public:
    explicit HeapVisualizer(QWidget* parent = nullptr) : HeapVisualizerT<int>(parent) {}
    
signals:
    void nodeRightClicked(int value);
    void rootNodeLeftClicked(int value);
    
protected:
    void onNodeRightClicked(int value) override {
        emit nodeRightClicked(value);
    }
    void onRootNodeLeftClicked(int value) override {
        emit rootNodeLeftClicked(value);
    }
};

class HeapVisualizerChar : public HeapVisualizerT<char> {
    Q_OBJECT
    
public:
    explicit HeapVisualizerChar(QWidget* parent = nullptr) : HeapVisualizerT<char>(parent) {}
    
signals:
    void nodeRightClicked(char value);
    void rootNodeLeftClicked(char value);
    
protected:
    void onNodeRightClicked(char value) override {
        emit nodeRightClicked(value);
    }
    void onRootNodeLeftClicked(char value) override {
        emit rootNodeLeftClicked(value);
    }
};

// Template implementation for NodeItemT
template <typename T>
NodeItemT<T>::NodeItemT(T value, qreal x, qreal y, qreal radius) 
    : QGraphicsEllipseItem(x - radius, y - radius, radius * 2, radius * 2), nodeValue(value) {
    
    // Set node color - light blue
    setBrush(QBrush(QColor(173, 216, 230)));
    normalPen = QPen(Qt::black, 2);
    highlightPen = QPen(Qt::red, 4);
    setPen(normalPen);

    // Add text for value
    textItem = new QGraphicsTextItem(ValueConverter<T>::toString(value), this);
    QFont font = textItem->font();
    font.setPointSize(12);
    font.setBold(true);
    textItem->setFont(font);

    // Center text inside the ellipse using local coordinates
    QRectF ellipseRect = rect();
    QRectF textRect = textItem->boundingRect();

    textItem->setPos(
        ellipseRect.center().x() - textRect.width() / 2,
        ellipseRect.center().y() - textRect.height() / 2);
}

template <typename T>
void NodeItemT<T>::setHighlighted(bool highlight) {
    setPen(highlight ? highlightPen : normalPen);
}

// Template implementation for HeapVisualizerT
template <typename T>
HeapVisualizerT<T>::HeapVisualizerT(QWidget* parent)
    : QGraphicsView(parent), binomialHeap(nullptr), currentHighlightedNode(nullptr) {
    
    scene = new QGraphicsScene(this);
    setScene(scene);
    
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Set scene size
    scene->setSceneRect(SCENE_MIN_X, SCENE_MIN_Y, SCENE_WIDTH, SCENE_HEIGHT);
    
    // Timer for highlight
    highlightTimer = new QTimer(this);
    highlightTimer->setSingleShot(true);
    QObject::connect(highlightTimer, &QTimer::timeout, [this]() {
        if (currentHighlightedNode) {
            currentHighlightedNode->setHighlighted(false);
            currentHighlightedNode = nullptr;
        }
    });
}

template <typename T>
HeapVisualizerT<T>::~HeapVisualizerT() {
    clearScene();
}

template <typename T>
void HeapVisualizerT<T>::setHeap(BinomialHeap<T>* heap) {
    binomialHeap = heap;
    updateVisualization(false);
}

template <typename T>
void HeapVisualizerT<T>::clearScene() {
    // Clear highlight timer
    if (highlightTimer->isActive()) {
        highlightTimer->stop();
    }
    currentHighlightedNode = nullptr;
    
    // Clear items
    scene->clear();
    nodeItemMap.clear();
    arrowItems.clear();
}

template <typename T>
void HeapVisualizerT<T>::calculateSubtreeWidth(BinomialNode<T>* node, qreal& width) {
    if (!node) {
        width = 0;
        return;
    }
    
    // Count children
    int childCount = 0;
    BinomialNode<T>* child = node->getChild();
    while (child) {
        childCount++;
        child = child->getSibling();
    }
    
    if (childCount == 0) {
        width = MIN_NODE_WIDTH;
        return;
    }
    
    // Calculate total width needed for all children
    qreal totalWidth = 0;
    child = node->getChild();
    while (child) {
        qreal childWidth = 0;
        calculateSubtreeWidth(child, childWidth);
        totalWidth += childWidth;
        child = child->getSibling();
    }
    
    width = std::max(totalWidth, MIN_NODE_WIDTH);
}

template <typename T>
void HeapVisualizerT<T>::calculateLayout(BinomialNode<T>* root, qreal& currentX, qreal y,
                                      QMap<BinomialNode<T>*, NodePosition>& positions) {
    if (!root) return;
    
    qreal subtreeWidth = 0;
    calculateSubtreeWidth(root, subtreeWidth);
    
    // Position root at center of its subtree
    qreal rootX = currentX + subtreeWidth / 2;
    
    NodePosition pos;
    pos.x = rootX;
    pos.y = y;
    pos.node = root;
    positions[root] = pos;
    
    // Layout children
    if (root->getChild()) {
        qreal childX = currentX;
        qreal childY = y + VERTICAL_SPACING;
        
        BinomialNode<T>* child = root->getChild();
        while (child) {
            calculateLayout(child, childX, childY, positions);
            child = child->getSibling();
        }
    }
    
    currentX += subtreeWidth;
}

template <typename T>
void HeapVisualizerT<T>::updateVisualization(bool animate) {
    if (!binomialHeap) return;
    
    clearScene();
    
    BinomialNode<T>* head = binomialHeap->getHead();
    if (!head) return;
    
    // Calculate positions for all nodes
    QMap<BinomialNode<T>*, NodePosition> positions;
    qreal currentX = 50;
    qreal rootY = 50;
    
    // Process each binomial tree root
    BinomialNode<T>* root = head;
    while (root) {
        calculateLayout(root, currentX, rootY, positions);
        currentX += TREE_GAP;
        root = root->getSibling();
    }
    
    // Draw all nodes
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        drawNode(it.key(), it.value(), animate);
    }
    
    // Draw head pointer
    if (head && positions.contains(head)) {
        NodePosition headPos = positions[head];

        // Text: "head"
        QGraphicsTextItem* headLabel = scene->addText("head");
        QFont font = headLabel->font();
        font.setBold(true);
        font.setPointSize(12);
        headLabel->setFont(font);

        QRectF textRect = headLabel->boundingRect();

        // Position text to the left of first root
        QPointF textPos(
            headPos.x - NODE_RADIUS * 2 - textRect.width() - 20,
            headPos.y - textRect.height() / 2
        );
        headLabel->setPos(textPos);

        // Arrow: head --> first root
        QPointF arrowStart(
            textPos.x() + textRect.width() + 5,
            headPos.y
        );

        QPointF arrowEnd(
            headPos.x - NODE_RADIUS,
            headPos.y
        );

        ArrowItem* headArrow = new ArrowItem(arrowStart, arrowEnd, false);
        scene->addItem(headArrow);
        arrowItems.append(headArrow);
    }

    // Draw order labels (B0, B1, ...) above each root
    BinomialNode<T>* rootNode = head;

    while (rootNode) {
        if (positions.contains(rootNode)) {
            NodePosition pos = positions[rootNode];

            QString labelText = QString("B%1").arg(rootNode->getOrder());

            QGraphicsTextItem* label = scene->addText(labelText);
            QFont font = label->font();
            font.setBold(true);
            font.setPointSize(12);
            label->setFont(font);

            QRectF textRect = label->boundingRect();

            // Position label above root node
            label->setPos(
                pos.x - textRect.width() / 2,
                pos.y - NODE_RADIUS * 2 - 10
            );
        }

        rootNode = rootNode->getSibling();
    }

    // Draw connections
    root = head;
    while (root) {
        drawConnections(root, positions);
        root = root->getSibling();
    }
}

template <typename T>
void HeapVisualizerT<T>::drawNode(BinomialNode<T>* node, const NodePosition& pos, bool animate) {
    if (!node) return;
    
    NodeItemT<T>* item = new NodeItemT<T>(node->getValue(), pos.x, pos.y);
    scene->addItem(item);
    nodeItemMap[node] = item;
}

template <typename T>
void HeapVisualizerT<T>::drawConnections(
    BinomialNode<T>* node,
    const QMap<BinomialNode<T>*, NodePosition>& positions) {

    if (!node) return;

    NodePosition parentPos = positions[node];

    // Parent -> children
    BinomialNode<T>* child = node->getChild();
    bool firstChild = true;

    while (child) {
        if (positions.contains(child)) {
            NodePosition childPos = positions[child];

            ArrowItem* arrow;

            if (firstChild) {
                // Bidirectional for first child
                arrow = new ArrowItem(
                    QPointF(parentPos.x, parentPos.y),
                    QPointF(childPos.x, childPos.y),
                    true
                );
            }
            else {
                // Other children point TO parent
                arrow = new ArrowItem(
                    QPointF(childPos.x, childPos.y),
                    QPointF(parentPos.x, parentPos.y),
                    false
                );
            }

            scene->addItem(arrow);
            arrowItems.append(arrow);

            // Recurse
            drawConnections(child, positions);

            firstChild = false;
        }
        child = child->getSibling();
    }

    // Sibling -> sibling (same level)
    BinomialNode<T>* sibling = node->getSibling();
    if (sibling && positions.contains(sibling)) {
        NodePosition pos1 = positions[node];
        NodePosition pos2 = positions[sibling];

        ArrowItem* sibArrow = new ArrowItem(
            QPointF(pos1.x, pos1.y),
            QPointF(pos2.x, pos2.y),
            false
        );

        scene->addItem(sibArrow);
        arrowItems.append(sibArrow);
    }
}

template <typename T>
void HeapVisualizerT<T>::highlightMinNode() {
    if (!binomialHeap) return;
    
    try {
        T minValue = binomialHeap->getMin();
        
        // Find the node item with min value
        for (auto it = nodeItemMap.begin(); it != nodeItemMap.end(); ++it) {
            NodeItemT<T>* item = it.value();
            if (item->getValue() == minValue) {
                // Clear previous highlight
                if (currentHighlightedNode) {
                    currentHighlightedNode->setHighlighted(false);
                }
                
                // Highlight new node
                item->setHighlighted(true);
                currentHighlightedNode = item;
                
                // Start timer to remove highlight after defined duration
                highlightTimer->start(HIGHLIGHT_DURATION_MS);
                break;
            }
        }
    } catch (const std::exception& e) {
        // Heap is empty, nothing to highlight
    }
}

template <typename T>
void HeapVisualizerT<T>::animateNodeToPosition(NodeItemT<T>* item, QPointF targetPos) {
    item->setPos(targetPos);
}

template <typename T>
NodeItemT<T>* HeapVisualizerT<T>::findNodeItemAt(QPointF scenePos) {
    QList<QGraphicsItem*> items = scene->items(scenePos);
    for (QGraphicsItem* item : items) {
        NodeItemT<T>* nodeItem = dynamic_cast<NodeItemT<T>*>(item);
        if (nodeItem) {
            return nodeItem;
        }
    }
    return nullptr;
}

template <typename T>
void HeapVisualizerT<T>::contextMenuEvent(QContextMenuEvent* event) {
    QPointF scenePos = mapToScene(event->pos());
    NodeItemT<T>* nodeItem = findNodeItemAt(scenePos);
    
    if (nodeItem) {
        QMenu menu(this);
        QAction* deleteAction = menu.addAction("Delete Node Value");
        
        QAction* selected = menu.exec(event->globalPos());
        if (selected == deleteAction) {
            onNodeRightClicked(nodeItem->getValue());
        }
    } else {
        QGraphicsView::contextMenuEvent(event);
    }
}

template <typename T>
void HeapVisualizerT<T>::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        NodeItemT<T>* nodeItem = findNodeItemAt(scenePos);
        
        if (nodeItem) {
            // Find the corresponding BinomialNode
            BinomialNode<T>* clickedNode = nullptr;
            for (auto it = nodeItemMap.begin(); it != nodeItemMap.end(); ++it) {
                if (it.value() == nodeItem) {
                    clickedNode = it.key();
                    break;
                }
            }
            
            // Check if it's a root node
            if (clickedNode && isRootNode(clickedNode)) {
                // Show context menu for duplicating
                QMenu menu(this);
                QAction* duplicateAction = menu.addAction("Duplicate Root Tree");
                
                QAction* selected = menu.exec(event->globalPos());
                if (selected == duplicateAction) {
                    onRootNodeLeftClicked(nodeItem->getValue());
                }
                return;
            }
        }
    }
    
    // Call base implementation for other cases
    QGraphicsView::mousePressEvent(event);
}

template <typename T>
bool HeapVisualizerT<T>::isRootNode(BinomialNode<T>* node) {
    if (!node || !binomialHeap) return false;
    
    // A root node is one that appears in the sibling list starting from head
    BinomialNode<T>* head = binomialHeap->getHead();
    BinomialNode<T>* current = head;
    
    while (current) {
        if (current == node) {
            return true;
        }
        current = current->getSibling();
    }
    
    return false;
}

// Type alias for backwards compatibility
using NodeItem = NodeItemT<int>;

#endif // HEAPVISUALIZER_H
