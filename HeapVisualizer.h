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
    int getValue() const { return nodeValue; }
    void setHighlighted(bool highlight);
    
private:
    int nodeValue;
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

class HeapVisualizer : public QGraphicsView {
    Q_OBJECT
    
public:
    explicit HeapVisualizer(QWidget* parent = nullptr);
    ~HeapVisualizer();
    
    void setHeap(BinomialHeap<int>* heap);
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
    struct NodePosition {
        qreal x;
        qreal y;
        BinomialNode<int>* node;
    };
    
    BinomialHeap<int>* binomialHeap;
    QGraphicsScene* scene;
    QTimer* highlightTimer;
    NodeItem* currentHighlightedNode;
    
    QMap<BinomialNode<int>*, NodeItem*> nodeItemMap;
    QList<ArrowItem*> arrowItems;
    
    // Layout calculation
    void calculateLayout(BinomialNode<int>* root, qreal& currentX, qreal y, 
                         QMap<BinomialNode<int>*, NodePosition>& positions);
    void calculateSubtreeWidth(BinomialNode<int>* node, qreal& width);
    
    // Drawing methods
    void drawNode(BinomialNode<int>* node, const NodePosition& pos, bool animate);
    void drawConnections(BinomialNode<int>* node, 
                        const QMap<BinomialNode<int>*, NodePosition>& positions);
    void clearScene();
    
    // Animation
    void animateNodeToPosition(NodeItem* item, QPointF targetPos);
    
    // Helper
    NodeItem* findNodeItemAt(QPointF scenePos);
};

#endif // HEAPVISUALIZER_H
