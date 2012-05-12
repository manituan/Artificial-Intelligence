#include "GraphHall.h"

//bool GraphUnit::m_bInstance = false;

GraphHall::GraphHall(int senderID, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth)
{
    
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->getSceneManager("GameSceneMgr");
    m_pEntity = m_pSceneMgr->createEntity("building_human_hall_level01_medium.ma.mesh");
    m_pNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    m_pNode->setPosition(Vector3(vPos.x, 0, vPos.y));
    m_pNode->setDirection(Vector3(vRot.x, 0, vRot.y));
    m_pNode->attachObject(m_pEntity);

    m_dHealth = vHealth.x;
    m_dTotalHealth = vHealth.y;

    createLifeBar(m_pNode->getScale());

    m_BuildingName = "Hall";
    
    SetID(senderID);
    m_pKOKDefine = 0;
    m_vPos = vPos;
    m_vOri = vRot;

}

GraphHall::~GraphHall()
{

}

bool GraphHall::HandleMessage(const Telegram& msg)
{
    switch( msg.Msg )
	{
	case SMSG_AISystemToGraphSystem:
		{    
            
            setNodeOrientation(msg);

            setNodeTraslation(msg.Position.x, 0, msg.Position.y, msg.DeltaTime);  

            //setNodeAnimation(msg);

            m_dHealth = msg.dActualHealth;

            return true;
		}
		break;
	};  
    return false;
}

void GraphHall::Update(double timeSinceLastFrame)
{
    //m_pAnimation->addTime(0.001 * timeSinceLastFrame);
    m_pLifeNode->setPosition(Vector3(m_pNode->getPosition().x, m_pNode->getPosition().y + 40, m_pNode->getPosition().z));
    updateLifeBar(m_pLifeBar, m_dHealth, m_dTotalHealth);
}
