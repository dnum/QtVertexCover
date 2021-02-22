#ifndef SOLUTION_H
#define SOLUTION_H

#include "node.h"
#include "graphewidget.h"

class Solution
{
public:
    Solution(GraphWidget& graph_p);
    bool solve(GraphWidget&, int k_p);
    int easySolve(GraphWidget&, int k_p);
    QVector<Node*>getSolution();
private:
   QVector<Node*>solutionNode_m;

};

inline QVector<Node*>Solution::getSolution(){return solutionNode_m;}


#endif // SOLUTION_H
