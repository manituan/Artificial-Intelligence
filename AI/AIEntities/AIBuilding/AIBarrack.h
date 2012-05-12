#ifndef AIBARRACK_H_
#define AIBARRACK_H_

#include "AIGame.h"
#include "AIBarrackState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIBarrack : public AIGame
{
private:

    AIStateSystem<AIBarrack>*  m_pStateMachine;
        int m_iAnimation;
protected:

public:

    AIBarrack();

    AIBarrack(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

    ~AIBarrack(){}
    int getAnimation(){return m_iAnimation;}
	void Update         (double time_elapsed);

    void                decreaseHealth();
    void                increaseHealth();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIBarrack>*   GetFSM()const{return m_pStateMachine;}

};

#endif