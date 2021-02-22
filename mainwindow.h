#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include "graphewidget.h"
#include "solution.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QDoubleSpinBox;
QT_END_NAMESPACE

class PenTool;
class BrushTool;
class CustomSlice;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

public Q_SLOTS:
    void createNewGraph();
    void showSolution();
    void updateCover();
    QLineEdit* getNumberOfNodesCover();
    QLineEdit* getNumberOfEdgesCover();
    Solution* getpSolution();

private:
    QDoubleSpinBox *numberOfNodes_m;
    QDoubleSpinBox *numberOfEdges_m;
    QLineEdit* edgesCover_m;
    QLineEdit* nodesCover_m;
    GraphWidget* graphWidget_m;
    Solution* pSolution_m;
};

inline QLineEdit* MainWindow::getNumberOfNodesCover(){ return nodesCover_m;}
inline QLineEdit* MainWindow::getNumberOfEdgesCover(){ return edgesCover_m;}
inline Solution* MainWindow::getpSolution(){ return pSolution_m;}

#endif // MAINWINDOW_H
