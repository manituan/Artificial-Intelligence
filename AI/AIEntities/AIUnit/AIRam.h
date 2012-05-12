#ifndef AIRAM_H_
#define AIRAM_H_

#include "AIUnit.h"
#include "AIRamState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIRam : public AIUnit
{
private:

      AIStateSystem<AIRam>*  m_pStateMachine;

protected:


public:

    AIRam();

    AIRam(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             RamSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIRam>*   GetFSM()const{return m_pStateMachine;}
};

#endif