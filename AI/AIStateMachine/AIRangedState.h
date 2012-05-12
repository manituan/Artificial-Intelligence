#ifndef AI_RANGED_STATE_H
#define AI_RANGED_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIRanged.h"

class AIRanged;
struct Telegram;

class SeekToFlagRanged : public AIState<AIRanged>
{
private:

    SeekToFlagRanged(){}

    //copy ctor and assignment should be private
    SeekToFlagRanged(const SeekToFlagRanged&);
    SeekToFlagRanged& operator=(const SeekToFlagRanged&);

public:

    //this is a singleton
    static SeekToFlagRanged* Instance();
  
    virtual bool Enter(AIRanged* Ranged);

    virtual bool Execute(AIRanged* Ranged);

    virtual bool Exit(AIRanged* Ranged);

    virtual bool OnMessage(AIRanged* agent, const Telegram& msg);
};

class ChargeRanged : public AIState<AIRanged>
{
private:

    ChargeRanged(){}

    //copy ctor and assignment should be private
    ChargeRanged(const ChargeRanged&);
    ChargeRanged& operator=(const ChargeRanged&);

public:

    //this is a singleton
    static ChargeRanged* Instance();
  
    virtual bool Enter(AIRanged* Ranged);

    virtual bool Execute(AIRanged* Ranged);

    virtual bool Exit(AIRanged* Ranged);

    virtual bool OnMessage(AIRanged* agent, const Telegram& msg);
};

class AttackRanged : public AIState<AIRanged>
{
private:

    AttackRanged(){}

    //copy ctor and assignment should be private
    AttackRanged(const AttackRanged&);
    AttackRanged& operator=(const AttackRanged&);

public:

    //this is a singleton
    static AttackRanged* Instance();
  
    virtual bool Enter(AIRanged* Ranged);

    virtual bool Execute(AIRanged* Ranged);

    virtual bool Exit(AIRanged* Ranged);

    virtual bool OnMessage(AIRanged* agent, const Telegram& msg);
};

class DieRanged : public AIState<AIRanged>
{
private:

    DieRanged(){}

    //copy ctor and assignment should be private
    DieRanged(const DieRanged&);
    DieRanged& operator=(const DieRanged&);

public:

    //this is a singleton
    static DieRanged* Instance();
  
    virtual bool Enter(AIRanged* Ranged);

    virtual bool Execute(AIRanged* Ranged);

    virtual bool Exit(AIRanged* Ranged);

    virtual bool OnMessage(AIRanged* agent, const Telegram& msg);

};

class WaitRanged : public AIState<AIRanged>
{
private:

    WaitRanged(){}

    //copy ctor and assignment should be private
    WaitRanged(const WaitRanged&);
    WaitRanged& operator=(const WaitRanged&);

public:

    //this is a singleton
    static WaitRanged* Instance();
  
    virtual bool Enter(AIRanged* Ranged);

    virtual bool Execute(AIRanged* Ranged);

    virtual bool Exit(AIRanged* Ranged);

    virtual bool OnMessage(AIRanged* agent, const Telegram& msg);

};

#endif