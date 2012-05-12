#include "AIEliteState.h"

SeekToFlagElite* SeekToFlagElite::Instance()
{
  static SeekToFlagElite instance;
  return &instance;
}

bool SeekToFlagElite::Enter(AIElite* pElite)
{
    pElite->setAnimation(WALK);
    pElite->setStop(false); 
    // pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    return true;
}

bool SeekToFlagElite::Execute(AIElite* pElite)
{  

    if (pElite->getActualHealth() == 0)
    {
        pElite->GetFSM()->ChangeState(DieElite::Instance());
        return true;
    }

    if (pElite->seeEnemy(pElite->IsAttacking()))
    {

        if (pElite->attackRange(pElite->getObjetive()))
        {
            pElite->GetFSM()->ChangeState(AttackElite::Instance());
            return true;
        }

        if(pElite->surrounded(pElite))
        {
            pElite->GetFSM()->ChangeState(WaitElite::Instance());
            return true;
        } 
        
        pElite->GetFSM()->ChangeState(ChargeElite::Instance());
        return true;   
    }  

    pElite->seekFlag();
    pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    if(pElite->getCollisions()->evaluate(pElite, pElite->getObjetive()))
    {
        pElite->GetFSM()->ChangeState(AttackElite::Instance());
    }

    return true;
}

bool SeekToFlagElite::Exit(AIElite* pElite)
{
    return false;
}

bool SeekToFlagElite::OnMessage(AIElite* pElite, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()))>0)
          {
            pElite->setActualHealth(pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()));
            pElite->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pElite->GetFSM()->ChangeState(ChargeElite::Instance());
          }
          else
            pElite->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeElite* ChargeElite::Instance()
{
  static ChargeElite instance;
  return &instance;
}

bool ChargeElite::Enter(AIElite* pElite)
{  
    pElite->setStop(false);
    pElite->setAnimation(CHARGE);
    pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    return true;
}

bool ChargeElite::Execute(AIElite* pElite)
{

    if (pElite->getActualHealth() == 0)
    {
        pElite->GetFSM()->ChangeState(DieElite::Instance());
        return true;
    }

    if (pElite->attackRange(pElite->getObjetive()))
    {
        pElite->GetFSM()->ChangeState(AttackElite::Instance());
        return true;
    }

    if(pElite->getCollisions()->evaluate(pElite, pElite->getObjetive()))
    {
        pElite->GetFSM()->ChangeState(AttackElite::Instance());
        return true;
    }

    if(pElite->surrounded(pElite))
    {
        pElite->GetFSM()->ChangeState(WaitElite::Instance());
        return true;
    }

    if (!pElite->seeEnemy2(pElite->IsAttacking()))
    {
        pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
        return true;
    }

    pElite->charge();

    return false;
}

bool ChargeElite::Exit(AIElite* pElite)
{
    return false;
}

bool ChargeElite::OnMessage(AIElite* pElite, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pElite->getPosition() - aux->getPosition()).length() < (pElite->getPosition() - pElite->getObjetive()->getPosition()).length())
                pElite->setObjetive(aux);
            pElite->setActualHealth(pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()));
          }
          else
            pElite->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackElite* AttackElite::Instance()
{
    static AttackElite instance;
    return &instance;
}

bool AttackElite::Enter(AIElite* pElite)
{   
    pElite->setStop(true);
    pElite->setAnimation(ATTACK);
    pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    return true;
}

bool AttackElite::Execute(AIElite* pElite)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pElite->ID(),
                              pElite->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pElite->getActualHealth() == 0)
    {
        pElite->GetFSM()->ChangeState(DieElite::Instance());
        return true;
    }

    if(pElite->getCollisions()->evaluate(pElite, pElite->getObjetive()))
    {
        return true;
    }

    if (!pElite->attackRange(pElite->getObjetive()))
    {
        pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
        return true;
    }

    return true;
}

bool AttackElite::Exit(AIElite* pElite)
{ 
    return false;
}

bool AttackElite::OnMessage(AIElite* pElite, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()))>0)
            pElite->setActualHealth(pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()));
          else
            pElite->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieElite* DieElite::Instance()
{
  static DieElite instance;
  return &instance;
}

bool DieElite::Enter(AIElite* pElite)
{
    pElite->setAnimation(DIE);  
    pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    return true;
}

bool DieElite::Execute(AIElite* pElite)
{ 
    pElite->GetFSM()->ChangeState(DieElite::Instance()); 
    return true;
}

bool DieElite::Exit(AIElite* pElite)
{ 
    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pElite).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pElite->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pElite)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pElite->die(pElite);
    return true;
}

bool DieElite::OnMessage(AIElite* pElite, const Telegram& msg)
{
    return false;
}

WaitElite* WaitElite::Instance()
{
  static WaitElite instance;
  return &instance;
}

bool WaitElite::Enter(AIElite* pElite)
{
    pElite->setStop(true);
    pElite->setAnimation(BREATH);
    pElite->setOri(pElite->getObjetive()->getPosition() - pElite->getPosition());
    return true;
}

bool WaitElite::Execute(AIElite* pElite)
{ 
    if (pElite->getActualHealth() == 0)
    {
        pElite->GetFSM()->ChangeState(DieElite::Instance());
        return true;
    }

    if(!pElite->surrounded(pElite))
    {
        pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
        return true;
    } 

    if (pElite->seeEnemy(pElite->IsAttacking()))
    {

        if (pElite->attackRange(pElite->getObjetive()))
        {
            pElite->GetFSM()->ChangeState(AttackElite::Instance());
            return true;
        }

        //pElite->GetFSM()->ChangeState(ChargeElite::Instance());
        //return true;
    
    }  

    return true;
}

bool WaitElite::Exit(AIElite* pElite)
{ 
    return true;
}

bool WaitElite::OnMessage(AIElite* pElite, const Telegram& msg)
{

  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()))>0)
          {
            pElite->setActualHealth(pElite->getActualHealth() - pElite->calculateDamage(msg.Sender, pElite->getTime()));
          pElite->GetFSM()->ChangeState(SeekToFlagElite::Instance());
          }
          else
            pElite->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}