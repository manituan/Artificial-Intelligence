#ifndef GRAPH_RIDER_
#define GRAPH_RIDER_

#include "Ogre.h"
#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphRider: public GraphUnit
{
public:

	GraphRider(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphRider();

    virtual bool  HandleMessage(const Telegram& msg);

    virtual void Update(double timeSinceLastFrame);
    
private:

};  

#endif GRAPH_Rider_