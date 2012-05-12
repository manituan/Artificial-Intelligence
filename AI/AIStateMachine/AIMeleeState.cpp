#include "AIMeleeState.h"

SeekToFlagMelee* SeekToFlagMelee::Instance()
{
  static SeekToFlagMelee instance;
  return &instance;
}

bool SeekToFlagMelee::Enter(AIMelee* pMelee)
{
    pMelee->setAnimation(WALK);
    pMelee->setStop(false); 
    // pMelee->setOri(pMelee->getObjetive()->getPosition() - pMelee->getPosition());
    return true;
}

bool SeekToFlagMelee::Execute(AIMelee* pMelee)
{  

    if (pMelee->getActualHealth() == 0)
    {
        pMelee->GetFSM()->ChangeState(DieMelee::Instance());
        return true;
    }

    if (pMelee->seeEnemy(pMelee->IsAttacking()))
    {

        if (pMelee->attackRange(pMelee->getObjetive()))
        {
            pMelee->GetFSM()->ChangeState(AttackMelee::Instance());
            return true;
        }

        if(pMelee->surrounded(pMelee))
        {
            pMelee->GetFSM()->ChangeState(WaitMelee::Instance());
            return true;
        } 
    
        pMelee->GetFSM()->ChangeState(ChargeMelee::Instance());
        return true;
    
    }   
 
    pMelee->seekFlag();
    pMelee->setOri(pMelee->getObjetive()->getPosition() - pMelee->getPosition());

    if(pMelee->getCollisions()->evaluate(pMelee, pMelee->getObjetive()))
    {
        pMelee->GetFSM()->ChangeState(AttackMelee::Instance());
    }

    return true;
}

bool SeekToFlagMelee::Exit(AIMelee* pMelee)
{
    return false;
}

bool SeekToFlagMelee::OnMessage(AIMelee* pMelee, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()))>0)
          {
            pMelee->setActualHealth(pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()));
            pMelee->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pMelee->GetFSM()->ChangeState(ChargeMelee::Instance());
          }
          else
            pMelee->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeMelee* ChargeMelee::Instance()
{
  static ChargeMelee instance;
  return &instance;
}

bool ChargeMelee::Enter(AIMelee* pMelee)
{  
    pMelee->setStop(false);
    pMelee->setAnimation(CHARGE);
    pMelee->setOri(pMelee->getObjetive()->getPosition() - pMelee->getPosition());
    return true;
}

bool ChargeMelee::Execute(AIMelee* pMelee)
{

    if(pMelee->getActualHealth() == 0)
    {
        pMelee->GetFSM()->ChangeState(DieMelee::Instance());
        return true;
    }
    
    if (pMelee->attackRange(pMelee->getObjetive()))
    {
        pMelee->GetFSM()->ChangeState(AttackMelee::Instance());
        return true;
    }

    if(pMelee->getCollisions()->evaluate(pMelee, pMelee->getObjetive()))
    {
        pMelee->GetFSM()->ChangeState(AttackMelee::Instance());
        return true;
    }

    // SeeEnemy2 doesn´t change the Objetive
    if (!pMelee->seeEnemy2(pMelee->IsAttacking()))
    {
        pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
        return true;
    }
   
    if(pMelee->surrounded(pMelee))
    {
        pMelee->GetFSM()->ChangeState(WaitMelee::Instance());
        return true;
    }
   
    pMelee->charge();

    return false;
}

bool ChargeMelee::Exit(AIMelee* pMelee)
{
    return false;
}

bool ChargeMelee::OnMessage(AIMelee* pMelee, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pMelee->getPosition() - aux->getPosition()).length() < (pMelee->getPosition() - pMelee->getObjetive()->getPosition()).length())
                pMelee->setObjetive(aux);
            pMelee->setActualHealth(pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()));
          }
          else
            pMelee->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackMelee* AttackMelee::Instance()
{
    static AttackMelee instance;
    return &instance;
}

bool AttackMelee::Enter(AIMelee* pMelee)
{   
    pMelee->setStop(true);
    pMelee->setAnimation(ATTACK);
    pMelee->setOri(pMelee->getObjetive()->getPosition() - pMelee->getPosition());
    return true;
}

bool AttackMelee::Execute(AIMelee* pMelee)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pMelee->ID(),
                              pMelee->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pMelee->getActualHealth() == 0)
    {
        pMelee->GetFSM()->ChangeState(DieMelee::Instance());
        return true;
    }
    
    if(pMelee->getCollisions()->evaluate(pMelee, pMelee->getObjetive()))
    {
        return true;
    }
    
    if (!pMelee->attackRange(pMelee->getObjetive()))
    {
        pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
        return true;
    }

    return true;
}

bool AttackMelee::Exit(AIMelee* pMelee)
{ 
    return false;
}

bool AttackMelee::OnMessage(AIMelee* pMelee, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
          // pMelee->setObjetive(NULL);
          break;
      }

      case LMSG_AttackTo:
      {
          if((pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()))>0)
            pMelee->setActualHealth(pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()));
          else
            pMelee->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieMelee* DieMelee::Instance()
{
  static DieMelee instance;
  return &instance;
}

bool DieMelee::Enter(AIMelee* pMelee)
{
    pMelee->setAnimation(DIE);  
    return true;
}

bool DieMelee::Execute(AIMelee* pMelee)
{ 
    pMelee->GetFSM()->ChangeState(DieMelee::Instance()); 
    return true;
}

bool DieMelee::Exit(AIMelee* pMelee)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pMelee).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pMelee->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pMelee)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pMelee->die(pMelee);
    return true;
}

bool DieMelee::OnMessage(AIMelee* pMelee, const Telegram& msg)
{
    return false;
}

WaitMelee* WaitMelee::Instance()
{
  static WaitMelee instance;
  return &instance;
}

bool WaitMelee::Enter(AIMelee* pMelee)
{
    pMelee->setStop(true);
    pMelee->setAnimation(BREATH);
    pMelee->setOri(pMelee->getObjetive()->getPosition() - pMelee->getPosition());
    return true;
}

bool WaitMelee::Execute(AIMelee* pMelee)
{
    if (pMelee->getActualHealth() == 0)
    {
        pMelee->GetFSM()->ChangeState(DieMelee::Instance());
        return true;
    }

    if(!pMelee->surrounded(pMelee))
    {   
        pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
        return true;
    } 

    if (pMelee->seeEnemy(pMelee->IsAttacking()))
    {
        if (pMelee->attackRange(pMelee->getObjetive()))
        {
            pMelee->GetFSM()->ChangeState(AttackMelee::Instance());
            return true;
        }
    // pMelee->GetFSM()->ChangeState(ChargeMelee::Instance());
    }
return false;
}

bool WaitMelee::Exit(AIMelee* pMelee)
{ 
    return true;
}

bool WaitMelee::OnMessage(AIMelee* pMelee, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMelee->GetFSM()->ChangeState(SeekToFlagMelee::Instance());
          //pMelee->setObjetive(NULL);
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()))>0)
          {
            pMelee->setActualHealth(pMelee->getActualHealth() - pMelee->calculateDamage(msg.Sender, pMelee->getTime()));
            pMelee->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pMelee->GetFSM()->ChangeState(ChargeMelee::Instance());
          }
          else
            pMelee->setActualHealth(0.0f);
          break;
      }
      return true;     
  }
  return false;
}