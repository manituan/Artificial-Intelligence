#ifndef GRAPH_RANGED_
#define GRAPH_RANGED_

#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphRanged: public GraphUnit
{
public:

	GraphRanged(int senderID, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphRanged();

	bool HandleMessage(const Telegram& telegram);

    virtual void Update(double timeSinceLastFrame);
    
private:

};  

#endif GRAPH_RANGED_