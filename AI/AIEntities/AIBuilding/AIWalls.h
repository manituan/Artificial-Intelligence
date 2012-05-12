#ifndef AIWALLS_H_
#define AIWALLS_H_

#include "AIGame.h"
#include "AIWallsState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIWalls : public AIGame
{
private:

    AIStateSystem<AIWalls>*  m_pStateMachine;
        int m_iAnimation;
protected:

public:

    AIWalls();

    AIWalls(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

    ~AIWalls(){}
    
    int getAnimation(){return m_iAnimation;}
	void Update         (double time_elapsed);

    void                decreaseHealth();
    void                increaseHealth();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIWalls>*   GetFSM()const{return m_pStateMachine;}

};

#endif