#ifndef GRAPH_Walls_
#define GRAPH_Walls_

#include "Ogre.h"
#include "GraphBuilding.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphWalls: public GraphBuilding
{
public:

	GraphWalls(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphWalls();

    virtual bool  HandleMessage(const Telegram& msg);
    virtual void Update(double timeSinceLastFrame);
    
protected:

    
};  

#endif GRAPH_Walls_