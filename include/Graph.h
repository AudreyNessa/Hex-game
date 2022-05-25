#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <ctime>
#include <cassert>
#include <iostream>
#include <list>
#include <random>
#include <vector>

class Graph
{
    public:
        //constructors
        Graph(short int nodes);
        Graph(short noNodes, double density);

        //accessors
        inline short getTotalNodes() const{return totalNodes;} //returns the number of vertices
        inline bool** getMatrix() const{return matrix;} //returns the whole matrix

        //returns the number of edges in the graph
        short getEdges() const;

        //tests whether there's an edge from node1 to node2
        inline bool adjacent(short node1, short node2) const{return matrix[node1][node2];}

        //list all nodes that have and edge with a node x
        vector<short> neighbours (short x) const;

        //gets the value associated with the edge node1, node2
        inline short getEdgeValue(short node1, short node2) const{return cost[node1][node2];}

        //returns the value associated with the node x
        inline short getNodeValue(short node1) const {return nodeValue[node1];}


        //mutators

        //adds an edge from node1 to node2 if it's not there
        inline void linkNodes(short node1, short node2);

        //removes the edge from node1 to node2 if it's there
        inline void unlinkNodes(short node1, short node2);

        //initialises all the pointer variables in private
        void initialiseVariables();

        //sets the value associated to the edge(x,y) to v
        inline void setEdgeValue(short x, short y, short v){cost[x][y] = v; cost[y][x] = v;}

        //sets the value associated with the node
        inline void setNodeValue(short x, short a){nodeValue[x] = a;}

        //shortest path algorithm(returns the least cost to go from one node to another) using djikstra's algorithm
        shortestPath(short node1, short node);

        //Destructor
        virtual ~Graph();

        class PriorityQueue
        {
            public:
                //initialise elements
                PriorityQueue(short node, short priority);

                /* ################################## ACCESSOR METHODS #######################*/
                short top() const {return PQ.front()}
                short sizePQ() const {return PQ.size()}
                inline list<short> getQueue() const {return PQ;}

                /* ########################## MUTATOR METHODS #########################*/
                //changes the priority(node value) of queue element if the new priority is better than the previous
                //also adds to the queue a new element
                //returns true if the element has a new parent node
                //happens when the new priority for a node is less than the existing one in the queue
                bool chgPriority(short node, short priority);

                //removes the top element of the queue and returns the value
                short minPriority();

                //shortest path algorithm(returns the least cost to go from one node to another)
                friend djikstra (Graph& graph);

            protected:
                vector<bool> inPQ;
                list<short> PQ;

        };


    protected:
        short int totalNodes;
        vector<vector<bool>> matrix;
        vector<vector<bool>> cost;
        vector<short> nodeValue;

    private:
};



#endif // GRAPH_H
