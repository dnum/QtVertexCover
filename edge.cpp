#include "edge.h"
#include "node.h"

#include <QPainter>
#include <QtMath>
#include <iostream>

Edge::Edge()
{
}
Edge::Edge(Node *sourceNode_p, Node *destNode_p)
    : source_m(sourceNode_p), dest_m(destNode_p)
{
    setAcceptedMouseButtons(Qt::NoButton);
    source_m->addEdge(this);
    dest_m->addEdge(this);
    adjust();
}

Node *Edge::sourceNode() const
{
    return source_m;
}

Node *Edge::destNode() const
{
    return dest_m;
}

void Edge::adjust()
{
    if (!source_m || !dest_m)
        return;

    QLineF line(mapFromItem(source_m, 0, 0), mapFromItem(dest_m, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint_m = line.p1() + edgeOffset;
        destPoint_m = line.p2() - edgeOffset;
    } else {
        sourcePoint_m = destPoint_m = line.p1();
    }
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source_m || !dest_m)
        return;

    QLineF line(sourcePoint_m, destPoint_m);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(color_m, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint_m + QPointF(sin(angle + M_PI / 3) * arrowSize_m,
                                                  cos(angle + M_PI / 3) * arrowSize_m);
    QPointF sourceArrowP2 = sourcePoint_m + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize_m,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize_m);
    QPointF destArrowP1 = destPoint_m + QPointF(sin(angle - M_PI / 3) * arrowSize_m,
                                              cos(angle - M_PI / 3) * arrowSize_m);
    QPointF destArrowP2 = destPoint_m + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize_m,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize_m);
    painter->setBrush(color_m);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

QRectF Edge::boundingRect() const
{
    if (!source_m || !dest_m)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize_m) / 2.0;

    return QRectF(sourcePoint_m, QSizeF(destPoint_m.x() - sourcePoint_m.x(),
                                      destPoint_m.y() - sourcePoint_m.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
