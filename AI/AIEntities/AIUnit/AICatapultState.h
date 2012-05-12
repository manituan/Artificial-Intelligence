#ifndef AI_CATAPULT_STATE_H
#define AI_CATAPULT_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AICatapult.h"

class AICatapult;
struct Telegram;

class SeekToFlagCatapult : public AIState<AICatapult>
{
private:

    SeekToFlagCatapult(){}

    //copy ctor and assignment should be private
    SeekToFlagCatapult(const SeekToFlagCatapult&);
    SeekToFlagCatapult& operator=(const SeekToFlagCatapult&);

public:

    //this is a singleton
    static SeekToFlagCatapult* Instance();
  
    virtual bool Enter(AICatapult* Catapult);

    virtual bool Execute(AICatapult* Catapult);

    virtual bool Exit(AICatapult* Catapult);

    virtual bool OnMessage(AICatapult* agent, const Telegram& msg);
};

class ChargeCatapult : public AIState<AICatapult>
{
private:

    ChargeCatapult(){}

    //copy ctor and assignment should be private
    ChargeCatapult(const ChargeCatapult&);
    ChargeCatapult& operator=(const ChargeCatapult&);

public:

    //this is a singleton
    static ChargeCatapult* Instance();
  
    virtual bool Enter(AICatapult* Catapult);

    virtual bool Execute(AICatapult* Catapult);

    virtual bool Exit(AICatapult* Catapult);

    virtual bool OnMessage(AICatapult* agent, const Telegram& msg);
};

class AttackCatapult : public AIState<AICatapult>
{
private:

    AttackCatapult(){}

    //copy ctor and assignment should be private
    AttackCatapult(const AttackCatapult&);
    AttackCatapult& operator=(const AttackCatapult&);

public:

    //this is a singleton
    static AttackCatapult* Instance();
  
    virtual bool Enter(AICatapult* Catapult);

    virtual bool Execute(AICatapult* Catapult);

    virtual bool Exit(AICatapult* Catapult);

    virtual bool OnMessage(AICatapult* agent, const Telegram& msg);
};


class DieCatapult : public AIState<AICatapult>
{
private:

    DieCatapult(){}

    //copy ctor and assignment should be private
    DieCatapult(const DieCatapult&);
    DieCatapult& operator=(const DieCatapult&);

public:

    //this is a singleton
    static DieCatapult* Instance();
  
    virtual bool Enter(AICatapult* Ranged);

    virtual bool Execute(AICatapult* Ranged);

    virtual bool Exit(AICatapult* Ranged);

    virtual bool OnMessage(AICatapult* agent, const Telegram& msg);

};

class WaitCatapult : public AIState<AICatapult>
{
private:

    WaitCatapult(){}

    //copy ctor and assignment should be private
    WaitCatapult(const WaitCatapult&);
    WaitCatapult& operator=(const WaitCatapult&);

public:

    //this is a singleton
    static WaitCatapult* Instance();
  
    virtual bool Enter(AICatapult* Ranged);

    virtual bool Execute(AICatapult* Ranged);

    virtual bool Exit(AICatapult* Ranged);

    virtual bool OnMessage(AICatapult* agent, const Telegram& msg);

};

#endif