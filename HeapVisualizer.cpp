#include "HeapVisualizer.h"
#include <QPainter>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <cmath>

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
