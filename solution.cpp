#include "solution.h"
#include "edge.h"
#include <iostream>

Solution::Solution(GraphWidget& graph_p)
{
    solutionNode_m = graph_p.getNodes();
}
int Solution::easySolve(GraphWidget& graph_p, int maxNodes_p)
{
    std::cout<<"Graph="<<graph_p.getIdGraph()<<" edge="<<graph_p.getEdges().size()<<" nodes="<<graph_p.getNodes().size()<<std::endl;
    if(maxNodes_p>=0 and graph_p.getEdges().size()==0 and graph_p.getRemoveNodes().size() < solutionNode_m.size())
	{
        solutionNode_m = graph_p.getRemoveNodes();
        std::cout<<" solution="<<graph_p.getNodes().size()<<std::endl;
        return 1;
    }
    
    if(graph_p.getRemoveNodes().size() > solutionNode_m.size() || graph_p.getEdges().size()==0)
    {
        std::cout<<" false nbNodes="<<graph_p.getNodes().size()<<std::endl;
        return 0;
    }

    Node* nodeMaxDeg_l=graph_p.nodeMaxDegree();
    std::cout<<"graphe2 we delete Node with max degree="<<nodeMaxDeg_l->getId()<<"  of degree "<<nodeMaxDeg_l->getNeighborhood().size()<<std::endl;
    GraphWidget graph2_l(graph_p);
    graph2_l.delNode(nodeMaxDeg_l);
    graph2_l.setIdGraph(graph2_l.getIdGraph()+"-"+std::to_string(nodeMaxDeg_l->getId()));

    GraphWidget graph3_l(graph_p);
    QVector<Node*> listvois =nodeMaxDeg_l->getNeighborhood();
	
    int numberOfNeighbour_l=listvois.size();
    std::cout<<" numberOfNeighbour_l="<<numberOfNeighbour_l<<std::endl;
    for(int id_l=0;id_l<numberOfNeighbour_l;id_l++)
     {
        std::cout<<"Graphe 3 we delete node "<<listvois[id_l]->getId()<<std::endl;
        graph3_l.delNode(listvois[id_l]);
        graph3_l.setIdGraph(graph3_l.getIdGraph()+"-"+std::to_string(listvois[id_l]->getId()));
     }
    return (easySolve(graph2_l,maxNodes_p-1) + easySolve(graph3_l,maxNodes_p-numberOfNeighbour_l));
}

bool Solution::solve(GraphWidget& graph_p, int maxNodes_p)
{
    //TODO Best Solution to implement (from an old project)
/*    if(maxNodes_p >=0 and graph_p.getEdges().size()==0)
        return true;

    if( maxNodes_p <0)
        return false;

    int numberOfNodes_l=graph_p.getNodes().size();

    for(int id_l=0;id_l<numberOfNodes_l;id_l++)            //si il exite un sommet de degrés 1 alors son voisin et dans le vertex cover
    {
        if(graph_p.getNodes()[id_l]->edges().size()==1)
        {
            Node* aloneNode_l=graph_p.getNodes()[id_l].getNeighborhood().size();
            GraphWidget graph2_l(graph_p,graph_p.voisins(x)[0]);
            return solve(graph2_l,maxNodes_p-1) ;
        }
    }

    for(int id_l=0;id_l<numberOfNodes_l;id_l++)       //on regarde les sommets de degrés supèrieur à 5
    {
        int degree_l=graph_p.getNodes()[id_l]->edges().size();
        if(degree_l>4)
        {
            GraphWidget g1(graph_p,id_l);
            GraphWidget g2(graph_p);
            g2.delVoisSommet(id_l);
            return (solve(g2,maxNodes_p-degree_l) or solve(g1,maxNodes_p-1)) ;
        }
    }

    int d=g.degSom(0);
    bool co=true;
    for(int i=1;i<numberOfNodes_l;i++)    //on regarde si le graphe est régulier
    {
        if(d!=g.degSom(i))
        {
            co=false;
        }
    }

    if(co)
    {
        int x=g.areteFirst()[0];
        int t=g.voisins(x).size();
        GraphWidget g1(graph_p,x);
        g.delVoisSommet(x);
        return (solve(g,maxNodes_p-t) or solve(g1,maxNodes_p-1)) ;
    }

    for(int i=0;i<numberOfNodes_l;i++)       //on regarde les sommets de degrés égale à 2
    {
        if(g.degSom(i)==2)
        {
            int a=g.voisins(i)[0];
            int b=g.voisins(i)[1];
            if(g.voisin(a,b))
            {
                GraphWidget g1(g,a);
                GraphWidget g2(g1,b);
                return solve(g2,maxNodes_p-2)  ;
            }
            else
            {
                int ta=g.voisins(a).size();
                bool k=false;
                int c;
                for(int j=0;j<ta;j++)
                {
                    if(g.voisin(b,g.voisins(a)[j]) and g.voisins(a)[j]!=i )
                    {
                        k=true;
                        c=g.voisins(a)[j];
                    }
                }
                if((a==b) and (a==2) and k)
                {
                    GraphWidget g1(g,c);
                    GraphWidget g2(g1,i);
                    return VC_N(g2,k-2);
                }
                else
                {
                    GraphWidget g2(g);
                    GraphWidget g3(g);
                    g3.delVoisSommet(i);
                    g2.delVoisSommet(a);
                    g2.delVoisSommet(b);
                    int l=g.nbSommet()-g2.nbSommet();
                    return (solve(g3,maxNodes_p-2) or solve(g2,maxNodes_p-l) ) ;
                }
            }
        }
    }

    for(int i=0;i<numberOfNodes_l;i++)       //on regarde les sommets de degrés égale à 3
    {
        if(g.degSom(i)==3)
        {
            int a=g.voisins(i)[0];
            int b=g.voisins(i)[1];
            int c=g.voisins(i)[2];
            int da=g.degSom(a);
            int db=g.degSom(b);
            int dc=g.degSom(c);
            //on cherche a savoir si i forme un triangle avec 2 de ses voisins
            if(g.voisin(a,b))
            {
                GraphWidget g2(g);
                g.delVoisSommet(i);
                g2.delVoisSommet(c);
                return (solve(g,maxNodes_p-3) or solve(g2,maxNodes_p-dc) ) ;
            }
            else if(g.voisin(a,c))
            {
                GraphWidget g2(g);
                g.delVoisSommet(i);
                g2.delVoisSommet(b);
                return (solve(g,maxNodes_p-3) or solve(g2,maxNodes_p-db) ) ;
            }
            else if(g.voisin(b,c))
            {
                GraphWidget g2(g);
                g.delVoisSommet(i);
                g2.delVoisSommet(a);
                return (solve(g,maxNodes_p-3) or solve(g2,maxNodes_p-da) ) ;
            }

            //recherche d'un cycle de longueur 4 contenant i , deux de ses voisins (a,b; a,c ou b,c) et un autre sommet n'étant pas voisin avec i (on le note e)
            int e;
            for(int la=0;la<da;la++)
            {
                for(int lb=0;lb<db;lb++)
                {
                    if (g.voisin(g.voisins(a)[da],g.voisins(b)[db]))
                    {
                        e=g.voisins(a)[da];
                        if(!g.voisin(e,i))
                        {
                            GraphWidget g2(g,i);
                            g.delVoisSommet(i);
                            GraphWidget g3(g2,e);
                            return (solve(g,maxNodes_p-3) or solve(g3,maxNodes_p-2) ) ;
                        }

                    }
                }
            }
            for(int la=0;la<da;la++)
            {
                for(int lc=0;lc<dc;lc++)
                {
                    if (g.voisin(g.voisins(a)[da],g.voisins(c)[dc]))
                    {
                        e=g.voisins(a)[da];
                        if(!g.voisin(e,i))
                        {
                            GraphWidget g2(g,i);
                            g.delVoisSommet(i);
                            GraphWidget g3(g2,e);
                            return (solve(g,maxNodes_p-3) or solve(g3,maxNodes_p-2) ) ;
                        }

                    }
                }
            }

            for(int lc=0;lc<dc;lc++)
            {
                for(int lb=0;lb<db;lb++)
                {
                    if (g.voisin(g.voisins(c)[dc],g.voisins(b)[db]))
                    {
                        e=g.voisins(c)[dc];
                        if(!g.voisin(e,i))
                        {
                            GraphWidget g2(g,i);
                            g.delVoisSommet(i);
                            GraphWidget g3(g2,e);
                            return (solve(g,maxNodes_p-3) or solve(g2,maxNodes_p-2) ) ;
                        }

                    }
                }
            }

            //si i est le seul voisin commun à a,b et c (ses 3 voisins ) et que l'un de ses sommet et de degré 4
            bool pasdecommun=true;
            bool d4=false;
            int ta,tb,tc;

            if(da==4)
            {
                d4=true;
                ta=a;
                tb=b;
                tc=c;
            }
            else
            {
                if(db==4)
                {
                    d4=true;
                    ta=b;
                    tb=a;
                    tc=c;
                }
                else
                {
                    if(dc==4)
                    {
                        d4=true;
                        ta=c;
                        tb=b;
                        tc=a;
                    }
                }
            }
            int dta=g.degSom(ta);
            int dtbc=g.degSom(tc)+g.degSom(tb);
            for(int la=0;la<da;la++)
            {
                for(int lb=0;lb<db;lb++)
                {
                    for(int lc=0;lc<dc;lc++)
                    {
                        if(((g.voisins(b)[lb]==g.voisins(a)[la] and g.voisins(b)[lb]==g.voisins(c)[lc])and g.voisins(a)[la]==g.voisins(c)[lc]) and 
(g.voisins(c)[lc]!=i))
                            pasdecommun=false;
                    }
                }
            }

            if(pasdecommun and d4)
            {
                GraphWidget g2(g,ta);
                GraphWidget g3(g);
                g.delVoisSommet(i);
                g2.delVoisSommet(tb);
                g2.delVoisSommet(tc);
                g3.delVoisSommet(ta);
                return ((solve(g,maxNodes_p-g.degSom(i)) or solve(g2,maxNodes_p-1-dtbc)) or solve(g3,maxNodes_p-dta) ) ;
            }

        }
    }*/
	return true;
}
