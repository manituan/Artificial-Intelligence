#ifndef AISYSTEM_H_
#define AISYSTEM_H_

#include <Ogre.h>
#include "Opcode.h"

using namespace Ogre;

class AISystem//: public AbIsAttacking
{
public:

    virtual         ~AISystem()
    {

    }

	virtual void Update(double time_elapsed)=0; 
  
	virtual bool    HandleMessage(const Telegram& msg){return false;}
	//virtual bool HandleMessage(int ClientID, MsgClientBase* msg){return false;}

    int			    ID()const{return m_ID;}

    //int			IDPlayer()const{return m_bIsAttacking;}
    bool			IsAttacking()const{return m_bIsAttacking;}

    bool            IsTagged()const{return m_bTag;}

    //use this to grab the next valid ID
    static int		getNextValidID(){return m_iNextValidID;}
  
    //this can be used to reset the next ID
    static void		resetNextValidID(){m_iNextValidID = 0;}

    Ogre::Vector2   getPosition()const{return m_vPos;}
    void            setPos(Vector2 newPos){m_vPos = newPos;}

    Ogre::Vector2   getPrevPosition()const{return m_vPrevPos;}
    void            setPrevPosition(Vector2 newPos){m_vPrevPos = newPos;}

    Ogre::Vector2   getOrientation()const{return m_vOri;}
    void            setOri(Vector2 newOri){m_vOri = newOri;}

    int getCollisionType(){return m_iCollisionType;}
    void setCollisionType(int col){m_iCollisionType = col;}

    int             getEntityType()const{return m_iType;}
    void            setEntityType(int newType){m_iType = newType;}

    double getTime(){return m_dTimeSinceLastFrame;}

    void            Tag(){m_bTag = true;}
    void            UnTag(){m_bTag = false;}

    double getRadius(){return m_dRadius;}

    int     m_iCollisionType;

private:

    //each entity has a unique ID
	int         m_ID;

	//this is the next valid ID. Each time a AISystem is instantiated
	//this value is updated
	static int  m_iNextValidID;

	//this must be called within each constructor to make sure the ID is set
	//correctly. It verifies that the value passed to the method is greater
	//or equal to the next valid ID, before setting the ID and incrementing
	//the next valid ID
	void        SetID(int val);

protected:

    bool            m_bIsAttacking;
    //	int         m_bIsAttacking;

    double          m_dRadius;

    //its location in the world
    Vector2     m_vPos;
    Vector2     m_vPrevPos;

    Vector2     m_vOri;

    int         m_iType;

    double      m_dTimeSinceLastFrame;

bool m_bTag;

	AISystem(int ID);  

    AISystem()
    {
        m_bTag = false;
        m_bIsAttacking = false;
    }
};

#endif