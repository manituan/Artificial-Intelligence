#ifndef AICATAPULT_H_
#define AICATAPULT_H_

#include "AIUnit.h"
#include "AICatapultState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AICatapult : public AIUnit
{
private:
      AIStateSystem<AICatapult>*  m_pStateMachine;
protected:


public:

    AICatapult();

    AICatapult(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             CatapultSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AICatapult>*   GetFSM()const{return m_pStateMachine;}
};

#endif