#ifndef AIHALL_H_
#define AIHALL_H_

#include "AIGame.h"
#include "AIHallState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIHall : public AIGame
{
private:

    AIStateSystem<AIHall>*  m_pStateMachine;
        int m_iAnimation;
protected:

public:

    AIHall();

    AIHall(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

    ~AIHall(){}
    
    int getAnimation(){return m_iAnimation;}
	void Update         (double time_elapsed);

    void                decreaseHealth();
    void                increaseHealth();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIHall>*   GetFSM()const{return m_pStateMachine;}

};

#endif