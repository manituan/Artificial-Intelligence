#ifndef AIMONSTER_H_
#define AIMONSTER_H_

#include "AIUnit.h"
#include "AIMonsterState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIMonster : public AIUnit
{
private:

    AIStateSystem<AIMonster>*  m_pStateMachine;

protected:

public:

    AIMonster();

    AIMonster(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             MonsterSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIMonster>*   GetFSM()const{return m_pStateMachine;}

};

#endif