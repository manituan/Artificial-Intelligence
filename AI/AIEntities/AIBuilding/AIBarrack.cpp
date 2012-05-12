#include "AIBarrack.h"


AIBarrack::AIBarrack()
{

}

AIBarrack::AIBarrack(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation)
{
    m_bIsAttacking = bIsAttacking;
    m_iType = type;
    m_vPos = position;
    m_iCollisionType = square;
    m_vPrevPos = m_vPos;
    m_vOri = orientation.normalisedCopy();
    m_dActualHealth = 2000;
    m_dTotalHealth = 2000;
    m_dRadius = 40.0;
    MessageSystem::getSingletonPtr()->DispatchMessageToGraphFactory(this->ID(), factoryGraph, SMSG_AICreateGraphBuilding, position, m_vOri, type, race, Vector2(m_dActualHealth, m_dTotalHealth));

    m_pStateMachine = new AIStateSystem<AIBarrack>(this);
    m_pStateMachine->SetCurrentState(PeaceBarrack::Instance());

}

void AIBarrack::Update(double timeElapsed)
{

    m_pStateMachine->Update();

    MessageSystem::getSingletonPtr()->DispatchMessageGraphUnit(this->ID(), this->ID(), SMSG_AISystemToGraphSystem, this->getPosition(), 
        this->getOrientation(), timeElapsed, this->m_iType, race(), getAnimation(), this->getActualHealth());

}

bool AIBarrack::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void AIBarrack::decreaseHealth()
{

}