#ifndef UNTITLED_SESSION_H
#define UNTITLED_SESSION_H

#endif //UNTITLED_SESSION_H
#ifndef SESSION_H_
#define SESSION_H_

#include "Graph.h"
#include <vector>
#include <string>
#include <queue>

class Agent;

enum TreeType{
    Cycle,
    MaxRank,
    Root
};

class Session{
public:
    Session(const std::string& path);
    virtual ~Session();
    Session(const  Session &other);
    const Session& operator=(const Session &other);
    Session(Session&& other);
    const Session& operator=(Session&& other);

    void simulate();
    void addAgent(const Agent& agent);

    void enqueueInfected(int Vnode);
    int dequeueInfected();
    TreeType getTreeType() const;

    std::vector<std::vector<int>> getEdges();
    std::queue<int> getInfected();
    Graph getGraph() const;
    int GetCountCycle()const;
    bool isInfected(int node);
    bool toContinue();
    void updateInfected(int infected);
    std::vector<std::vector<int>>* getPointerEdges();
    bool amInfected(int node);
    std::vector<int> GetInfected();
    void AddInfected(int node);
    bool hasAgent(int i);

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*>agents;
    std::queue<int> infected; //the infection queue for the contact tracers.
    int countCycle; //counter of the cycle of the simulation.
    std::vector<int> infections;//represents all the viruses in the graph.

};
#endif