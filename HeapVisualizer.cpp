#include "HeapVisualizer.h"
#include <QPainter>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <cmath>


// NodeItem implementation
NodeItem::NodeItem(int value, qreal x, qreal y, qreal radius) : QGraphicsEllipseItem(x - radius, y - radius, radius * 2, radius * 2), nodeValue(value) {

    // Set node color - light blue
    setBrush(QBrush(QColor(173, 216, 230)));
    normalPen = QPen(Qt::black, 2);
    highlightPen = QPen(Qt::red, 4);
    setPen(normalPen);

    // Add text for value
    textItem = new QGraphicsTextItem(QString::number(value), this);
    QFont font = textItem->font();
    font.setPointSize(12);
    font.setBold(true);
    textItem->setFont(font);

    // Center text inside the ellipse using local coordinates
    QRectF ellipseRect = rect();          // local rect of ellipse
    QRectF textRect = textItem->boundingRect();

    textItem->setPos(
        ellipseRect.center().x() - textRect.width() / 2,
        ellipseRect.center().y() - textRect.height() / 2);
};

void NodeItem::setHighlighted(bool highlight) {
    setPen(highlight ? highlightPen : normalPen);
}

static constexpr qreal NODE_RADIUS = 25.0; // must match NodeItem radius


// ArrowItem implementation
ArrowItem::ArrowItem(QPointF start, QPointF end, bool doubleArrow)
    : QGraphicsLineItem(QLineF(start, end)), isDoubleArrow(doubleArrow) {
    QLineF centerLine(start, end);

    if (centerLine.length() > 2 * NODE_RADIUS) {
        QPointF unit = (end - start) / centerLine.length();
        QPointF newStart = start + unit * NODE_RADIUS;
        QPointF newEnd = end - unit * NODE_RADIUS;
        setLine(QLineF(newStart, newEnd));
    }
    else {
        setLine(centerLine);
    }

    setPen(QPen(Qt::black, 2));
}

void ArrowItem::updatePosition(QPointF start, QPointF end) {
    QLineF centerLine(start, end);

    if (centerLine.length() > 2 * NODE_RADIUS) {
        QPointF unit = (end - start) / centerLine.length();
        QPointF newStart = start + unit * NODE_RADIUS;
        QPointF newEnd = end - unit * NODE_RADIUS;
        setLine(QLineF(newStart, newEnd));
    }
    else {
        setLine(centerLine);
    }
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
    binomialHeap = heap;
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

void HeapVisualizer::calculateSubtreeWidth(BinomialNode<int>* node, qreal& width) {
    if (!node) {
        width = 0;
        return;
    }
    
    // Count children
    int childCount = 0;
    BinomialNode<int>* child = node->getChild();
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
        calculateSubtreeWidth(child, childWidth);
        totalWidth += childWidth;
        child = child->getSibling();
    }
    
    width = std::max(totalWidth, MIN_NODE_WIDTH);
}

void HeapVisualizer::calculateLayout(BinomialNode<int>* root, qreal& currentX, qreal y,
                                     QMap<BinomialNode<int>*, NodePosition>& positions) {
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
        
        BinomialNode<int>* child = root->getChild();
        while (child) {
            calculateLayout(child, childX, childY, positions);
            child = child->getSibling();
        }
    }
    
    currentX += subtreeWidth;
}

void HeapVisualizer::updateVisualization(bool animate) {
    if (!binomialHeap) return;
    
    clearScene();
    
    BinomialNode<int>* head = binomialHeap->getHead();
    if (!head) return;
    
    // Calculate positions for all nodes
    QMap<BinomialNode<int>*, NodePosition> positions;
    qreal currentX = 50;
    qreal rootY = 50;
    
    // Process each binomial tree root
    BinomialNode<int>* root = head;
    while (root) {
        calculateLayout(root, currentX, rootY, positions);
        currentX += TREE_GAP; // Gap between trees
        root = root->getSibling();
    }
    
    // Draw all nodes
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        drawNode(it.key(), it.value(), animate);
    }
    // ---- Draw head pointer ----
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

    // ---- Draw order labels (B0, B1, ...) above each root ----
    BinomialNode<int>* rootNode = head;

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

void HeapVisualizer::drawNode(BinomialNode<int>* node, const NodePosition& pos, bool animate) {
    if (!node) return;
    
    NodeItem* item = new NodeItem(node->getValue(), pos.x, pos.y);
    scene->addItem(item);
    nodeItemMap[node] = item;
    
    if (animate) {
        // Could add animation here
    }
}

void HeapVisualizer::drawConnections(
    BinomialNode<int>* node,
    const QMap<BinomialNode<int>*, NodePosition>& positions) {

    if (!node) return;

    NodePosition parentPos = positions[node];

    // ---- Parent -> children ----
    BinomialNode<int>* child = node->getChild();
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

    // ---- Sibling -> sibling (same level) ----
    // ---- Immediate sibling only ----
    BinomialNode<int>* sibling = node->getSibling();
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


void HeapVisualizer::highlightMinNode() {
    if (!binomialHeap) return;
    
    try {
        int minValue = binomialHeap->getMin();
        
        // Find the node item with min value
        for (auto it = nodeItemMap.begin(); it != nodeItemMap.end(); ++it) {
            NodeItem* item = it.value();
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
            emit nodeRightClicked(nodeItem->getValue());
        }
    } else {
        QGraphicsView::contextMenuEvent(event);
    }
}
