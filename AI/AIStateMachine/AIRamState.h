#ifndef AI_RAM_STATE_H
#define AI_RAM_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIRam.h"

class AIRam;
struct Telegram;

class SeekToFlagRam : public AIState<AIRam>
{
private:

    SeekToFlagRam(){}

    //copy ctor and assignment should be private
    SeekToFlagRam(const SeekToFlagRam&);
    SeekToFlagRam& operator=(const SeekToFlagRam&);

public:

    //this is a singleton
    static SeekToFlagRam* Instance();
  
    virtual bool Enter(AIRam* Ram);

    virtual bool Execute(AIRam* Ram);

    virtual bool Exit(AIRam* Ram);

    virtual bool OnMessage(AIRam* agent, const Telegram& msg);
};

class ChargeRam : public AIState<AIRam>
{
private:

    ChargeRam(){}

    //copy ctor and assignment should be private
    ChargeRam(const ChargeRam&);
    ChargeRam& operator=(const ChargeRam&);

public:

    //this is a singleton
    static ChargeRam* Instance();
  
    virtual bool Enter(AIRam* Ram);

    virtual bool Execute(AIRam* Ram);

    virtual bool Exit(AIRam* Ram);

    virtual bool OnMessage(AIRam* agent, const Telegram& msg);
};

class AttackRam : public AIState<AIRam>
{
private:

    AttackRam(){}

    //copy ctor and assignment should be private
    AttackRam(const AttackRam&);
    AttackRam& operator=(const AttackRam&);

public:

    //this is a singleton
    static AttackRam* Instance();
  
    virtual bool Enter(AIRam* Ram);

    virtual bool Execute(AIRam* Ram);

    virtual bool Exit(AIRam* Ram);

    virtual bool OnMessage(AIRam* agent, const Telegram& msg);
};


class DieRam : public AIState<AIRam>
{
private:

    DieRam(){}

    //copy ctor and assignment should be private
    DieRam(const DieRam&);
    DieRam& operator=(const DieRam&);

public:

    //this is a singleton
    static DieRam* Instance();
  
    virtual bool Enter(AIRam* Ranged);

    virtual bool Execute(AIRam* Ranged);

    virtual bool Exit(AIRam* Ranged);

    virtual bool OnMessage(AIRam* agent, const Telegram& msg);

};

class WaitRam : public AIState<AIRam>
{
private:

    WaitRam(){}

    //copy ctor and assignment should be private
    WaitRam(const WaitRam&);
    WaitRam& operator=(const WaitRam&);

public:

    //this is a singleton
    static WaitRam* Instance();
  
    virtual bool Enter(AIRam* Ranged);

    virtual bool Execute(AIRam* Ranged);

    virtual bool Exit(AIRam* Ranged);

    virtual bool OnMessage(AIRam* agent, const Telegram& msg);

};

#endif