#ifndef GraphSystem_H
#define GraphSystem_H

#include <Ogre.h>
#include "Opcode.h"
#include <OgreInstancedEntity.h>
#include "Framework.h"

struct MsgServerBase;

using namespace Ogre;

// This class is the equivalent to AISystem, but only for graphics.

class GraphSystem
{
public:

	virtual ~GraphSystem(){}

	virtual bool HandleMessage(const Telegram& msg){ return false; }

    virtual void Update(double timeSinceLastFrame){};

    int			    ID()const{return m_ID;}

    //use this to grab the next valid ID
    static int		getNextValidID(){return m_iNextValidID;}
  
    //this can be used to reset the next ID
    static void		resetNextValidID(){m_iNextValidID = 0;}

    Ogre::Vector2   getPosition()const{return m_vPos;}
    void            setPos(Vector2 newPos){m_vPos = newPos;}

    Ogre::Vector2   orientation()const{return m_vOri;}
    void            setOri(Vector2 newOri){m_vOri = newOri;}

    int             getEntityType()const{return m_iType;}
    void            setEntityType(int newType){m_iType = newType;}

    AnimationState* getAnimationState(){return m_pAnimation;}

private:

    //each entity has a unique ID
	int         m_ID;

	//this is the next valid ID. Each time a AISystem is instantiated
	//this value is updated
	static int  m_iNextValidID;

protected:

	//this must be called within each constructor to make sure the ID is set
	//correctly. It verifies that the value passed to the method is greater
	//or equal to the next valid ID, before setting the ID and incrementing
	//the next valid ID
	void                    SetID(int val);

    //ID of the AI entity to send message
	int						m_AIOwnerID;			  

    //its location in the world
    Vector2                 m_vPos;

    Vector2                 m_vOri;

    int                     m_iType;

    int                     m_iRace;

    // Optional
    // SceneNode*  m_pProyectile;

    SceneManager*           m_pSceneMgr;

    InstancedEntity*        m_pInstancedEntity;

    Entity*                 m_pEntity;

    SceneNode*              m_pNode;

    Ogre::AnimationState*   m_pAnimation;

    double                  m_dHealth;
    double                  m_dTotalHealth;

    GraphSystem(int ID);  

    GraphSystem(){}
};

#endif