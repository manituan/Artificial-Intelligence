#ifndef GRAPH_WIZARD_
#define GRAPH_WIZARD_

#include "Ogre.h"
#include "GraphUnit.h"
#include "Opcode.h"
#include "Framework.h"
#include "KOKDefine.h"

class GraphWizard: public GraphUnit
{
public:

    GraphWizard(int sender, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);

	~GraphWizard();

    virtual bool  HandleMessage(const Telegram& msg);

    virtual void Update(double timeSinceLastFrame);
    
private:



};  

#endif GRAPH_WIZARD_