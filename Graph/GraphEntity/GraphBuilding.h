#ifndef GRAPH_BUILDING_
#define GRAPH_BUILDING_

#include "Ogre.h"
#include "GraphSystem.h"
#include "Opcode.h"
#include "Framework.h"
#include "GraphUnitDef.h"

class GraphBuilding: public GraphSystem
{
public:

    GraphBuilding(int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot){}

    GraphBuilding(){}

    ~GraphBuilding(){};

    virtual bool    HandleMessage(const Telegram& msg){return false;}

    virtual void Update(double timeSinceLastFrame){};

    void createLifeBar(Ogre::Vector3 scale);
    void updateLifeBar(Ogre::Billboard* pLifeBar, Ogre::Real life, Ogre::Real maxLife);

protected:

    Ogre::Billboard*    m_pLifeBar;
    Ogre::SceneNode*    m_pLifeNode;

    void setNodeTraslation(float x, float y, float z, float delta);

    void setNodeOrientation(const Telegram& msg);

    void setNodeAnimation(const Telegram& msg);

	std::string				m_BuildingName;

    // The direction the object is moving
    Ogre::Vector3			m_Direction;

    // Instance variables
    InstanceManager*        instanceManager;
    SceneNode*              pseudoRootNode;

    KOKDefine*   m_pKOKDefine;

    Ogre::Quaternion mOrientSrc;               // Initial orientation
    Ogre::Quaternion mOrientDest;              // Destination orientation
    Ogre::Real mRotProgress;                   // How far we've interpolated
    Ogre::Real mRotFactor;                     // Interpolation step
    bool mRotating;
    Ogre::BillboardSet* pLifeBarSet;
    int RotationCount;

};  

#endif GRAPH_UNIT_