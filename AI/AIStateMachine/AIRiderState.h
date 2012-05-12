#ifndef AI_RIDER_STATE_H
#define AI_RIDER_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIRider.h"

class AIRider;
struct Telegram;

class SeekToFlagRider : public AIState<AIRider>
{
private:

    SeekToFlagRider(){}

    //copy ctor and assignment should be private
    SeekToFlagRider(const SeekToFlagRider&);
    SeekToFlagRider& operator=(const SeekToFlagRider&);

public:

    //this is a singleton
    static SeekToFlagRider* Instance();
  
    virtual bool Enter(AIRider* Rider);

    virtual bool Execute(AIRider* Rider);

    virtual bool Exit(AIRider* Rider);

    virtual bool OnMessage(AIRider* agent, const Telegram& msg);
};

class ChargeRider : public AIState<AIRider>
{
private:

    ChargeRider(){}

    //copy ctor and assignment should be private
    ChargeRider(const ChargeRider&);
    ChargeRider& operator=(const ChargeRider&);

public:

    //this is a singleton
    static ChargeRider* Instance();
  
    virtual bool Enter(AIRider* Rider);

    virtual bool Execute(AIRider* Rider);

    virtual bool Exit(AIRider* Rider);

    virtual bool OnMessage(AIRider* agent, const Telegram& msg);
};

class AttackRider : public AIState<AIRider>
{
private:

    AttackRider(){}

    //copy ctor and assignment should be private
    AttackRider(const AttackRider&);
    AttackRider& operator=(const AttackRider&);

public:

    //this is a singleton
    static AttackRider* Instance();
  
    virtual bool Enter(AIRider* Rider);

    virtual bool Execute(AIRider* Rider);

    virtual bool Exit(AIRider* Rider);

    virtual bool OnMessage(AIRider* agent, const Telegram& msg);
};


class DieRider : public AIState<AIRider>
{
private:

    DieRider(){}

    //copy ctor and assignment should be private
    DieRider(const DieRider&);
    DieRider& operator=(const DieRider&);

public:

    //this is a singleton
    static DieRider* Instance();
  
    virtual bool Enter(AIRider* Ranged);

    virtual bool Execute(AIRider* Ranged);

    virtual bool Exit(AIRider* Ranged);

    virtual bool OnMessage(AIRider* agent, const Telegram& msg);

};

class WaitRider : public AIState<AIRider>
{
private:

    WaitRider(){}

    //copy ctor and assignment should be private
    WaitRider(const WaitRider&);
    WaitRider& operator=(const WaitRider&);

public:

    //this is a singleton
    static WaitRider* Instance();
  
    virtual bool Enter(AIRider* Ranged);

    virtual bool Execute(AIRider* Ranged);

    virtual bool Exit(AIRider* Ranged);

    virtual bool OnMessage(AIRider* agent, const Telegram& msg);

};

#endif