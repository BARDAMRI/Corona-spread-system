#include "Graph.h"
#include "Tree.h"
#include "Session.h"
#include <vector>
#include <queue>
#include "iostream"
#include <fstream>

using namespace std;
//Graph class
Graph::Graph():edges(),Inodes(){}
Graph::Graph(std::vector<std::vector<int>> matrix):edges(),Inodes() {

    //push the values of the input matrix to the matrix field.
    int size=matrix.size();
    for(int i = 0 ; i < size ; i++){
        vector <int> toAdd;
        int temp=matrix[i].size();
        for(int j = 0 ; j < temp ; j++){
            toAdd.push_back(matrix[i][j]);
        }
        edges.push_back(toAdd);
        Inodes.push_back(false); //set the default value of the infected vector.
    }
}

//mark a node as infected in the graph.
void Graph::infectNode(int nodeInd){
    Inodes[nodeInd] = true;
}

//return of node in the graph is infected.
bool Graph::isInfected(int nodeInd){
    return Inodes[nodeInd];
}

//BFS scan for the graph to build a tree for the contact tracer.
Tree * Graph::BFS(const Session &session, int node){

    vector<int> neighbors = edgesOf(node); //start with the neighbors of the root we got as input.
    if(neighbors.empty()) return getTree(session, node); //for node with no neighbors there is no tree but itself alone as root.

    //creat tree from the type we received from the input.
    Tree* tree = getTree(session, node);
    queue<Tree*> nodes = queue<Tree*>();
    vector<bool> IN ; //boolean table for the scan to know on which node we have already passed.

    //reset the boolean table to default.
    int size=edges.size();
    for(int j = 0 ; j < size ; j++){
        IN.push_back( false);
    }

    //pass on the nodes queue and for each node, pass on all its neighbors, build them as trees, put them as it's children and push them to the queue.
    nodes.push(tree);
    IN[tree->GetNode()]=true;
    Tree * temp;
    while(!nodes.empty()) {
        temp=nodes.front();
        nodes.pop();
        neighbors = edgesOf(temp->GetNode());
        for (int neighbor : neighbors) {
            Tree *neighborTree;
            if (!IN[neighbor]) {
                neighborTree = getTree(session, neighbor);
                neighborTree->SetDepth(tree->GetDepth() + 1);
                nodes.push(neighborTree);
                IN[neighborTree->GetNode()]=true;
                temp->addChild(neighborTree);
            }
        }
    }
    return tree;
}

std::vector<std::vector<int>> Graph::GetEdges() {
    return edges;
}

//method which returns the nodes which are neighbors of the input node.
std::vector<int> Graph::edgesOf(int node) {
    vector<int> toReturn;
    int size=edges.size();
    for(int i = 0 ; i < size; i++){
        if(edges[node][i] == 1&&edges[i][node] == 1) {
            toReturn.push_back(i);
        }
    }
    return toReturn;
}

//method for the bfs which build new tree in the type of the input .
Tree * Graph::getTree(const Session &session, int source) {
    TreeType type = session.getTreeType();
    if(type == Cycle)
        return new CycleTree(source,session.GetCountCycle()) ;
    else if(type == MaxRank){
        return new MaxRankTree(source);
    }
    return new RootTree(source);
}

std::vector<std::vector<int>> * Graph::getPointerEdges() {
    return &edges;
}

//used to get the number of nodes/ number of the matrix of edges.
int Graph::getSize() {
    return edges.size();
}

//out node from infected queue in the graph.
void Graph::SetInodes(int node) {
    Inodes[node] = false;
}