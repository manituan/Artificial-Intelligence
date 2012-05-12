#ifndef GRAPH_MONSTER_
#define GRAPH_MONSTER_

#include "Ogre.h"
#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphMonster: public GraphUnit
{
public:

	GraphMonster(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphMonster();

    virtual bool  HandleMessage(const Telegram& msg);

    virtual void Update(double timeSinceLastFrame);
    
private:

};  

#endif GRAPH_Monster_