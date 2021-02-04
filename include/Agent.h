#ifndef AGENT_H_
#define AGENT_H_
#include "Session.h"

#include <vector>

class Agent{
public:
    Agent();
    virtual ~Agent(){};
    virtual void act(Session& session)=0;
    virtual Agent* clone() const =0;
    virtual int nodeAgent() const = 0;
    virtual char getType() const = 0;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual int nodeAgent() const;
    virtual char getType() const;
};


class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual int nodeAgent() const;
    virtual char getType() const;

private:
    const int nodeInd;
};

#endif