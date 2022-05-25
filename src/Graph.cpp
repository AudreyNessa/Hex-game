#include "Graph.h"

using namespace std;

const short int LEASTNODES = 50;

Graph::Graph(short int nodes)
{
    if (nodes >= LEASTNODES)
    {
        Graph::totalNodes = nodes;
        Graph::initialiseVariables();
    }
    else
    {
        Graph::totalNodes = 0;
    }

}

Graph::Graph(short noNodes, double density)
{
    //set up the random generator
    uniform_real_distribution<double> u(0,1);
    uniform_int_distribution<unsigned> c(1,10);
    default_random_engine e(time(0));

    const short highestProb = 1;
    Graph::totalNodes = noNodes;

    //ensure the arguments entered are correct
    if (noNodes >= LEASTNODES && density <= highestProb)
    {
        Graph::initialiseVariables();
        for (short i = 0; i < noNodes; ++i)
        {
            for (short j = i; j < noNodes; ++j)
            {
                //create an undirected graph with the density having no self-loops
                if (i != j && (u(e) < density))
                {
                    Graph::matrix[i][j] = true;
                    Graph::matrix[j][i] = true;
                    Graph::cost[i][j] = c(e);
                    Graph::cost[j][i] = cost[i][j];
                }
            }
        }
    }
    else
    {
        Graph::totalNodes = 0;
    }

}

short Graph::getEdges() const
{
    short noEdges = 0; //counts the number of edges
    for (int i = 0; i < Graph::totalNodes; ++i)
    {
        for (int j = 0; j < Graph::totalNodes; ++j)
        {
            if (Graph::matrix[i][j])
                ++noEdges;
        }
    }
    return noEdges;
}

//list all nodes that have an edge with node x
vector<short> Graph::neighbours (short x)
{
    vector<short> xEdges;
    for (short i = 0; i < Graph::totalNodes; ++i)
    {
        if (Graph::matrix[x][i])
            xEdges.push_back(i);
    }
    return xEdges;
}

//adds an edge from node1 to node2 if it's not there
inline void Graph::linkNodes(short node1, short node2)
{
    switch (Graph::matrix[node1][node2])
    {
        case false:
            //makes an undirected graph
            Graph::matrix[node1][node2] = true;
            Graph::matrix[node2][node1] = true;
    }

}

//removes the edge from node1 to node2 if it's there
inline void Graph::unlinkNodes(short node1, short node2)
{
    switch (Graph::matrix[node1][node2])
    {
        case true:
            Graph::matrix[node1][node2] = false;
            Graph::matrix[node2][node1] = false;
    }
}


//initialises all the pointer variables in private
void Graph::initialiseVariables()
{
    Graph::matrix.resize(Graph::totalNodes);
    Graph::cost.resize(Graph::totalNodes);
    Graph::nodeValue.resize(Graph::totalNodes, 0);
    for (int i = 0; i < Graph::totalNodes; ++i)
    {
        Graph::matrix[i].resize(Graph::totalNodes, false);
        Graph::cost[i].resize(Graph::totalNodes, false);
    }
}

Graph::PriorityQueue::PriorityQueue(short node, short priority)
{
            Graph::PriorityQueue::PQ.push_back(node);
            Graph::setNodeValue(node, priority);

            Graph::PriorityQueue::inPQ.resize(Graph::totalNodes, false);
            Graph::PriorityQueue::inPQ[node] = true;
}

Graph::PriorityQueue::chgPriority(short node, short priority)
{
    bool addedToPQ = false;
    bool parentChanged = false;

    //if the queue is empty create a new queue and add the element as the first element of the queue
    if (Graph::PriorityQueue::PQ.empty())
    {
        if (!Graph::PriorityQueue::inPQ[node])
        {
            Graph::PriorityQueue::PQ.push_back(node);
            Graph::setNodeValue(node, priority);
            Graph::PriorityQueue::inPQ[node] = true;
        }

    }

    /*if it's already in the priority queue and the new priority is less than previous,
    or it hasn't been added do the priority queue, set the priority and add to the
    queue in ascending order else end
    */
    else if ((Graph::PriorityQueue::inPQ[node] && Graph::getNodeValue(node) > priority) || !Graph::PriorityQueue::inPQ[node])
    {

        //set the new priority
        Graph::setNodeValue(node, priority);

        //add to the appropriate place in the priority queue in ascending order

        auto cursor = Graph::PriorityQueue::PQ.begin();

        //user cursor+1 to keep track of the of the priority queue.
        //If cursor is used instead, when it's a null pointer there is no way to get the previous element
        while ((cursor+1) != Graph::PriorityQueue::PQ.end())
        {
            //delete the node if was already in the priority queue
            if (*cursor == node)
            {
                parentChanged = true;
                //if the new priority has already been added, delete previous else leave as is
                if (addedToPQ)
                {
                    Graph::PriorityQueue::PQ.erase(cursor);
                    break;
                }
                else
                {
                    addedToPQ = true;
                    break;

                }

            }
            else if (!addedToPQ && graph.getNodeValue(*cursor) > priority)
            {
                Graph::PriorityQueue::PQ.insert(cursor, node);
                addedToPQ = true;
                Graph::PriorityQueue::inPQ[node] = true;

            }
            ++cursor;
        }
        //if the last node in the priority queue is the same as the node already added PQ, delete
        if (*cursor == node && !parentChanged)
        {
            parentChanged = true;
            if (addedToPQ)
            {
               Graph::PriorityQueue::PQ.erase(cursor);
            }

        }

        //if not added to the priority queue fix the node in the appropriate place
        //if the priority of the node is less than the last element in PQ add the node before it else add it after
        else if (!addedToPQ)
        {
            if (*cursor != node)
            {
                if (graph.getNodeValue(*cursor) > priority)
                {
                    Graph::PriorityQueue::PQ.insert(cursor, node);
                }
                else
                {
                    Graph::PriorityQueue::PQ.push_back(node);
                }
                Graph::PriorityQueue::inPQ[node] = true;
            }

        }


    }
    return parentChanged;
}

short Graph::PriorityQueue::minPriority()
{
    short firstNode = Graph::PriorityQueue::PQ.front();
    PQ.pop_front();
    return firstNode;
}

Graph::~Graph()
{
    //dtor
}
