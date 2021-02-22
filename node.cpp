#include "node.h"

#include "edge.h"
#include "graphewidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QLCDNumber>
#include <QStaticText>
#include <iostream>

Node::Node(GraphWidget *graphWidget)
    : graph_m(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Node::Node(Node *node)
{
    edgeList_m = node->edgeList_m;
    neighbourList_m = node->neighbourList_m;
    newPos_m = node->newPos_m;
    graph_m = node->graph_m;
    color_m = node->color_m;
    darkColor_m = node->darkColor_m;
}

void Node::addEdge(Edge *edge)
{
    edgeList_m.push_back(edge);
    edge->adjust();
}

QVector<Edge*> Node::edges() const
{
    return edgeList_m;
}

int Node::getId()
{
    return id;
}

void Node::setId(int id_p)
{
    id = id_p;
}

bool Node::advancePosition()
{
    if (newPos_m == pos())
        return false;

    setPos(newPos_m);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(color_m).lighter(120));
        gradient.setColorAt(0, QColor(darkColor_m).lighter(120));
    } else {
        gradient.setColorAt(0, color_m);
        gradient.setColorAt(1, darkColor_m);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
    QString id_l= QString::number(id);
    painter->drawText(newPos_m,id_l);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edgeList_m))
            edge->adjust();
        graph_m->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void Node::selectNode()
{
    int nbNodeCover_l= graph_m->getNbNodesCover();
    int nbEdgesCover_l = graph_m->getNbEdgesCover();
    if(color_m == Qt::yellow)
	{
        nbNodeCover_l++;

		color_m = Qt::red;
		darkColor_m = Qt::darkRed;
		for(Edge* edge_l : edgeList_m)
		{
            if(edge_l->getColor() == Qt::black)
                nbEdgesCover_l++;
			edge_l->setColor(Qt::blue);
			edge_l->update();
		}
	}
    else {
        if(color_m == Qt::red)
            nbNodeCover_l--;
		color_m = Qt::yellow;
		darkColor_m = Qt::darkYellow;
		for(Edge* edge_l : edgeList_m)
		{
            if((edge_l->destNode()!=this && edge_l->destNode()->color_m == Qt::yellow) || (edge_l->sourceNode()!=this && edge_l->sourceNode()->color_m == Qt::yellow))
            {
                if(edge_l->getColor() == Qt::blue)
                    nbEdgesCover_l--;
                edge_l->setColor(Qt::black);
                edge_l->update();
            }
		}
	}
    graph_m->setNbNodesSelected(nbNodeCover_l);
    graph_m->setNbEdgesCover(nbEdgesCover_l);
	/*QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(color_m).lighter(120));
        gradient.setColorAt(0, QColor(Qt::darkColor_m).lighter(120));
    } else {
        gradient.setColorAt(0, Qt::color_m);
        gradient.setColorAt(1, Qt::darkColor_m);
    }
    this->setBrush(gradient);*/
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	selectNode();
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout<<"Node of id "<<id<<std::endl;
    QGraphicsItem::mouseDoubleClickEvent(event);
}


QVector<Node*> Node::getNeighborhood()
{
	return neighbourList_m;
}

void Node::addNeighbour(Node*pNode_p)
{
    neighbourList_m.push_back(pNode_p);
}
