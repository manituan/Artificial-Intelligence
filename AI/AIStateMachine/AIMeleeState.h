#ifndef AI_MELEE_STATE_H
#define AI_MELEE_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIMelee.h"

class AIMelee;
struct Telegram;

class SeekToFlagMelee : public AIState<AIMelee>
{
private:

    SeekToFlagMelee(){}

    //copy ctor and assignment should be private
    SeekToFlagMelee(const SeekToFlagMelee&);
    SeekToFlagMelee& operator=(const SeekToFlagMelee&);

public:

    //this is a singleton
    static SeekToFlagMelee* Instance();
  
    virtual bool Enter(AIMelee* melee);

    virtual bool Execute(AIMelee* melee);

    virtual bool Exit(AIMelee* melee);

    virtual bool OnMessage(AIMelee* agent, const Telegram& msg);
};

class ChargeMelee : public AIState<AIMelee>
{
private:

    ChargeMelee(){}

    //copy ctor and assignment should be private
    ChargeMelee(const ChargeMelee&);
    ChargeMelee& operator=(const ChargeMelee&);

public:

    //this is a singleton
    static ChargeMelee* Instance();
  
    virtual bool Enter(AIMelee* melee);

    virtual bool Execute(AIMelee* melee);

    virtual bool Exit(AIMelee* melee);

    virtual bool OnMessage(AIMelee* agent, const Telegram& msg);
};

class AttackMelee : public AIState<AIMelee>
{
private:

    AttackMelee(){}

    //copy ctor and assignment should be private
    AttackMelee(const AttackMelee&);
    AttackMelee& operator=(const AttackMelee&);

public:

    //this is a singleton
    static AttackMelee* Instance();
  
    virtual bool Enter(AIMelee* melee);

    virtual bool Execute(AIMelee* melee);

    virtual bool Exit(AIMelee* melee);

    virtual bool OnMessage(AIMelee* agent, const Telegram& msg);
};


class DieMelee : public AIState<AIMelee>
{
private:

    DieMelee(){}

    //copy ctor and assignment should be private
    DieMelee(const DieMelee&);
    DieMelee& operator=(const DieMelee&);

public:

    //this is a singleton
    static DieMelee* Instance();
  
    virtual bool Enter(AIMelee* Ranged);

    virtual bool Execute(AIMelee* Ranged);

    virtual bool Exit(AIMelee* Ranged);

    virtual bool OnMessage(AIMelee* agent, const Telegram& msg);

};

class WaitMelee : public AIState<AIMelee>
{
private:

    WaitMelee(){}

    //copy ctor and assignment should be private
    WaitMelee(const WaitMelee&);
    WaitMelee& operator=(const WaitMelee&);

public:

    //this is a singleton
    static WaitMelee* Instance();
  
    virtual bool Enter(AIMelee* Ranged);

    virtual bool Execute(AIMelee* Ranged);

    virtual bool Exit(AIMelee* Ranged);

    virtual bool OnMessage(AIMelee* agent, const Telegram& msg);

};

#endif