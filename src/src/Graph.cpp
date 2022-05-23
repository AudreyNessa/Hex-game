#include "Graph.h"

using namespace std;

struct Queue {short node; Queue* prev; Queue* next;};
const short int LEASTNODES = 50;

Graph::Graph(short int nodes)
{
    if (nodes >= LEASTNODES)
    {
        totalNodes = nodes;
        initialiseVariables(nodes);
    }
    else
    {
        totalNodes = 0;
    }

}

Graph::Graph(short noNodes, double density)
{
    //set up the random generator
    uniform_real_distribution<double> u(0,1);
    uniform_int_distribution<unsigned> c(1,10);
    default_random_engine e(time(0));

    const short highestProb = 1;
    totalNodes = noNodes;

    //ensure the arguments entered are correct
    if (noNodes >= LEASTNODES && density <= highestProb)
    {
        initialiseVariables(noNodes);
        for (short i = 0; i < noNodes; ++i)
        {
            for (short j = i; j < noNodes; ++j)
            {
                //create an undirected graph with the density having no self-loops
                if (i != j && (u(e) < density))
                {
                    matrix[i][j] = true;
                    matrix[j][i] = true;
                    cost[i][j] = c(e);
                    cost[j][i] = cost[i][j];
                }
            }
        }
    }
    else
    {
        totalNodes = 0;
    }

}

short Graph::getEdges() const
{
    short noEdges = 0; //counts the number of edges
    for (int i = 0; i < totalNodes; ++i)
    {
        for (int j = 0; j < totalNodes; ++j)
        {
            if (matrix[i][j])
                ++noEdges;
        }
    }
    return noEdges;
}

//list all nodes that have an edge with node x
vector<short> Graph::neighbours (short x)
{
    vector<short> xEdges;
    for (short i = 0; i < totalNodes; ++i)
    {
        if (matrix[x][i])
            xEdges.push_back(i);
    }
    return xEdges;
}

//adds an edge from node1 to node2 if it's not there
inline void Graph::linkNodes(short node1, short node2)
{
    switch (matrix[node1][node2])
    {
        case false:
            //makes an undirected graph
            matrix[node1][node2] = true;
            matrix[node2][node1] = true;
    }

}

//removes the edge from node1 to node2 if it's there
inline void unlinkNodes(short node1, short node2)
{
    switch (matrix[node1][node2])
    {
        case true:
            matrix[node1][node2] = false;
            matrix[node2][node1] = false;
    }
}


//initialises all the pointer variables in private
void initialiseVariables(short int matrixSize)
{
    matrix.resize(matrixSize);
    cost.resize(matrixSize);
    nodeValue.resize(matrixSize, 0);
    for (int i = 0; i < matrixSize; ++i)
    {
        matrix[i].resize(matrixSize, false);
        cost[i].resize(matrixSize, false);
    }
}

Graph::~Graph()
{
    //dtor
}
