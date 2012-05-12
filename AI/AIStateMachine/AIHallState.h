#ifndef AI_Hall_STATE_H
#define AI_Hall_STATE_H

#include "AIState.h"
#include "AIStateSystem.h"
#include "AIHall.h"

class AIHall;
struct Telegram;

class PeaceHall : public AIState<AIHall>
{
private:

    PeaceHall(){}

    //copy ctor and assignment should be private
    PeaceHall(const PeaceHall&);
    PeaceHall& operator=(const PeaceHall&);

public:

    //this is a singleton
    static PeaceHall* Instance();
  
    virtual bool Enter(AIHall* Hall);

    virtual bool Execute(AIHall* Hall);

    virtual bool Exit(AIHall* Hall);

    virtual bool OnMessage(AIHall* agent, const Telegram& msg);
};

class DecreaseHealthHall : public AIState<AIHall>
{
private:

    DecreaseHealthHall(){}

    //copy ctor and assignment should be private
    DecreaseHealthHall(const DecreaseHealthHall&);
    DecreaseHealthHall& operator=(const DecreaseHealthHall&);

public:

    //this is a singleton
    static DecreaseHealthHall* Instance();
  
    virtual bool Enter(AIHall* Hall);

    virtual bool Execute(AIHall* Hall);

    virtual bool Exit(AIHall* Hall);

    virtual bool OnMessage(AIHall* agent, const Telegram& msg);
};

class IncreaseHealthHall : public AIState<AIHall>
{
private:

    IncreaseHealthHall(){}

    //copy ctor and assignment should be private
    IncreaseHealthHall(const IncreaseHealthHall&);
    IncreaseHealthHall& operator=(const IncreaseHealthHall&);

public:

    //this is a singleton
    static IncreaseHealthHall* Instance();
  
    virtual bool Enter(AIHall* Hall);

    virtual bool Execute(AIHall* Hall);

    virtual bool Exit(AIHall* Hall);

    virtual bool OnMessage(AIHall* agent, const Telegram& msg);
};

class DieHall : public AIState<AIHall>
{
private:

    DieHall(){}

    //copy ctor and assignment should be private
    DieHall(const DieHall&);
    DieHall& operator=(const DieHall&);

public:

    //this is a singleton
    static DieHall* Instance();
  
    virtual bool Enter(AIHall* Ranged);

    virtual bool Execute(AIHall* Ranged);

    virtual bool Exit(AIHall* Ranged);

    virtual bool OnMessage(AIHall* agent, const Telegram& msg);

};

#endif