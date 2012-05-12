#ifndef AI_MONSTER_STATE_H
#define AI_MONSTER_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIMonster.h"

class AIMonster;
struct Telegram;

class SeekToFlagMonster : public AIState<AIMonster>
{
private:

    SeekToFlagMonster(){}

    //copy ctor and assignment should be private
    SeekToFlagMonster(const SeekToFlagMonster&);
    SeekToFlagMonster& operator=(const SeekToFlagMonster&);

public:

    //this is a singleton
    static SeekToFlagMonster* Instance();
    
    virtual bool Enter(AIMonster* Monster);

    virtual bool Execute(AIMonster* Monster);

    virtual bool Exit(AIMonster* Monster);

    virtual bool OnMessage(AIMonster* agent, const Telegram& msg);
};

class ChargeMonster : public AIState<AIMonster>
{
private:

    ChargeMonster(){}

    //copy ctor and assignment should be private
    ChargeMonster(const ChargeMonster&);
    ChargeMonster& operator=(const ChargeMonster&);

public:

    //this is a singleton
    static ChargeMonster* Instance();
  
    virtual bool Enter(AIMonster* Monster);

    virtual bool Execute(AIMonster* Monster);

    virtual bool Exit(AIMonster* Monster);

    virtual bool OnMessage(AIMonster* agent, const Telegram& msg);
};

class AttackMonster : public AIState<AIMonster>
{
private:

    AttackMonster(){}

    //copy ctor and assignment should be private
    AttackMonster(const AttackMonster&);
    AttackMonster& operator=(const AttackMonster&);

public:

    //this is a singleton
    static AttackMonster* Instance();
  
    virtual bool Enter(AIMonster* Monster);

    virtual bool Execute(AIMonster* Monster);

    virtual bool Exit(AIMonster* Monster);

    virtual bool OnMessage(AIMonster* agent, const Telegram& msg);
};


class DieMonster : public AIState<AIMonster>
{
private:

    DieMonster(){}

    //copy ctor and assignment should be private
    DieMonster(const DieMonster&);
    DieMonster& operator=(const DieMonster&);

public:

    //this is a singleton
    static DieMonster* Instance();
  
    virtual bool Enter(AIMonster* Ranged);

    virtual bool Execute(AIMonster* Ranged);

    virtual bool Exit(AIMonster* Ranged);

    virtual bool OnMessage(AIMonster* agent, const Telegram& msg);

};

class WaitMonster : public AIState<AIMonster>
{
private:

    WaitMonster(){}

    //copy ctor and assignment should be private
    WaitMonster(const WaitMonster&);
    WaitMonster& operator=(const WaitMonster&);

public:

    //this is a singleton
    static WaitMonster* Instance();
  
    virtual bool Enter(AIMonster* Ranged);

    virtual bool Execute(AIMonster* Ranged);

    virtual bool Exit(AIMonster* Ranged);

    virtual bool OnMessage(AIMonster* agent, const Telegram& msg);

};

#endif