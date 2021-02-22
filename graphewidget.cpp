#include "graphewidget.h"

#include "edge.h"
#include "node.h"
#include "mainwindow.h"

#include <iostream>
#include <math.h>

#include <QtWidgets/QLineEdit>
#include <QKeyEvent>
#include <QMessageBox>
//#include <QRandomGenerator>

//! [0]
GraphWidget::GraphWidget(QWidget *parent, int numberOfNodes_p, int numberOfEdges_p)
    : QGraphicsView(parent)
{
    QGraphicsScene* scene_l = new QGraphicsScene(this);
    scene_l->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene_l->setSceneRect(-500, -500, 700, 700);
    setScene(scene_l);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Random generated graph"));

    if(numberOfEdges_p > numberOfNodes_p*(numberOfNodes_p-1)/2)
        numberOfEdges_p = numberOfNodes_p*(numberOfNodes_p-1)/2;
    generateRandomNodes(scene_l,numberOfNodes_p);
    generateRandomEdges(scene_l,numberOfEdges_p);
}

GraphWidget::GraphWidget(GraphWidget& graph_p)
{
    lNodes_m=graph_p.lNodes_m;
    lEdges_m=graph_p.lEdges_m;
    removeNodes_m=graph_p.removeNodes_m;
    idGraph_m=graph_p.idGraph_m;
}

GraphWidget::~GraphWidget()
{
}
GraphWidget::GraphWidget(QVector<Node*>vNodes_m_p, QVector<Edge*>vEdges_p)
{
    lNodes_m=vNodes_m_p;
    lEdges_m=vEdges_p;
}

void GraphWidget::generateRandomNodes(QGraphicsScene*scene_p,int numberOfNodes_p)
{
    srand (time(NULL));
    for( int indice_l=0; indice_l < numberOfNodes_p; ++indice_l)
    {
        Node *node_l = new Node(this);
        node_l->setId(indice_l);
        //  node_l->setPos(-250 + QRandomGenerator::global()->bounded(400), -250 + QRandomGenerator::global()->bounded(400));
        node_l->setPos(-250 + rand()%400, -250 + rand()%400);
        scene_p->addItem(node_l);
        lNodes_m.push_back(node_l);
    }
}


void GraphWidget::generateRandomEdges(QGraphicsScene*scene_p,int numberOfEdges_p)
{
    int j,i=0,edge[numberOfEdges_p][2];
    while (i < numberOfEdges_p) {
        edge[i][0] = rand()%lNodes_m.size();
        edge[i][1] = rand()%lNodes_m.size();
        if(edge[i][1] == edge[i][0])
        {
            continue;
        }
        else
        {
            for(j=0;j<i;j++)
            {
                if( (edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] && edge[i][1] == edge[j][0]))
                    i--;
            }
        }
        i++;
    }
    for(i = 0; i< lNodes_m.size();++i)
    {
        Node*nodeOne_l = lNodes_m[i];
        for(j=0;j < numberOfEdges_p; ++j)
        {
            if(edge[j][0] == i)
            {
                Node*nodeTwo_l = lNodes_m[edge[j][1]];
                Edge *edge_l = new Edge(nodeOne_l, nodeTwo_l);
                lEdges_m.push_back(edge_l);
                nodeOne_l->addNeighbour(nodeTwo_l);
                nodeTwo_l->addNeighbour(nodeOne_l);
                scene_p->addItem(edge_l);
            }
        }
    }
}
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    /*    Q_UNUSED(event);

    QVector<Node *> nodes;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }*/
}

#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
 /*   QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);*/

    // Fill
    /*QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);*/

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click on a minimum number of nodes to cover all edges "));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    srand (time(NULL));
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
        {
            //   item->setPos(-250 + QRandomGenerator::global()->bounded(400), -250 + QRandomGenerator::global()->bounded(400));
            item->setPos(-250 + rand()%400, -250 + rand()%400);
        }
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
    //scene()->setSceneRect(-500*1.2, -500*1.2, 700*1.2, 700*1.2);
}

void GraphWidget::success()
{

}

Node* GraphWidget::nodeMaxDegree()
{
    if(!lNodes_m.size())
        return nullptr;
    Node*maxNode_l=lNodes_m[0];
    for(int id_l=1; id_l < lNodes_m.size(); ++id_l)
    {
        if(maxNode_l->getNeighborhood().size()< lNodes_m[id_l]->getNeighborhood().size())
            maxNode_l=lNodes_m[id_l];
    }
    return maxNode_l;
}

void GraphWidget::delNode(Node*pNode_p)
{
    removeNodes_m.push_back(pNode_p);
    for(auto node_l : pNode_p->getNeighborhood())
    {
        int i=0;
        for(auto edge_l : node_l->edges())
        {
            if(edge_l->sourceNode() == pNode_p || edge_l->destNode() == pNode_p)
            {
                break;
            }
            ++i;
        }
        node_l->edges().remove(i);

        i=0;


        for(auto idNode_l : node_l->getNeighborhood())
        {
            if(idNode_l && idNode_l == pNode_p)
            {
                std::cout<<node_l->getId()<<" have "<<node_l->getNeighborhood().size()<<"  nodes so remove "<<node_l->getNeighborhood()[i]->getId()<<std::endl;
                node_l->getNeighborhood().remove(i);
                std::cout<<" Now have "<<node_l->getNeighborhood().size()<<" or "<<node_l->getNeighborhood().length()<<" nodes "<<std::endl;
                break;
            }
            else ++i;
        }
    }

    for(int id_l=0; id_l < lNodes_m.size(); ++id_l)
    {
        if(lNodes_m[id_l] == pNode_p)
        {
            lNodes_m.remove(id_l);
            break;
        }
    }

    //int i=0;
    std::cout<<" NbEdge="<<lEdges_m.size()<<std::endl;
    for(int i=0; i<lEdges_m.size();)
    {
        if(lEdges_m[i]->sourceNode() == pNode_p || lEdges_m[i]->destNode() == pNode_p)
            lEdges_m.remove(i);
        else i++;
    }

    pNode_p->edges().clear();

}

void GraphWidget::setNbNodesSelected(int nodesCover_p){
    nodesCover_m = nodesCover_p;
    static_cast<MainWindow*>(parent())->getNumberOfNodesCover()->setText(QString::number(nodesCover_m));
}
void GraphWidget::setNbEdgesCover(int edgesCover_p){
    edgesCover_m = edgesCover_p;
    static_cast<MainWindow*>(parent())->getNumberOfEdgesCover()->setText(QString::number(edgesCover_m));

    if(edgesCover_m == lEdges_m.size() && nodesCover_m <= static_cast<MainWindow*>(parent())->getpSolution()->getSolution().size())
    {
        std::cout<<"Congrats, you found an optimal solution for the problem of Vertex Cover !!!"<<std::endl;
       /* QMessageBox msgBox_l;
        msgBox_l.setText("Congrats, you found an optimal solution for the problem of Vertex Cover !!!");
        msgBox_l.exec();*/
        QMessageBox::about(this,"Vertex Cover","Congrats, you found an optimal solution for the problem of Vertex Cover !!!");
    }
}
