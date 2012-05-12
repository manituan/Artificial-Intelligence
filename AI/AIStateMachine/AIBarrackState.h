#ifndef AI_Barrack_STATE_H
#define AI_Barrack_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIBarrack.h"

class AIBarrack;
struct Telegram;

class PeaceBarrack : public AIState<AIBarrack>
{
private:

    PeaceBarrack(){}

    //copy ctor and assignment should be private
    PeaceBarrack(const PeaceBarrack&);
    PeaceBarrack& operator=(const PeaceBarrack&);

public:

    //this is a singleton
    static PeaceBarrack* Instance();
  
    virtual bool Enter(AIBarrack* Barrack);

    virtual bool Execute(AIBarrack* Barrack);

    virtual bool Exit(AIBarrack* Barrack);

    virtual bool OnMessage(AIBarrack* agent, const Telegram& msg);
};

class DecreaseHealthBarrack : public AIState<AIBarrack>
{
private:

    DecreaseHealthBarrack(){}

    //copy ctor and assignment should be private
    DecreaseHealthBarrack(const DecreaseHealthBarrack&);
    DecreaseHealthBarrack& operator=(const DecreaseHealthBarrack&);

public:

    //this is a singleton
    static DecreaseHealthBarrack* Instance();
  
    virtual bool Enter(AIBarrack* Barrack);

    virtual bool Execute(AIBarrack* Barrack);

    virtual bool Exit(AIBarrack* Barrack);

    virtual bool OnMessage(AIBarrack* agent, const Telegram& msg);
};

class IncreaseHealthBarrack : public AIState<AIBarrack>
{
private:

    IncreaseHealthBarrack(){}

    //copy ctor and assignment should be private
    IncreaseHealthBarrack(const IncreaseHealthBarrack&);
    IncreaseHealthBarrack& operator=(const IncreaseHealthBarrack&);

public:

    //this is a singleton
    static IncreaseHealthBarrack* Instance();
  
    virtual bool Enter(AIBarrack* Barrack);

    virtual bool Execute(AIBarrack* Barrack);

    virtual bool Exit(AIBarrack* Barrack);

    virtual bool OnMessage(AIBarrack* agent, const Telegram& msg);
};

class DieBarrack : public AIState<AIBarrack>
{
private:

    DieBarrack(){}

    //copy ctor and assignment should be private
    DieBarrack(const DieBarrack&);
    DieBarrack& operator=(const DieBarrack&);

public:

    //this is a singleton
    static DieBarrack* Instance();
  
    virtual bool Enter(AIBarrack* Ranged);

    virtual bool Execute(AIBarrack* Ranged);

    virtual bool Exit(AIBarrack* Ranged);

    virtual bool OnMessage(AIBarrack* agent, const Telegram& msg);

};

#endif