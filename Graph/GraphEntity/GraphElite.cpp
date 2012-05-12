#include "GraphElite.h"

//bool GraphUnit::m_bInstance = false;

GraphElite::GraphElite(int senderID, int UnitType, int Race, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth)
{
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->getSceneManager("GameSceneMgr");

    switch(Race)
	{
	case HUMAN:
		{    
            m_pInstancedEntity = OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanElite->createInstancedEntity("Examples/Instancing/VTF/Human_Elite");
		}
		break;

    case BEAST:
        {
            m_pInstancedEntity = OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastElite->createInstancedEntity("Examples/Instancing/VTF/Beast_Elite");
        }
        break;
/*
    case UNDEAD:
        {
            m_pInstancedEntity = OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadElite->createInstancedEntity("Examples/Instancing/VTF/Undead_Elite"); 
        }
        break;
        */
	};  



    pseudoRootNode = OGRE_NEW SceneNode(0);
    pseudoRootNode->_notifyRootNode();
    m_pNode = OGRE_NEW SceneNode(0);
    pseudoRootNode->addChild(m_pNode);

    m_dHealth = vHealth.x;
    m_dTotalHealth = vHealth.y;

    m_pNode->attachObject(m_pInstancedEntity);
    //setNodeOrientation(vRot.x, 0, vRot.y);
    m_pAnimation = m_pInstancedEntity->getAnimationState("breathing01");
    m_pAnimation->setLoop(true);
    m_pAnimation->setEnabled(true);

    createLifeBar(m_pNode->getScale());

    m_UnitName = "Elite";
    
    SetID(senderID);
    m_pKOKDefine = 0;
    m_vPos = vPos;
    m_vOri = vRot;

}

GraphElite::~GraphElite()
{
    m_pInstancedEntity->setVisible(false);
}

bool GraphElite::HandleMessage(const Telegram& msg)
{
    switch( msg.Msg )
	{
	case SMSG_AISystemToGraphSystem:
		{    
            
            setNodeOrientation(msg);

            setNodeTraslation(msg.Position.x, 0, msg.Position.y, msg.DeltaTime);  

            setNodeAnimation(msg);

            m_dHealth = msg.dActualHealth;

            return true;
		}
		break;
	};  
    return false;
}

void GraphElite::Update(double timeSinceLastFrame)
{
    m_pAnimation->addTime(0.001 * timeSinceLastFrame);
    m_pLifeNode->setPosition(Vector3(m_pNode->getPosition().x, m_pNode->getPosition().y + 4, m_pNode->getPosition().z));
    updateLifeBar(m_pLifeBar, m_dHealth, m_dTotalHealth);
}
