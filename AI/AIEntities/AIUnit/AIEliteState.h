#ifndef AI_ELITE_STATE_H
#define AI_ELITE_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIElite.h"

class AIElite;
struct Telegram;

class SeekToFlagElite : public AIState<AIElite>
{
private:

    SeekToFlagElite(){}

    //copy ctor and assignment should be private
    SeekToFlagElite(const SeekToFlagElite&);
    SeekToFlagElite& operator=(const SeekToFlagElite&);

public:

    //this is a singleton
    static SeekToFlagElite* Instance();
  
    virtual bool Enter(AIElite* Elite);

    virtual bool Execute(AIElite* Elite);

    virtual bool Exit(AIElite* Elite);

    virtual bool OnMessage(AIElite* agent, const Telegram& msg);
};

class ChargeElite : public AIState<AIElite>
{
private:

    ChargeElite(){}

    //copy ctor and assignment should be private
    ChargeElite(const ChargeElite&);
    ChargeElite& operator=(const ChargeElite&);

public:

    //this is a singleton
    static ChargeElite* Instance();
  
    virtual bool Enter(AIElite* Elite);

    virtual bool Execute(AIElite* Elite);

    virtual bool Exit(AIElite* Elite);

    virtual bool OnMessage(AIElite* agent, const Telegram& msg);
};

class AttackElite : public AIState<AIElite>
{
private:

    AttackElite(){}

    //copy ctor and assignment should be private
    AttackElite(const AttackElite&);
    AttackElite& operator=(const AttackElite&);

public:

    //this is a singleton
    static AttackElite* Instance();
  
    virtual bool Enter(AIElite* Elite);

    virtual bool Execute(AIElite* Elite);

    virtual bool Exit(AIElite* Elite);

    virtual bool OnMessage(AIElite* agent, const Telegram& msg);
};


class DieElite : public AIState<AIElite>
{
private:

    DieElite(){}

    //copy ctor and assignment should be private
    DieElite(const DieElite&);
    DieElite& operator=(const DieElite&);

public:

    //this is a singleton
    static DieElite* Instance();
  
    virtual bool Enter(AIElite* Ranged);

    virtual bool Execute(AIElite* Ranged);

    virtual bool Exit(AIElite* Ranged);

    virtual bool OnMessage(AIElite* agent, const Telegram& msg);

};

class WaitElite : public AIState<AIElite>
{
private:

    WaitElite(){}

    //copy ctor and assignment should be private
    WaitElite(const WaitElite&);
    WaitElite& operator=(const WaitElite&);

public:

    //this is a singleton
    static WaitElite* Instance();
  
    virtual bool Enter(AIElite* Ranged);

    virtual bool Execute(AIElite* Ranged);

    virtual bool Exit(AIElite* Ranged);

    virtual bool OnMessage(AIElite* agent, const Telegram& msg);

};

#endif