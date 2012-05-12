#include "GraphUnit.h"


void GraphUnit::setNodeTraslation(float x, float y, float z, float delta)
{
    m_pNode->setPosition(Vector3(x,y,z));
}

void GraphUnit::setNodeOrientation(const Telegram& msg)
{
    /*
    Vector3 src = Vector3(m_pNode->getOrientation().x, 0, m_pNode->getOrientation().z) * Vector3::UNIT_Z;      // Orientation from initial direction

    Vector3 mDestination = Vector3(msg.Orientation.x, 0, msg.Orientation.y);   
    src.normalise();
    mDestination.normalise();
    Quaternion quat = src.getRotationTo(mDestination);
    m_pNode->rotate(quat);      

  */
    
/*
  RotationCount += 1;  
  if (RotationCount > 10)
  {
*/
      RotationCount = 0;
      Vector3 mDestination = Vector3(msg.Orientation.x, m_pNode->getOrientation().y, msg.Orientation.y);                    // mDestination is the next location
      Vector3 src = m_pNode->getOrientation() * Vector3::UNIT_Z;      // Orientation from initial direction
      src.y = 0;                                                      // Ignore pitch difference angle
      mDestination.y = 0;
      src.normalise();
      Real mDistance = mDestination.normalise();                     // Both vectors modified so renormalize them
      if ((1.0f + src.dotProduct(mDestination)) < 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
      {
          m_pNode->yaw(Degree(180));
      }
      else
      {
        Quaternion quat = src.getRotationTo(mDestination);
        m_pNode->rotate(quat);       
      } 
  //}
}

void GraphUnit::setNodeAnimation(const Telegram& msg)
{
    m_pAnimation->setEnabled(false);
    m_pAnimation = m_pInstancedEntity->getAnimationState(m_pKOKDefine->getAnimationName(msg.iAnimation));
    m_pAnimation->setEnabled(true);
    if(m_pAnimation->getAnimationName() == "dying01")
    {
        m_pAnimation->setLoop(false);
        pLifeBarSet->clear();
        //m_pLifeBar;
    }
}

void GraphUnit::updateLifeBar(Ogre::Billboard* pLifeBar, Ogre::Real life, Ogre::Real maxLife) {
    Ogre::Real ratio = life / maxLife;

    if (ratio < 0.0f)
        ratio = 0.0f;

    Ogre::Real u1 = (1.0f - ratio) / 2.0f;
    Ogre::Real v1 = 0.0f;
    Ogre::Real u2 = 0.5f + (1.0f - ratio) / 2.0f;
    Ogre::Real v2 = 1.0f;

    pLifeBar->setTexcoordRect(u1, v1, u2, v2);
}

void GraphUnit::createLifeBar(Ogre::Vector3 scale) 
{
    pLifeBarSet = m_pSceneMgr->createBillboardSet();
    pLifeBarSet->setMaterialName("lifeBar");
    m_pLifeBar = pLifeBarSet->createBillboard(Ogre::Vector3(0, 1.6, 0));
    m_pLifeBar->setDimensions(1, 0.1);
    m_pLifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);

    m_pLifeNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();

    m_pLifeNode->attachObject(pLifeBarSet);

    m_pLifeNode->setScale(10*scale.x, 10*scale.y, 10*scale.z);

    //m_vLifeBar.push_back(pLifeBar);

}