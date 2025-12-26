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
#include "binomial_heap.hpp"

// Custom ellipse item that stores node value
class NodeItem : public QGraphicsEllipseItem {
public:
    NodeItem(int value, qreal x, qreal y, qreal radius = 25);
    NodeItem(char value, qreal x, qreal y, qreal radius = 25);
    int getIntValue() const { return intNodeValue; }
    char getCharValue() const { return charNodeValue; }
    bool isIntType() const { return isInt; }
    void setHighlighted(bool highlight);
    
private:
    int intNodeValue;
    char charNodeValue;
    bool isInt;
    QGraphicsTextItem* textItem;
    QPen normalPen;
    QPen highlightPen;
    
    void initialize(const QString& displayText, qreal x, qreal y, qreal radius);
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

class HeapVisualizer : public QGraphicsView {
    Q_OBJECT
    
public:
    explicit HeapVisualizer(QWidget* parent = nullptr);
    ~HeapVisualizer();
    
    void setHeap(BinomialHeap<int>* heap);
    void setHeap(BinomialHeap<char>* heap);
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
    
signals:
    void nodeRightClicked(int value);
    
protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    
private:
    enum HeapType {
        INT_HEAP,
        CHAR_HEAP
    };
    
    struct NodePosition {
        qreal x;
        qreal y;
        void* node;  // Pointer to either BinomialNode<int>* or BinomialNode<char>*
    };
    
    BinomialHeap<int>* intHeap;
    BinomialHeap<char>* charHeap;
    HeapType currentHeapType;
    QGraphicsScene* scene;
    QTimer* highlightTimer;
    NodeItem* currentHighlightedNode;
    
    QMap<void*, NodeItem*> nodeItemMap;
    QList<ArrowItem*> arrowItems;
    
    // Layout calculation (templated)
    template<typename T>
    void calculateLayoutT(BinomialNode<T>* root, qreal& currentX, qreal y, 
                         QMap<void*, NodePosition>& positions);
    template<typename T>
    void calculateSubtreeWidthT(BinomialNode<T>* node, qreal& width);
    
    // Drawing methods (templated)
    template<typename T>
    void drawNodeT(BinomialNode<T>* node, const NodePosition& pos, bool animate);
    template<typename T>
    void drawConnectionsT(BinomialNode<T>* node, 
                        const QMap<void*, NodePosition>& positions);
    void clearScene();
    
    // Animation
    void animateNodeToPosition(NodeItem* item, QPointF targetPos);
    
    // Helper
    NodeItem* findNodeItemAt(QPointF scenePos);
};

#endif // HEAPVISUALIZER_H
