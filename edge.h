#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge();
    Edge(Node *sourceNode, Node *destNode);

    Node *sourceNode() const;
    Node *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }
	void setColor(Qt::GlobalColor);
    Qt::GlobalColor getColor();
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source_m, *dest_m;
	Qt::GlobalColor color_m = Qt::black;
    QPointF sourcePoint_m;
    QPointF destPoint_m;
    qreal arrowSize_m = 10;
};

inline void Edge::setColor(Qt::GlobalColor color_p){color_m = color_p;} 
inline Qt::GlobalColor Edge::getColor(){return color_m;}
#endif // EDGE_H
