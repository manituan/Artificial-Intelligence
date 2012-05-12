#include "AIHall.h"


AIHall::AIHall()
{

}

AIHall::AIHall(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation)
{
    m_bIsAttacking = bIsAttacking;
    m_iType = type;
    m_vPos = position;
    m_iCollisionType = square;
    m_vPrevPos = m_vPos;
    m_vOri = orientation.normalisedCopy();
    m_dRadius = 80.0;

    MessageSystem::getSingletonPtr()->DispatchMessageToGraphFactory(this->ID(), factoryGraph, SMSG_AICreateGraphBuilding, position, m_vOri, type, race, Vector2(m_dActualHealth, m_dTotalHealth));

    m_pStateMachine = new AIStateSystem<AIHall>(this);
    m_pStateMachine->SetCurrentState(PeaceHall::Instance());

}

void AIHall::Update(double timeElapsed)
{

    // El update de AIHall va a la máquina de estados y según en el que esté calcula un steering behavior. 
    // Si queremos un update cada cierto tiempo la unidad tiene que seguir moviendose en ese plazo, por lo que debe haber 2 updates.
    // 1 se debería llamar cada frame y el otro 5 veces cada 60 frames. Necesitamos la dirección anterior.
    // reset the steering force

    m_pStateMachine->Update();

    //int receiver = FactoryGraphics::getSingletonPtr()->GetGraphEntityFromID(this->ID());
    MessageSystem::getSingletonPtr()->DispatchMessageGraphUnit(this->ID(), this->ID(), SMSG_AISystemToGraphSystem, this->getPosition(), 
        this->getOrientation(), timeElapsed, this->m_iType, race(), getAnimation(), m_dActualHealth);

}

bool AIHall::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void AIHall::decreaseHealth()
{

}