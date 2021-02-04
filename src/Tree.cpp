#include "Tree.h"
#include "Session.h"
#include <iostream>
using namespace std;

//Tree class Destructor
Tree::~Tree()
{
    int x=GetChildren().size();
    for(int i = 0 ; i < x  ; i++){
        delete GetChildren().at(i);
    }
}
//Tree class Constructor
Tree::Tree(int rootLabel):node(rootLabel),children(),depth(0) { }

//create tree method create a new tree from the input node uses the bfs method in graph class.
Tree * Tree::createTree(const Session& session, int rootLabel) { return session.getGraph().BFS(session , rootLabel); }


//Tree class copy constructor
Tree::Tree(const Tree& other):node(other.node),children(),depth(other.depth)
{
    for(auto& child : other.children)
    {
        children.push_back(child->clone());
    }
    depth = other.depth;
}

//move constructor
Tree::Tree(Tree&& other):node(other.node),children(other.children),depth(other.depth){}

//return the node number of the root.
int Tree::GetNode()const{
    return node;
}

//add new child as const to a tree.
void Tree::addChild(const Tree &child) {
    children.push_back(child.clone());
}
//add new child to a tree.
void Tree::addChild(Tree *child) { children.push_back(child); }

//returns the depth of node in the tree.
int Tree::GetDepth() const{ return depth; }

//set new depth to a node.
void Tree::SetDepth(int newDepth) { depth = newDepth; }

//returns the children vector of ta node as const.
std::vector<Tree*> Tree::GetChildren() const { return children; }

//method used in max rank tree traceTree method.
vector<int> Tree::RetMax(Tree *tree,std::vector<int> vec) {return vector<int>();}



//CycleTree class
CycleTree::CycleTree(int rootLabel, int currCycle): Tree(rootLabel),currCycle(currCycle){}

//trace tree method of cycle tree.
int CycleTree::traceTree() {

    //moves always to the (currCycle) left son while possible and
    CycleTree *output=this;
    for (int i = 0; i < currCycle-1; ++i) {
        if (output->GetChildren().size()>0) {
            CycleTree *temp = (CycleTree *) (((output->GetChildren()))[0]);
            output = temp;
        }
    }
    return output->GetNode();
}

//clone method for cycle tree for constructor and operator use.
Tree * CycleTree::clone() const{
    CycleTree* tree = new CycleTree(this->GetNode() ,this->currCycle);
    for(auto& child: this->GetChildren())
    {
        tree->addChild(child->clone());
    }
    return tree;
}

//clean method for cycle tree destructor and move functions.
void CycleTree::clean() const {

    //delete all pointers and the data from the heap.
    for(auto& child: this->GetChildren())
    {
        delete child;
        child = nullptr;
    }
}

//MaxRank
MaxRankTree::MaxRankTree(int rootLabel):Tree((rootLabel)){}

//max rank tree clone method for constructors and operators use.
Tree * MaxRankTree::clone() const {
    MaxRankTree* tree = new MaxRankTree(this->GetNode());
    for(auto& child : this->GetChildren())
    {
        tree->addChild(child->clone());
    }
    return tree;
}

//trace tree method of max rank tree.
int MaxRankTree::traceTree() {
    int x=this->GetChildren().size();
    vector<int> vec={x, this->GetNode()};
    return RetMax(this,vec)[1];
}

//recursion function for trace tree use that returns the node with the max number of children.
vector<int> MaxRankTree::RetMax(Tree *tree,vector<int> vec) {

    //stop condition.
    if(tree->GetChildren().empty()) {
        return vec;
    }
    else{

        //moves on all children and compare their max children number using recursion.
            for(Tree* tree1 : tree->GetChildren()) {
                int size=tree1->GetChildren().size();
                if(size > vec[0]) {
                    vec[0] = tree1->GetChildren().size();
                    vec[1]=tree1->GetNode();
                }
                vector<int>temp = RetMax(tree1,vec);
                if(temp[0]>vec[0]) {
                    vec[0] = temp[0];
                    vec[1]=temp[1];
                }
            }
        return vec;
    }
}

//clean method for max rank tree destructor and move operator.
void MaxRankTree::clean() const{
    int size=GetChildren().size();
    for(int i = 0 ; i < size  ; i++){
        delete GetChildren().at(i);
    }
}

//RootTree class
RootTree::RootTree(int rootLabel):Tree(rootLabel){}

//thee root tree method of trace tree returns the input node.()which is th root of the tree)
int RootTree::traceTree() { return this->GetNode(); }

//root tree clone method for constructors and operators use.
Tree * RootTree::clone() const{
    RootTree* tree = new RootTree(this->GetNode());
    //clone each child and its data.
    for(auto& child: this->GetChildren())
    {
        tree->addChild(child->clone());
    }
    return tree;
}

//clean method for root tree destructor and move operator.
void RootTree::clean() const {

    //delete all pointers and their data on the heap.
    for(auto& child: this->GetChildren())
    {
        delete child;
        child=nullptr;
    }
}