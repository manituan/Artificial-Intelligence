#include "AIRangedState.h"

SeekToFlagRanged* SeekToFlagRanged::Instance()
{
  static SeekToFlagRanged instance;
  return &instance;
}

bool SeekToFlagRanged::Enter(AIRanged* pRanged)
{
    pRanged->setAnimation(WALK);
    pRanged->setStop(false); 
    //pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    return true;
}

bool SeekToFlagRanged::Execute(AIRanged* pRanged)
{  

    if (pRanged->getActualHealth() == 0)
    {
        pRanged->GetFSM()->ChangeState(DieRanged::Instance());
        return true;
    }

    if (pRanged->seeEnemy(pRanged->IsAttacking()))
    {

        if (pRanged->attackRange(pRanged->getObjetive()))
        {
            pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
            return true;
        }

        if(pRanged->surrounded(pRanged))
        {
            pRanged->GetFSM()->ChangeState(WaitRanged::Instance());
            return true;
        } 
    
        pRanged->GetFSM()->ChangeState(ChargeRanged::Instance());
        return true;    
    }   

    pRanged->seekFlag();
    pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    if(pRanged->getCollisions()->evaluate(pRanged, pRanged->getObjetive()))
    {
        pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
    }
    return true;
}

bool SeekToFlagRanged::Exit(AIRanged* pRanged)
{
    return false;
}

bool SeekToFlagRanged::OnMessage(AIRanged* pRanged, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()))>0)
          {
            pRanged->setActualHealth(pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()));
            pRanged->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pRanged->GetFSM()->ChangeState(ChargeRanged::Instance());
          }
          else
            pRanged->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeRanged* ChargeRanged::Instance()
{
  static ChargeRanged instance;
  return &instance;
}

bool ChargeRanged::Enter(AIRanged* pRanged)
{  
    pRanged->setStop(false);
    pRanged->setAnimation(CHARGE);
    pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    return true;
}

bool ChargeRanged::Execute(AIRanged* pRanged)
{

    if (pRanged->getActualHealth() == 0)
    {
        pRanged->GetFSM()->ChangeState(DieRanged::Instance());
        return true;
    }

    if (pRanged->attackRange(pRanged->getObjetive()))
    {
        pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
        return true;
    }

    if(pRanged->getCollisions()->evaluate(pRanged, pRanged->getObjetive()))
    {
        pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
    }

    if(pRanged->surrounded(pRanged))
    {
        pRanged->GetFSM()->ChangeState(WaitRanged::Instance());
        return true;
    }

    if (!pRanged->seeEnemy2(pRanged->IsAttacking()))
    {
        pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
        return true;
    }

    pRanged->charge();

    return false;
}

bool ChargeRanged::Exit(AIRanged* pRanged)
{
    return false;
}

bool ChargeRanged::OnMessage(AIRanged* pRanged, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pRanged->getPosition() - aux->getPosition()).length() < (pRanged->getPosition() - pRanged->getObjetive()->getPosition()).length())
                pRanged->setObjetive(aux);
            pRanged->setActualHealth(pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()));
          }
          else
            pRanged->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackRanged* AttackRanged::Instance()
{
    static AttackRanged instance;
    return &instance;
}

bool AttackRanged::Enter(AIRanged* pRanged)
{   
    pRanged->setStop(true);
    pRanged->setAnimation(ATTACK);
    pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    return true;
}

bool AttackRanged::Execute(AIRanged* pRanged)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pRanged->ID(),
                              pRanged->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pRanged->getActualHealth() == 0)
    {
        pRanged->GetFSM()->ChangeState(DieRanged::Instance());
        return true;
    }

    if(pRanged->getCollisions()->evaluate(pRanged, pRanged->getObjetive()))
    {
        pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
    }

    if (!pRanged->attackRange(pRanged->getObjetive()))
    {
        pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
        return true;
    }

    return true;
}

bool AttackRanged::Exit(AIRanged* pRanged)
{ 
    return false;
}

bool AttackRanged::OnMessage(AIRanged* pRanged, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()))>0)
            pRanged->setActualHealth(pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()));
          else
            pRanged->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieRanged* DieRanged::Instance()
{
  static DieRanged instance;
  return &instance;
}

bool DieRanged::Enter(AIRanged* pRanged)
{
    pRanged->setAnimation(DIE);  
    pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    return true;
}

bool DieRanged::Execute(AIRanged* pRanged)
{ 
    pRanged->GetFSM()->ChangeState(DieRanged::Instance()); 
    return true;
}

bool DieRanged::Exit(AIRanged* pRanged)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRanged).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pRanged->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRanged)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pRanged->die(pRanged);

    return true;
}

bool DieRanged::OnMessage(AIRanged* pRanged, const Telegram& msg)
{
    return false;
}

WaitRanged* WaitRanged::Instance()
{
  static WaitRanged instance;
  return &instance;
}

bool WaitRanged::Enter(AIRanged* pRanged)
{
    pRanged->setStop(true);
    pRanged->setAnimation(BREATH);
    pRanged->setOri(pRanged->getObjetive()->getPosition() - pRanged->getPosition());
    return true;
}

bool WaitRanged::Execute(AIRanged* pRanged)
{ 
    if (pRanged->getActualHealth() == 0)
    {
        pRanged->GetFSM()->ChangeState(DieRanged::Instance());
        return true;
    }

    if(!pRanged->surrounded(pRanged))
    {
        pRanged->GetFSM()->ChangeState(WaitRanged::Instance());
        return true;
    } 

    if (pRanged->seeEnemy(pRanged->IsAttacking()))
    {
        if (pRanged->attackRange(pRanged->getObjetive()))
        {
            pRanged->GetFSM()->ChangeState(AttackRanged::Instance());
            return true;
        } 
        // pRanged->GetFSM()->ChangeState(ChargeRanged::Instance());
        // return true;
    
    }   

    return true;
}

bool WaitRanged::Exit(AIRanged* pRanged)
{ 
    return true;
}

bool WaitRanged::OnMessage(AIRanged* pRanged, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()))>0)
          {
            pRanged->setActualHealth(pRanged->getActualHealth() - pRanged->calculateDamage(msg.Sender, pRanged->getTime()));
                      pRanged->GetFSM()->ChangeState(SeekToFlagRanged::Instance());
          }
          else
            pRanged->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}