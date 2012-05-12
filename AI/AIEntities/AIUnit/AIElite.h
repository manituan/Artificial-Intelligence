#ifndef AIELITE_H_
#define AIELITE_H_

#include "AIUnit.h"
#include "AIEliteState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIElite : public AIUnit
{
private:

    AIStateSystem<AIElite>*  m_pStateMachine;

protected:

public:

    AIElite();

    AIElite(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             EliteSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIElite>*   GetFSM()const{return m_pStateMachine;}

};

#endif