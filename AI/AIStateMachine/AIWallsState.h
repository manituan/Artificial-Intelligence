#ifndef AI_WALLS_STATE_H
#define AI_WALLS_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIWalls.h"

class AIWalls;
struct Telegram;

class PeaceWalls : public AIState<AIWalls>
{
private:

    PeaceWalls(){}

    //copy ctor and assignment should be private
    PeaceWalls(const PeaceWalls&);
    PeaceWalls& operator=(const PeaceWalls&);

public:

    //this is a singleton
    static PeaceWalls* Instance();
  
    virtual bool Enter(AIWalls* Walls);

    virtual bool Execute(AIWalls* Walls);

    virtual bool Exit(AIWalls* Walls);

    virtual bool OnMessage(AIWalls* agent, const Telegram& msg);
};

class DecreaseHealthWalls : public AIState<AIWalls>
{
private:

    DecreaseHealthWalls(){}

    //copy ctor and assignment should be private
    DecreaseHealthWalls(const DecreaseHealthWalls&);
    DecreaseHealthWalls& operator=(const DecreaseHealthWalls&);

public:

    //this is a singleton
    static DecreaseHealthWalls* Instance();
  
    virtual bool Enter(AIWalls* Walls);

    virtual bool Execute(AIWalls* Walls);

    virtual bool Exit(AIWalls* Walls);

    virtual bool OnMessage(AIWalls* agent, const Telegram& msg);
};

class IncreaseHealthWalls : public AIState<AIWalls>
{
private:

    IncreaseHealthWalls(){}

    //copy ctor and assignment should be private
    IncreaseHealthWalls(const IncreaseHealthWalls&);
    IncreaseHealthWalls& operator=(const IncreaseHealthWalls&);

public:

    //this is a singleton
    static IncreaseHealthWalls* Instance();
  
    virtual bool Enter(AIWalls* Walls);

    virtual bool Execute(AIWalls* Walls);

    virtual bool Exit(AIWalls* Walls);

    virtual bool OnMessage(AIWalls* agent, const Telegram& msg);
};

class DieWalls : public AIState<AIWalls>
{
private:

    DieWalls(){}

    //copy ctor and assignment should be private
    DieWalls(const DieWalls&);
    DieWalls& operator=(const DieWalls&);

public:

    //this is a singleton
    static DieWalls* Instance();
  
    virtual bool Enter(AIWalls* Ranged);

    virtual bool Execute(AIWalls* Ranged);

    virtual bool Exit(AIWalls* Ranged);

    virtual bool OnMessage(AIWalls* agent, const Telegram& msg);

};

#endif