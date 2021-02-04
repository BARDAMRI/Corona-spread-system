#include "Session.h"
#include "../include/json.hpp"
#include "Agent.h"
#include "Graph.h"
#include "iostream"
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <iterator>

using json = nlohmann::json;
using namespace std;
Session::Session(const std::string &path):g(),treeType(),agents(),infected(),countCycle(1),infections(){

    //read json file using ifstream.
    std::ifstream file(path);
    json j ;
    file>>j;
    cout<<j<<endl;

    //build matrix from the json and send it to the graph g constructor.
    std::vector< std::vector<int>> graph = j["graph"];
    g = Graph(graph);

    //get tree typr from json and set the field treeType.
    std::string tree =j["tree"];
    if(tree=="R")
        treeType = Root;
    else if (tree == "M")
            treeType = MaxRank;
    else if(tree=="C")
            treeType = Cycle;

    //reset the infected queue.
    Session::infected = queue<int>();

    //create agents from json input, and insert to the agents vector.
    for(auto& elem:j["agents"]) {
        if (elem[0] == "C") {
            Agent *agent = new ContactTracer();
            agents.push_back(agent);
        }
        else {
            Agent *agent = new Virus(elem[1]);
            agents.push_back(agent);
            AddInfected(agent->nodeAgent());
            infected.push(agent->nodeAgent());
        }
    }

}

//destructor.
Session::~Session() {
    int size=agents.size();
    for(int i = 0 ; i < size ; i++){
        if(agents[i]) delete agents[i];
    }
    agents.clear();
}

// copy constructor.
Session::Session(const Session &other):g(other.g),treeType(other.treeType),agents(),infected(other.infected),countCycle(other.countCycle),infections(other.infections){
    agents = vector<Agent *>();
    int size=other.agents.size();
    for (int i = 0; i < size; i++) {
        agents.push_back(other.agents[i]->clone());
    }
}

//assignment operator.
const Session& Session::operator=(const Session &other){
    if(this != &other) {
        g = other.g;
        infected = other.infected;
        treeType = other.treeType;
        countCycle = other.countCycle;
        agents = vector<Agent *>();
        int size=other.agents.size();
        for (int i = 0; i < size; i++) {
            agents[i] = other.agents[i];
        }
    }
    return *this;
}

//move constructor.
Session::Session(Session &&other):g(),treeType(other.treeType),agents(),infected(),countCycle(other.countCycle),infections(){

    g=other.g;
    infected=other.infected;
    infections=other.infections;
    agents = other.agents;
    int size=agents.size();
    for(int i = 0 ; i < size ; i++){
        other.agents[i] = nullptr;
    }
}

// move assignment operator.
const Session & Session::operator=(Session &&other){

    //delete all agents on the heap and reset the pointers.
    int size= agents.size();
    for(int i = 0 ; i <size ; i++){
        if(agents[i]) delete agents[i];
    }

    //enter new values.
    g = other.g;
    infected = other.infected;
    treeType = other.treeType;
    countCycle = other.countCycle;
    agents = other.agents;

    //delete heap data from input.
    int temp= agents.size();
    for(int i = 0 ; i < temp ; i++){
        other.agents[i] = nullptr;
    }
    return *this;
}

//main loop of the program.
void Session::simulate() {
    while (toContinue() || countCycle == 1) { //the conditions for the loop continuance.
        vector<Agent *> currAgents = agents;
        int x=currAgents.size();

        //start all agents act.
        for(int i=0;i<x;i++){

            currAgents[i]->act(*this);
        }
        //at the end of iteration raise the counter.
        countCycle++;
    }

    //at the end of the simulation, create json file of the results.(the graph and the infected.)
    json j;
    std::vector<std::vector<int>> edges;
    for (int i = 0; i < g.getSize(); ++i) {
        vector<int> neighbors = g.GetEdges()[i];
        edges.push_back(neighbors);
    }
    vector<int> areInfected;
    for (Agent *agent : agents) {
        if (agent->getType() == 'V') areInfected.push_back(agent->nodeAgent());
    }
    j["infected"] = areInfected;
    ofstream i("./output.json");
    j["graph"] = edges;
    i << j;
}

//add new agent to the vector.
void Session::addAgent(const Agent &agent){
    agents.push_back(agent.clone());
}

//enter new node to the infected queue from agents list.
void Session::enqueueInfected(int Vnode) {
    infected.push(Vnode);
}

//return the first node from infected queue to the contact tracer.
int Session::dequeueInfected() {
    if(infected.empty()) return -1;
    int temp = infected.front();
    g.SetInodes(temp);
    infected.pop();
    return temp;
}


TreeType Session::getTreeType() const {
    return treeType;
}

std::queue<int> Session::getInfected() {
    return infected;
}

Graph Session::getGraph()const{
    return g;
}

//after infected node infects healthy one , this method will update it in the agents list and in the graph infected list.
void Session::updateInfected(int infected) {

        addAgent(Virus(infected));
        g.infectNode(infected);
}

//method to return the graph for updates after contact tracer turn.
std::vector<std::vector<int>> Session::getEdges() {
    return g.GetEdges();
}

int Session::GetCountCycle() const {
    return countCycle;
}

//pass the infected queue and look for a node in it.
bool Session::isInfected(int node) {
    bool isInfected = false;
    int size=infected.size();
    for(int i = 0 ; i < size; i++){
        int temp = infected.front();
        infected.pop();
        if(temp == node) isInfected = true;
        infected.push(temp);
    }
    return isInfected;
}

//the condition for the loop continuance.
bool Session::toContinue() {
    //if there is no one to infect we can stop the simulate.
    int size1=infections.size();
    int size2=g.getSize();
    if(size1 == size2) return false;

    //passes all of the agents list, for each virus which check if there is any neighbor he can infects.
    for(Agent* a : agents)
    {
        if(a->nodeAgent()!=-1) {
            vector<int> neighbors = g.edgesOf(a->nodeAgent());
            for (int nei: neighbors) {
                if (!amInfected(nei)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::vector<int>> * Session::getPointerEdges() {
    return g.getPointerEdges();
}

//returns if a node ever got infected.
bool Session::amInfected(int node) {

    for(int a: GetInfected() )
        if(a==node)
            return true;
    return false;
}

//returns all of the infections from the first simulation.
std::vector<int> Session::GetInfected() { return this->infections;}

//add new node to the general sick/infected list.(not influenced from contact tracer act.)
void Session::AddInfected(int node) {  this->infections.push_back(node); }

//search for node in the agents list.
bool Session::hasAgent(int i) {
    for(Agent* a: agents )
    {
        if(a->nodeAgent()==i)
            return true;
    }
    return false;
}
