#ifndef GRAPH_H_
#define GRAPH_H_
#include "Tree.h"
#include <vector>

class Graph{
public:
    Graph();
    Graph(std::vector<std::vector<int>> matrix);

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    Tree* BFS(const Session& session,int node);
    std::vector<std::vector<int>> GetEdges();
    std::vector<int> edgesOf(int node);
    Tree* getTree(const Session & session,int source);
    std::vector<std::vector<int>>* getPointerEdges();
    int getSize();
    void SetInodes(int node);


private:
    std::vector<std::vector<int>> edges;
    std::vector<bool> Inodes;  // will represent the infected nodes.
};
#endif