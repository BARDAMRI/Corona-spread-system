#include "Agent.h"
#include "Session.h"
#include "Tree.h"
#include <vector>
#include <iostream>

using namespace std;

// Agents class
Agent::Agent(){}

// contactTracer class
ContactTracer::ContactTracer(){}

void ContactTracer::act(Session &session) {

    vector<vector<int>>* edges = session.getPointerEdges();
    int root = session.dequeueInfected(); //gets the first node from the infected queue to be the root of the tree.
    if(root != -1){

        //call create tree function which returns tree to use traceTree function.
        Tree *t = Tree::createTree(session , root);
        int toRemove = t->traceTree();

        //remove all the edges of the node that selected.
        int size=edges->size();
        for(int i = 0 ; i < size ; i++){
            (*edges)[toRemove][i] = 0;
        }
        for(int i = 0 ; i < size ; i++) {
            (*edges)[i][toRemove] = 0;
        }

        //delete the tree that is no longer in use.
        delete t;
    }
}

Agent * ContactTracer::clone() const {
    return new ContactTracer(*this);
}

int ContactTracer::nodeAgent() const {
    return -1; //all of the contact tracer are marked as -1 to be separated from the nodes index.
}

char ContactTracer::getType() const {
    return 'C';
}

// Virus class
Virus::Virus(int nodeInd):nodeInd(nodeInd){}

void Virus::act(Session &session) {

    //add new infected to the infected queue for next turn of the contact tracer.
    if(!session.amInfected(nodeInd)){
        session.enqueueInfected(nodeInd);
        session.AddInfected(nodeInd);
    }
    vector<vector<int>> edges = session.getEdges();

    //search for neighbor of the virus that is not infected yet and infects him.
    int size=edges[nodeInd].size();
    for (int i = 0; i < size ; i++) {
        if(edges[nodeInd][i] == 1 && !session.hasAgent(i)) {
            session.updateInfected(i);
            break;
        }
    }
}

Agent * Virus::clone() const {
    return new Virus(*this);
}

int Virus::nodeAgent() const {
    return nodeInd;
}

char Virus::getType() const {
    return 'V';
}