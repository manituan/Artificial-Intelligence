#ifndef AIRANGED_H_
#define AIRANGED_H_

#include "AIUnit.h"
#include "AIRangedState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

class AIRanged : public AIUnit
{
private:

    AIStateSystem<AIRanged>*  m_pStateMachine;

protected:

public:

    AIRanged();

    AIRanged(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

    ~AIRanged()
    {

    }

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             rangedSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIRanged>*   GetFSM()const{return m_pStateMachine;}
};

#endif