#include "HeapVisualizer.h"
#include <QPainter>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <cmath>

// NodeItem implementation
NodeItem::NodeItem(int value, qreal x, qreal y, qreal radius)
    : QGraphicsEllipseItem(x - radius, y - radius, radius * 2, radius * 2), 
      intNodeValue(value), charNodeValue('\0'), isInt(true) {
    initialize(QString::number(value), x, y, radius);
}

NodeItem::NodeItem(char value, qreal x, qreal y, qreal radius)
    : QGraphicsEllipseItem(x - radius, y - radius, radius * 2, radius * 2), 
      intNodeValue(0), charNodeValue(value), isInt(false) {
    initialize(QString(value), x, y, radius);
}

void NodeItem::initialize(const QString& displayText, qreal x, qreal y, qreal radius) {
    // Set node color - light blue
    setBrush(QBrush(QColor(173, 216, 230)));
    normalPen = QPen(Qt::black, 2);
    highlightPen = QPen(Qt::red, 4);
    setPen(normalPen);
    
    // Add text for value
    textItem = new QGraphicsTextItem(displayText, this);
    QFont font = textItem->font();
    font.setPointSize(12);
    font.setBold(true);
    textItem->setFont(font);
    
    // Center text within the ellipse
    QRectF textRect = textItem->boundingRect();
    textItem->setPos(radius - textRect.width() / 2, radius - textRect.height() / 2);
}

void NodeItem::setHighlighted(bool highlight) {
    setPen(highlight ? highlightPen : normalPen);
}

// ArrowItem implementation
ArrowItem::ArrowItem(QPointF start, QPointF end, bool doubleArrow)
    : QGraphicsLineItem(QLineF(start, end)), isDoubleArrow(doubleArrow) {
    setPen(QPen(Qt::black, 2));
}

void ArrowItem::updatePosition(QPointF start, QPointF end) {
    setLine(QLineF(start, end));
}

QPolygonF ArrowItem::createArrowHead(QPointF start, QPointF end) {
    const qreal arrowSize = 10;
    QLineF line(start, end);
    
    qreal angle = std::atan2(-line.dy(), line.dx());
    
    QPointF arrowP1 = end - QPointF(std::sin(angle + M_PI / 3) * arrowSize,
                                     std::cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = end - QPointF(std::sin(angle + M_PI - M_PI / 3) * arrowSize,
                                     std::cos(angle + M_PI - M_PI / 3) * arrowSize);
    
    QPolygonF arrowHead;
    arrowHead << end << arrowP1 << arrowP2;
    return arrowHead;
}

void ArrowItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setPen(pen());
    painter->setBrush(Qt::black);
    
    QLineF line = this->line();
    painter->drawLine(line);
    
    // Draw arrowhead at end
    QPolygonF arrowHead = createArrowHead(line.p1(), line.p2());
    painter->drawPolygon(arrowHead);
    
    // Draw arrowhead at start if double arrow
    if (isDoubleArrow) {
        QPolygonF arrowHead2 = createArrowHead(line.p2(), line.p1());
        painter->drawPolygon(arrowHead2);
    }
}

// HeapVisualizer implementation
HeapVisualizer::HeapVisualizer(QWidget* parent)
    : QGraphicsView(parent), intHeap(nullptr), charHeap(nullptr), 
      currentHeapType(INT_HEAP), currentHighlightedNode(nullptr) {
    
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
    connect(highlightTimer, &QTimer::timeout, this, [this]() {
        if (currentHighlightedNode) {
            currentHighlightedNode->setHighlighted(false);
            currentHighlightedNode = nullptr;
        }
    });
}

HeapVisualizer::~HeapVisualizer() {
    clearScene();
}

void HeapVisualizer::setHeap(BinomialHeap<int>* heap) {
    intHeap = heap;
    charHeap = nullptr;
    currentHeapType = INT_HEAP;
    updateVisualization(false);
}

void HeapVisualizer::setHeap(BinomialHeap<char>* heap) {
    charHeap = heap;
    intHeap = nullptr;
    currentHeapType = CHAR_HEAP;
    updateVisualization(false);
}

void HeapVisualizer::clearScene() {
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

template<typename T>
void HeapVisualizer::calculateSubtreeWidthT(BinomialNode<T>* node, qreal& width) {
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
        width = MIN_NODE_WIDTH; // Minimum width for leaf nodes
        return;
    }
    
    // Calculate total width needed for all children
    qreal totalWidth = 0;
    child = node->getChild();
    while (child) {
        qreal childWidth = 0;
        calculateSubtreeWidthT(child, childWidth);
        totalWidth += childWidth;
        child = child->getSibling();
    }
    
    width = std::max(totalWidth, MIN_NODE_WIDTH);
}

template<typename T>
void HeapVisualizer::calculateLayoutT(BinomialNode<T>* root, qreal& currentX, qreal y,
                                     QMap<void*, NodePosition>& positions) {
    if (!root) return;
    
    qreal subtreeWidth = 0;
    calculateSubtreeWidthT(root, subtreeWidth);
    
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
            calculateLayoutT(child, childX, childY, positions);
            child = child->getSibling();
        }
    }
    
    currentX += subtreeWidth;
}

void HeapVisualizer::updateVisualization(bool animate) {
    clearScene();
    
    if (currentHeapType == INT_HEAP && intHeap) {
        BinomialNode<int>* head = intHeap->getHead();
        if (!head) return;
        
        // Calculate positions for all nodes
        QMap<void*, NodePosition> positions;
        qreal currentX = 50;
        qreal rootY = 50;
        
        // Process each binomial tree root
        BinomialNode<int>* root = head;
        while (root) {
            calculateLayoutT(root, currentX, rootY, positions);
            currentX += TREE_GAP; // Gap between trees
            root = root->getSibling();
        }
        
        // Draw all nodes
        for (auto it = positions.begin(); it != positions.end(); ++it) {
            drawNodeT(static_cast<BinomialNode<int>*>(it.key()), it.value(), animate);
        }
        
        // Draw connections
        root = head;
        while (root) {
            drawConnectionsT(root, positions);
            root = root->getSibling();
        }
    } else if (currentHeapType == CHAR_HEAP && charHeap) {
        BinomialNode<char>* head = charHeap->getHead();
        if (!head) return;
        
        // Calculate positions for all nodes
        QMap<void*, NodePosition> positions;
        qreal currentX = 50;
        qreal rootY = 50;
        
        // Process each binomial tree root
        BinomialNode<char>* root = head;
        while (root) {
            calculateLayoutT(root, currentX, rootY, positions);
            currentX += TREE_GAP; // Gap between trees
            root = root->getSibling();
        }
        
        // Draw all nodes
        for (auto it = positions.begin(); it != positions.end(); ++it) {
            drawNodeT(static_cast<BinomialNode<char>*>(it.key()), it.value(), animate);
        }
        
        // Draw connections
        root = head;
        while (root) {
            drawConnectionsT(root, positions);
            root = root->getSibling();
        }
    }
}

template<typename T>
void HeapVisualizer::drawNodeT(BinomialNode<T>* node, const NodePosition& pos, bool animate) {
    if (!node) return;
    
    NodeItem* item;
    if constexpr (std::is_same_v<T, int>) {
        item = new NodeItem(node->getValue(), pos.x, pos.y);
    } else {
        item = new NodeItem(node->getValue(), pos.x, pos.y);
    }
    scene->addItem(item);
    nodeItemMap[node] = item;
    
    if (animate) {
        // Could add animation here
    }
}

template<typename T>
void HeapVisualizer::drawConnectionsT(BinomialNode<T>* node,
                                     const QMap<void*, NodePosition>& positions) {
    if (!node) return;
    
    NodePosition parentPos = positions[node];
    
    // Draw parent-child connections (double arrows)
    BinomialNode<T>* child = node->getChild();
    while (child) {
        if (positions.contains(child)) {
            NodePosition childPos = positions[child];
            
            ArrowItem* arrow = new ArrowItem(
                QPointF(parentPos.x, parentPos.y),
                QPointF(childPos.x, childPos.y),
                true  // double arrow for parent-child
            );
            scene->addItem(arrow);
            arrowItems.append(arrow);
            
            // Recursively draw connections for child
            drawConnectionsT(child, positions);
        }
        child = child->getSibling();
    }
    
    // Draw sibling connections (single arrow)
    BinomialNode<T>* sibling = node->getSibling();
    if (sibling && positions.contains(sibling)) {
        NodePosition siblingPos = positions[sibling];
        
        // Only draw sibling arrows at root level (avoid duplicates in subtrees)
        if (!node->getParent()) {
            ArrowItem* arrow = new ArrowItem(
                QPointF(parentPos.x, parentPos.y),
                QPointF(siblingPos.x, siblingPos.y),
                false  // single arrow for siblings
            );
            scene->addItem(arrow);
            arrowItems.append(arrow);
        }
    }
}

void HeapVisualizer::highlightMinNode() {
    try {
        // Find the node item with min value
        for (auto it = nodeItemMap.begin(); it != nodeItemMap.end(); ++it) {
            NodeItem* item = it.value();
            bool isMinNode = false;
            
            if (currentHeapType == INT_HEAP && intHeap && item->isIntType()) {
                int minValue = intHeap->getMin();
                if (item->getIntValue() == minValue) {
                    isMinNode = true;
                }
            } else if (currentHeapType == CHAR_HEAP && charHeap && !item->isIntType()) {
                char minValue = charHeap->getMin();
                if (item->getCharValue() == minValue) {
                    isMinNode = true;
                }
            }
            
            if (isMinNode) {
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

void HeapVisualizer::animateNodeToPosition(NodeItem* item, QPointF targetPos) {
    // Note: Direct animation of QGraphicsItem position would require 
    // using QGraphicsItemAnimation or custom animation
    // For now, we'll do instant positioning as animation is complex with QGraphicsItem
    // A full implementation would use QGraphicsItemAnimation or custom QObject wrapper
    item->setPos(targetPos);
}

NodeItem* HeapVisualizer::findNodeItemAt(QPointF scenePos) {
    QList<QGraphicsItem*> items = scene->items(scenePos);
    for (QGraphicsItem* item : items) {
        NodeItem* nodeItem = dynamic_cast<NodeItem*>(item);
        if (nodeItem) {
            return nodeItem;
        }
    }
    return nullptr;
}

void HeapVisualizer::contextMenuEvent(QContextMenuEvent* event) {
    QPointF scenePos = mapToScene(event->pos());
    NodeItem* nodeItem = findNodeItemAt(scenePos);
    
    if (nodeItem) {
        QMenu menu(this);
        QAction* deleteAction = menu.addAction("Delete Node Value");
        
        QAction* selected = menu.exec(event->globalPos());
        if (selected == deleteAction) {
            if (nodeItem->isIntType()) {
                emit nodeRightClicked(nodeItem->getIntValue());
            } else {
                emit nodeRightClicked(static_cast<int>(nodeItem->getCharValue()));
            }
        }
    } else {
        QGraphicsView::contextMenuEvent(event);
    }
}

// Explicit template instantiations
template void HeapVisualizer::calculateSubtreeWidthT<int>(BinomialNode<int>*, qreal&);
template void HeapVisualizer::calculateSubtreeWidthT<char>(BinomialNode<char>*, qreal&);
template void HeapVisualizer::calculateLayoutT<int>(BinomialNode<int>*, qreal&, qreal, QMap<void*, NodePosition>&);
template void HeapVisualizer::calculateLayoutT<char>(BinomialNode<char>*, qreal&, qreal, QMap<void*, NodePosition>&);
template void HeapVisualizer::drawNodeT<int>(BinomialNode<int>*, const NodePosition&, bool);
template void HeapVisualizer::drawNodeT<char>(BinomialNode<char>*, const NodePosition&, bool);
template void HeapVisualizer::drawConnectionsT<int>(BinomialNode<int>*, const QMap<void*, NodePosition>&);
template void HeapVisualizer::drawConnectionsT<char>(BinomialNode<char>*, const QMap<void*, NodePosition>&);
