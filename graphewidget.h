#ifndef GRAPHEWIDGET_H
#define GRAPHEWIDGET_H

#include <QGraphicsView>

class Node;
class Edge;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = nullptr, int numberOfNodes_p=5, int numberOfEdges_p=7);
    GraphWidget(GraphWidget&);
    virtual ~GraphWidget();
    GraphWidget(QVector<Node*>, QVector<Edge*>);
    void itemMoved();

    int getNbNodesCover();
    int getNbEdgesCover();
    void setNbNodesSelected(int);
    void setNbEdgesCover(int);

    std::string getIdGraph();
    void setIdGraph(std::string);

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void success();

	Node* nodeMaxDegree();
    QVector<Edge*> getEdges();
    QVector<Node*> getNodes();
    QVector<Node*> getRemoveNodes();
	void delNode(Node*);

protected:
    void generateRandomNodes(QGraphicsScene*, int numberOfNodes_p);
    void generateRandomEdges(QGraphicsScene*, int numberOfEdges_p);

    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);
private:
    QVector<Node*>lNodes_m;
    QVector<Node*>removeNodes_m;
    QVector<Edge*>lEdges_m;
    int timerId = 0;
    int edgesCover_m = 0;
    int nodesCover_m = 0;
    std::string  idGraph_m = "";
};

inline QVector<Edge*> GraphWidget::getEdges(){return lEdges_m;}
inline QVector<Node*> GraphWidget::getNodes(){return lNodes_m;}
inline QVector<Node*> GraphWidget::getRemoveNodes(){return removeNodes_m;}
inline int GraphWidget::getNbNodesCover(){return nodesCover_m;}
inline int GraphWidget::getNbEdgesCover(){return edgesCover_m;}
inline std::string GraphWidget::getIdGraph(){return idGraph_m;}
inline void GraphWidget::setIdGraph(std::string idGraph_p){idGraph_m = idGraph_p;}


#endif // GRAPHWIDGET_H
