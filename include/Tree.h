#ifndef TREE_H_
#define TREE_H_

#include <vector>
class Session;

class Tree{
public:
    Tree(int rootLabel);
    virtual ~Tree();
    Tree(Tree&& other);
    Tree(const Tree& other);
    void addChild(const Tree& child);
    void addChild(Tree* child);
    virtual Tree* clone()const=0;
    virtual void clean()const =0;
    Tree& operator=(const Tree& other);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
    int GetNode() const;
    std::vector<Tree*> GetChildren() const;
    int GetDepth() const;
    void SetDepth(int newDepth);
    virtual std::vector<int> RetMax(Tree* tree,std::vector<int> vec);
    void print();
private:
    int node;
    std::vector<Tree*> children; //hold the children of the current node.
    int depth; //the depth of the node in the tree.
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual Tree* clone() const;
    virtual void clean() const;
private:
    int currCycle; // current cuycle of the tree which is the cycle of the simulation/
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    virtual Tree* clone() const;
    virtual void clean() const;
    virtual std::vector<int> RetMax(Tree* ,std::vector<int> vec );
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
    virtual Tree* clone() const;
    virtual void clean() const;
};

#endif