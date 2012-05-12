#ifndef GRAPH_ELITE_
#define GRAPH_ELITE_

#include "Ogre.h"
#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphElite: public GraphUnit
{
public:

	GraphElite(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphElite();

    virtual bool  HandleMessage(const Telegram& msg);

    virtual void Update(double timeSinceLastFrame);
    
private:



};  

#endif GRAPH_ELITE_