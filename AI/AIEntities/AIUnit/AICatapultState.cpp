#include "AICatapultState.h"

SeekToFlagCatapult* SeekToFlagCatapult::Instance()
{
  static SeekToFlagCatapult instance;
  return &instance;
}

bool SeekToFlagCatapult::Enter(AICatapult* pCatapult)
{
    pCatapult->setAnimation(WALK);
    pCatapult->setStop(false); 
    return true;
}

bool SeekToFlagCatapult::Execute(AICatapult* pCatapult)
{  

    if (pCatapult->getActualHealth() == 0)
    {
        pCatapult->GetFSM()->ChangeState(DieCatapult::Instance());
        return true;
    }

    if (pCatapult->seeEnemy(pCatapult->IsAttacking()))
    {

        if (pCatapult->attackRange(pCatapult->getObjetive()))
        {
            pCatapult->GetFSM()->ChangeState(AttackCatapult::Instance());
            return true;
        }
    /*
        if(pCatapult->surrounded(pCatapult))
        {
            pCatapult->GetFSM()->ChangeState(WaitCatapult::Instance());
            return true;
        } 
    */
        pCatapult->GetFSM()->ChangeState(ChargeCatapult::Instance());
        return true;
    
    }   
 
    pCatapult->seekFlag();
    pCatapult->setOri(pCatapult->getObjetive()->getPosition() - pCatapult->getPosition());

    if(pCatapult->getCollisions()->evaluate(pCatapult, pCatapult->getObjetive()))
    {
        pCatapult->GetFSM()->ChangeState(AttackCatapult::Instance());
    }

    return true;
}

bool SeekToFlagCatapult::Exit(AICatapult* pCatapult)
{
    return false;
}

bool SeekToFlagCatapult::OnMessage(AICatapult* pCatapult, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()))>0)
          {
            pCatapult->setActualHealth(pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()));
            // pCatapult->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pCatapult->GetFSM()->ChangeState(ChargeCatapult::Instance());
          }
          else
            pCatapult->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeCatapult* ChargeCatapult::Instance()
{
  static ChargeCatapult instance;
  return &instance;
}

bool ChargeCatapult::Enter(AICatapult* pCatapult)
{  
    pCatapult->setStop(false);
    pCatapult->setAnimation(CHARGE);
    pCatapult->setOri(pCatapult->getObjetive()->getPosition() - pCatapult->getPosition());
    return true;
}

bool ChargeCatapult::Execute(AICatapult* pCatapult)
{

    if(pCatapult->getActualHealth() == 0)
    {
        pCatapult->GetFSM()->ChangeState(DieCatapult::Instance());
        return true;
    }
    
    if (pCatapult->attackRange(pCatapult->getObjetive()))
    {
        pCatapult->GetFSM()->ChangeState(AttackCatapult::Instance());
        return true;
    }

    if(pCatapult->getCollisions()->evaluate(pCatapult, pCatapult->getObjetive()))
    {
        pCatapult->GetFSM()->ChangeState(AttackCatapult::Instance());
        return true;
    }

    // SeeEnemy2 doesn´t change the Objetive
    if (!pCatapult->seeEnemy2(pCatapult->IsAttacking()))
    {
        pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
        return true;
    }
/*   
    if(pCatapult->surrounded(pCatapult))
    {
        pCatapult->GetFSM()->ChangeState(WaitCatapult::Instance());
        return true;
    }
*/   
    pCatapult->charge();

    return false;
}

bool ChargeCatapult::Exit(AICatapult* pCatapult)
{
    return false;
}

bool ChargeCatapult::OnMessage(AICatapult* pCatapult, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()))>0)
          {
              /*
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pCatapult->getPosition() - aux->getPosition()).length() < (pCatapult->getPosition() - pCatapult->getObjetive()->getPosition()).length())
                pCatapult->setObjetive(aux);
              */
            pCatapult->setActualHealth(pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()));
          }
          else
            pCatapult->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackCatapult* AttackCatapult::Instance()
{
    static AttackCatapult instance;
    return &instance;
}

bool AttackCatapult::Enter(AICatapult* pCatapult)
{   
    pCatapult->setStop(true);
    pCatapult->setAnimation(ATTACK);
    pCatapult->setOri(pCatapult->getObjetive()->getPosition() - pCatapult->getPosition());
    return true;
}

bool AttackCatapult::Execute(AICatapult* pCatapult)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pCatapult->ID(),
                              pCatapult->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pCatapult->getActualHealth() == 0)
    {
        pCatapult->GetFSM()->ChangeState(DieCatapult::Instance());
        return true;
    }
    
    if(pCatapult->getCollisions()->evaluate(pCatapult, pCatapult->getObjetive()))
    {
        return true;
    }
    
    if (!pCatapult->attackRange(pCatapult->getObjetive()))
    {
        pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
        return true;
    }

    return true;
}

bool AttackCatapult::Exit(AICatapult* pCatapult)
{ 
    return false;
}

bool AttackCatapult::OnMessage(AICatapult* pCatapult, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
          // pCatapult->setObjetive(NULL);
          break;
      }

      case LMSG_AttackTo:
      {
          if((pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()))>0)
            pCatapult->setActualHealth(pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()));
          else
            pCatapult->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieCatapult* DieCatapult::Instance()
{
  static DieCatapult instance;
  return &instance;
}

bool DieCatapult::Enter(AICatapult* pCatapult)
{
    pCatapult->setAnimation(DIE);  
    return true;
}

bool DieCatapult::Execute(AICatapult* pCatapult)
{ 
    pCatapult->GetFSM()->ChangeState(DieCatapult::Instance()); 
    return true;
}

bool DieCatapult::Exit(AICatapult* pCatapult)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pCatapult).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pCatapult->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pCatapult)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pCatapult->die(pCatapult);
    return true;
}

bool DieCatapult::OnMessage(AICatapult* pCatapult, const Telegram& msg)
{
    return false;
}

WaitCatapult* WaitCatapult::Instance()
{
  static WaitCatapult instance;
  return &instance;
}

bool WaitCatapult::Enter(AICatapult* pCatapult)
{
    pCatapult->setStop(true);
    pCatapult->setAnimation(BREATH);
    pCatapult->setOri(pCatapult->getObjetive()->getPosition() - pCatapult->getPosition());
    return true;
}

bool WaitCatapult::Execute(AICatapult* pCatapult)
{
    if (pCatapult->getActualHealth() == 0)
    {
        pCatapult->GetFSM()->ChangeState(DieCatapult::Instance());
        return true;
    }
/*
    if(!pCatapult->surrounded(pCatapult))
    {   
        pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
        return true;
    } 
*/
    if (pCatapult->seeEnemy(pCatapult->IsAttacking()))
    {
        if (pCatapult->attackRange(pCatapult->getObjetive()))
        {
            pCatapult->GetFSM()->ChangeState(AttackCatapult::Instance());
            return true;
        }
    // pCatapult->GetFSM()->ChangeState(ChargeCatapult::Instance());
    }
return false;
}

bool WaitCatapult::Exit(AICatapult* pCatapult)
{ 
    return true;
}

bool WaitCatapult::OnMessage(AICatapult* pCatapult, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pCatapult->GetFSM()->ChangeState(SeekToFlagCatapult::Instance());
          //pCatapult->setObjetive(NULL);
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()))>0)
          {
            pCatapult->setActualHealth(pCatapult->getActualHealth() - pCatapult->calculateDamage(msg.Sender, pCatapult->getTime()));
            // pCatapult->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pCatapult->GetFSM()->ChangeState(ChargeCatapult::Instance());
          }
          else
            pCatapult->setActualHealth(0.0f);
          break;
      }
      return true;     
  }
  return false;
}