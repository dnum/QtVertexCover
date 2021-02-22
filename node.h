#ifndef NODE_H
#define NODE_H


#include <QGraphicsItem>
#include <QVector>

class Edge;
class GraphWidget;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget);
    Node(Node *node);

    void addEdge(Edge *edge);
   QVector<Edge *> edges() const;
    int getId();
    void setId(int);
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

   // void calculateForces();
    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVector<Node*> getNeighborhood();
	void addNeighbour(Node*);
    Qt::GlobalColor getColor();
	void setColor(Qt::GlobalColor);
	void setDarkColor(Qt::GlobalColor);
public Q_SLOTS:
    void selectNode();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<Edge *> edgeList_m;
    QVector<Node*>neighbourList_m;
    QPointF newPos_m;
    GraphWidget *graph_m;
    Qt::GlobalColor color_m = Qt::yellow;
	Qt::GlobalColor darkColor_m = Qt::darkYellow;
    int id;
};
inline void Node::setColor(Qt::GlobalColor color_p){color_m = color_p;}
inline Qt::GlobalColor Node::getColor() {return color_m;}
inline void Node::setDarkColor(Qt::GlobalColor darkColor_p){darkColor_m = darkColor_p; }
#endif // NODE_H
