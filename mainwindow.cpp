#include "mainwindow.h"

#include <iostream>
#include <string>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFontDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // Graph settings

    numberOfNodes_m = new QDoubleSpinBox();
    numberOfNodes_m->setMinimum(1);
    numberOfNodes_m->setMaximum(50);
    numberOfNodes_m->setSingleStep(1);
    numberOfNodes_m->setValue(5);

    numberOfEdges_m = new QDoubleSpinBox();
    numberOfEdges_m->setMinimum(0);
    numberOfEdges_m->setMaximum(1225);
    numberOfEdges_m->setSingleStep(1);
    numberOfEdges_m->setValue(7);

    //Create graph
    graphWidget_m = new GraphWidget(this,numberOfNodes_m->value(),numberOfEdges_m->value());

    nodesCover_m = new QLineEdit();
    nodesCover_m->setText(QString::number(graphWidget_m->getNbNodesCover()));
    nodesCover_m->setEnabled(false);

    edgesCover_m = new QLineEdit();
    edgesCover_m->setText(QString::number(graphWidget_m->getNbEdgesCover()));
    edgesCover_m->setEnabled(false);

    QPushButton *generateGraphButton_l = new QPushButton("Generate graph");
    QPushButton *showSolutionButton_l = new QPushButton("Show solution");

    QFormLayout *graphSettingsLayout_l = new QFormLayout();
    graphSettingsLayout_l->addRow("Number of nodes", numberOfNodes_m);
    graphSettingsLayout_l->addRow("Number of edges", numberOfEdges_m);
    graphSettingsLayout_l->addRow(generateGraphButton_l);
    graphSettingsLayout_l->addRow(showSolutionButton_l);

    graphSettingsLayout_l->addRow("Nodes selected", nodesCover_m);
    graphSettingsLayout_l->addRow("Edges covered", edgesCover_m);

    QGroupBox *graphSettings_l = new QGroupBox("New Graph");
    graphSettings_l->setLayout(graphSettingsLayout_l);


    //Connect button
    connect(generateGraphButton_l, &QPushButton::clicked, this, &MainWindow::createNewGraph);
    connect(showSolutionButton_l, &QPushButton::clicked, this, &MainWindow::showSolution);
  //  connect(graphWidget_m, &QMouseEvent::, this, &MainWindow::update);

    // create main layout
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(graphSettings_l);
    settingsLayout->addStretch();

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(settingsLayout, 0, 0);
    baseLayout->addWidget(graphWidget_m, 0, 1);
    setLayout(baseLayout);

    pSolution_m = new Solution(*graphWidget_m);
    pSolution_m->easySolve(*graphWidget_m, numberOfNodes_m->value());
}

void MainWindow::createNewGraph()
{
    graphWidget_m = new GraphWidget(nullptr,numberOfNodes_m->value(),numberOfEdges_m->value());
    nodesCover_m->setText(QString::number(graphWidget_m->getNbNodesCover()));
    edgesCover_m->setText(QString::number(graphWidget_m->getNbEdgesCover()));
    //layout()->removeWidget(graphWidget_m);
    static_cast<QGridLayout *>(layout())->addWidget(graphWidget_m, 0, 1);
    std::cout<<"NEW GRAPH"<<std::endl<<std::endl;
    pSolution_m = new Solution(*graphWidget_m);
    pSolution_m->easySolve(*graphWidget_m, numberOfNodes_m->value());
}

void MainWindow::showSolution()
{
    int nbSelect=graphWidget_m->getNbNodesCover();
    for(Node* pNode_l : pSolution_m->getSolution())
	{
        if(pNode_l->getColor()==Qt::red)
            nbSelect--;
        //TODO do the same for edges
        pNode_l->setColor(Qt::green);
        pNode_l->setDarkColor(Qt::darkGreen);
        pNode_l->update();
	}
    graphWidget_m->setNbNodesSelected(nbSelect);
}

void MainWindow::updateCover()
{
    nodesCover_m->setText(QString::number(graphWidget_m->getNbNodesCover()));
    edgesCover_m->setText(QString::number(graphWidget_m->getNbEdgesCover()));
}


