#ifndef GRAPH_HALL_
#define GRAPH_HALL_

#include "Ogre.h"
#include "GraphBuilding.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphHall: public GraphBuilding
{
public:

	GraphHall(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphHall();

    virtual bool  HandleMessage(const Telegram& msg);
    virtual void Update(double timeSinceLastFrame);
    
protected:
    
};  

#endif GRAPH_HALL_