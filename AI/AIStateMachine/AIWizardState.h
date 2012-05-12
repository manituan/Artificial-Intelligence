#ifndef AI_WIZARD_STATE_H
#define AI_WIZARD_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIWizard.h"

class AIWizard;
struct Telegram;

class SeekToFlagWizard : public AIState<AIWizard>
{
private:

    SeekToFlagWizard(){}

    //copy ctor and assignment should be private
    SeekToFlagWizard(const SeekToFlagWizard&);
    SeekToFlagWizard& operator=(const SeekToFlagWizard&);

public:

    //this is a singleton
    static SeekToFlagWizard* Instance();
  
    virtual bool Enter(AIWizard* Wizard);

    virtual bool Execute(AIWizard* Wizard);

    virtual bool Exit(AIWizard* Wizard);

    virtual bool OnMessage(AIWizard* agent, const Telegram& msg);
};

class ChargeWizard : public AIState<AIWizard>
{
private:

    ChargeWizard(){}

    //copy ctor and assignment should be private
    ChargeWizard(const ChargeWizard&);
    ChargeWizard& operator=(const ChargeWizard&);

public:

    //this is a singleton
    static ChargeWizard* Instance();
  
    virtual bool Enter(AIWizard* Wizard);

    virtual bool Execute(AIWizard* Wizard);

    virtual bool Exit(AIWizard* Wizard);

    virtual bool OnMessage(AIWizard* agent, const Telegram& msg);
};

class AttackWizard : public AIState<AIWizard>
{
private:

    AttackWizard(){}

    //copy ctor and assignment should be private
    AttackWizard(const AttackWizard&);
    AttackWizard& operator=(const AttackWizard&);

public:

    //this is a singleton
    static AttackWizard* Instance();
  
    virtual bool Enter(AIWizard* Wizard);

    virtual bool Execute(AIWizard* Wizard);

    virtual bool Exit(AIWizard* Wizard);

    virtual bool OnMessage(AIWizard* agent, const Telegram& msg);
};


class DieWizard : public AIState<AIWizard>
{
private:

    DieWizard(){}

    //copy ctor and assignment should be private
    DieWizard(const DieWizard&);
    DieWizard& operator=(const DieWizard&);

public:

    //this is a singleton
    static DieWizard* Instance();
  
    virtual bool Enter(AIWizard* Ranged);

    virtual bool Execute(AIWizard* Ranged);

    virtual bool Exit(AIWizard* Ranged);

    virtual bool OnMessage(AIWizard* agent, const Telegram& msg);

};

class WaitWizard : public AIState<AIWizard>
{
private:

    WaitWizard(){}

    //copy ctor and assignment should be private
    WaitWizard(const WaitWizard&);
    WaitWizard& operator=(const WaitWizard&);

public:

    //this is a singleton
    static WaitWizard* Instance();
  
    virtual bool Enter(AIWizard* Ranged);

    virtual bool Execute(AIWizard* Ranged);

    virtual bool Exit(AIWizard* Ranged);

    virtual bool OnMessage(AIWizard* agent, const Telegram& msg);

};

#endif