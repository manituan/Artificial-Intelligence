#ifndef GRAPH_Barrack_
#define GRAPH_Barrack_

#include "Ogre.h"
#include "GraphBuilding.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphBarrack: public GraphBuilding
{
public:

	GraphBarrack(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphBarrack();

    virtual bool  HandleMessage(const Telegram& msg);
    virtual void Update(double timeSinceLastFrame);
    
private:
    
};  

#endif GRAPH_Barrack_