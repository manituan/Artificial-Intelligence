#include "AIRiderState.h"

SeekToFlagRider* SeekToFlagRider::Instance()
{
  static SeekToFlagRider instance;
  return &instance;
}

bool SeekToFlagRider::Enter(AIRider* pRider)
{
    pRider->setAnimation(WALK);
    pRider->setStop(false); 
    //pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    return true;
}

bool SeekToFlagRider::Execute(AIRider* pRider)
{  
    // esta muerto?
    if (pRider->getActualHealth() == 0)
    {
        pRider->GetFSM()->ChangeState(DieRider::Instance());
        return true;
    }

    if (pRider->seeEnemy(pRider->IsAttacking()))
    {
        if (pRider->attackRange(pRider->getObjetive()))
        {
            pRider->GetFSM()->ChangeState(AttackRider::Instance());
            return true;
        }

        if(pRider->surrounded(pRider))
        {
            pRider->GetFSM()->ChangeState(WaitRider::Instance());
            return true;
        } 

        pRider->GetFSM()->ChangeState(ChargeRider::Instance());
        return true;   
    }   

    pRider->seekFlag();
    pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    if(pRider->getCollisions()->evaluate(pRider, pRider->getObjetive()))
    {
        pRider->GetFSM()->ChangeState(AttackRider::Instance());
    }
    return true;
}

bool SeekToFlagRider::Exit(AIRider* pRider)
{
    return false;
}

bool SeekToFlagRider::OnMessage(AIRider* pRider, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()))>0)
          {
            pRider->setActualHealth(pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()));
            pRider->setObjetive((AIGame*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pRider->GetFSM()->ChangeState(ChargeRider::Instance());
          }
          else
            pRider->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeRider* ChargeRider::Instance()
{
  static ChargeRider instance;
  return &instance;
}

bool ChargeRider::Enter(AIRider* pRider)
{  
    pRider->setStop(false);
    pRider->setAnimation(CHARGE);
    pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    return true;
}

bool ChargeRider::Execute(AIRider* pRider)
{

    if (pRider->getActualHealth() == 0)
    {
        pRider->GetFSM()->ChangeState(DieRider::Instance());
        return true;
    }

    if (pRider->attackRange(pRider->getObjetive()))
    {
        pRider->GetFSM()->ChangeState(AttackRider::Instance());
        return true;
    }

    if(pRider->getCollisions()->evaluate(pRider, pRider->getObjetive()))
    {
        pRider->GetFSM()->ChangeState(AttackRider::Instance());
        return true;
    }

    if (!pRider->seeEnemy2(pRider->IsAttacking()))
    {
        pRider->GetFSM()->ChangeState(SeekToFlagRider::Instance());
        return true;
    }

    if(pRider->surrounded(pRider))
    {
        pRider->GetFSM()->ChangeState(WaitRider::Instance());
        return true;
    }

    pRider->charge();

    return false;
}

bool ChargeRider::Exit(AIRider* pRider)
{
    return false;
}

bool ChargeRider::OnMessage(AIRider* pRider, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRider->GetFSM()->ChangeState(SeekToFlagRider::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pRider->getPosition() - aux->getPosition()).length() < (pRider->getPosition() - pRider->getObjetive()->getPosition()).length())
                pRider->setObjetive(aux);
            pRider->setActualHealth(pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()));
          }
          else
            pRider->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackRider* AttackRider::Instance()
{
    static AttackRider instance;
    return &instance;
}

bool AttackRider::Enter(AIRider* pRider)
{   
    pRider->setStop(true);
    pRider->setAnimation(ATTACK);
    pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    return true;
}

bool AttackRider::Execute(AIRider* pRider)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pRider->ID(),
                              pRider->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pRider->getActualHealth() == 0)
    {
        pRider->GetFSM()->ChangeState(DieRider::Instance());
        return true;
    }

    if(pRider->getCollisions()->evaluate(pRider, pRider->getObjetive()))
    {
        return true;
    }

    if (!pRider->attackRange(pRider->getObjetive()))
    {
        pRider->GetFSM()->ChangeState(SeekToFlagRider::Instance());
        return true;
    }


    return true;
}

bool AttackRider::Exit(AIRider* pRider)
{ 
    return false;
}

bool AttackRider::OnMessage(AIRider* pRider, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRider->GetFSM()->ChangeState(SeekToFlagRider::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()))>0)
            pRider->setActualHealth(pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()));
          else
            pRider->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieRider* DieRider::Instance()
{
  static DieRider instance;
  return &instance;
}

bool DieRider::Enter(AIRider* pRider)
{
    pRider->setAnimation(DIE);  
    pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    return true;
}

bool DieRider::Execute(AIRider* pRider)
{ 
    pRider->GetFSM()->ChangeState(DieRider::Instance()); 
    return true;
}

bool DieRider::Exit(AIRider* pRider)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRider).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pRider->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRider)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pRider->die(pRider);

    return true;
}

bool DieRider::OnMessage(AIRider* pRider, const Telegram& msg)
{
    return false;
}

WaitRider* WaitRider::Instance()
{
  static WaitRider instance;
  return &instance;
}

bool WaitRider::Enter(AIRider* pRider)
{
    pRider->setStop(true);
    pRider->setAnimation(BREATH);
    pRider->setOri(pRider->getObjetive()->getPosition() - pRider->getPosition());
    return true;
}

bool WaitRider::Execute(AIRider* pRider)
{ 
    if (pRider->getActualHealth() == 0)
    {
        pRider->GetFSM()->ChangeState(DieRider::Instance());
        return true;
    }

    if(!pRider->surrounded(pRider))
    {
        pRider->GetFSM()->ChangeState(WaitRider::Instance());
        return true;
    } 

    if (pRider->seeEnemy(pRider->IsAttacking()))
    {
        if (pRider->attackRange(pRider->getObjetive()))
        {
            pRider->GetFSM()->ChangeState(AttackRider::Instance());
            return true;
        }
        // pRider->GetFSM()->ChangeState(ChargeRider::Instance());
        // return true;   
    }   

    return true;
}

bool WaitRider::Exit(AIRider* pRider)
{ 
    return true;
}

bool WaitRider::OnMessage(AIRider* pRider, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRider->GetFSM()->ChangeState(SeekToFlagRider::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()))>0)
          {
            pRider->setActualHealth(pRider->getActualHealth() - pRider->calculateDamage(msg.Sender, pRider->getTime()));
            pRider->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pRider->GetFSM()->ChangeState(ChargeRider::Instance());
          }
          else
            pRider->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}