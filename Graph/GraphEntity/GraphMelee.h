#ifndef GRAPH_MELEE_
#define GRAPH_MELEE_

#include "Ogre.h"
#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphMelee: public GraphUnit
{
public:

	GraphMelee(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphMelee();

    virtual bool  HandleMessage(const Telegram& msg);

    virtual void Update(double timeSinceLastFrame);
    
private:



};  

#endif GRAPH_MELEE_