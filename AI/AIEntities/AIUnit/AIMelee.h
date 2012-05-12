#ifndef AIMELEE_H_
#define AIMELEE_H_

#include "AIUnit.h"
#include "AIMeleeState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"


struct Telegram;

class AIMelee : public AIUnit
{
private:

    AIStateSystem<AIMelee>*  m_pStateMachine;

protected:

public:

    AIMelee();

    AIMelee(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             MeleeSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIMelee>*   GetFSM()const{return m_pStateMachine;}

};

#endif