#ifndef _FACTORY_GRAPHICS_H_
#define _FACTORY_GRAPHICS_H_

#include <map>
#include <vector>
#include "Ogre.h"
#include "Opcode.h"
#include "GraphSystem.h"

#include "GraphMelee.h"
#include "GraphRanged.h"
#include "GraphRider.h"
#include "GraphMonster.h"
#include "GraphElite.h"
#include "GraphWizard.h"

#include "GraphBarrack.h"
#include "GraphHall.h"
#include "GraphWalls.h"

#include "RaySystem.h"

using namespace Ogre;

struct ServerMessage;

class FactoryGraphics: public Singleton<FactoryGraphics>
{

public:

    FactoryGraphics();
	~FactoryGraphics();

    void Update(float delta);

	//Manager Section

	GraphSystem* GetGraphEntityFromID( unsigned long ID );
	FactoryGraphics* GetGraphEntityFromSceneNode( Ogre::SceneNode* Node );

	//Factory Section
    virtual bool HandleMessage(const Telegram& msg);

    // create AISystem objects
    void         newGraphUnit(int IDsender, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);
    void         newGraphBuilding(int IDsender, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth);
    //this method stores a pointer to the entity in the std::vector
    //m_Entities at the index position indicated by the entity's ID
    //(makes for faster access)
    void            RegisterGraphics(GraphSystem* NewGraph);

    //returns a pointer to the entity with the ID given as a parameter
    GraphSystem*      GetGraphFromID(int id)const;

    //this method removes the entity from the list
    void            RemoveGraph(GraphSystem* pGraph);

    //clears all entities from the entity map
    void            Reset(){m_GraphMap.clear();}

protected:

	std::map<unsigned long, GraphSystem*> GraphEntityMap;
	std::vector<GraphSystem*> GraphEntityToDelete;

private:

    typedef std::map<int, GraphSystem*> GraphMap;
    
    //to facilitate quick lookup the entities are stored in a std::map, in which
    //pointers to entities are cross referenced by their identifying number
    GraphMap                m_GraphMap;

    FactoryGraphics(const FactoryGraphics&);
	FactoryGraphics& operator = (const FactoryGraphics&);

    GraphSystem*            m_pGraph;
    GraphMelee*             m_pGraphMelee;
    GraphRanged*            m_pGraphRanged;
    GraphRider*             m_pGraphRider;
    GraphMonster*           m_pGraphMonster;

};

#endif