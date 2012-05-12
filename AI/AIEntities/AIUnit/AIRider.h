#ifndef AIRIDER_H_
#define AIRIDER_H_

#include "AIUnit.h"
#include "AIRiderState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIRider : public AIUnit
{
private:

    AIStateSystem<AIRider>*  m_pStateMachine;

protected:

public:

    AIRider();

    AIRider(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

    ~AIRider()
    {

    }

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             RiderSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIRider>*   GetFSM()const{return m_pStateMachine;}

};

#endif