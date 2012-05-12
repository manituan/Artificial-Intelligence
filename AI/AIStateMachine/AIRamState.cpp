#include "AIRamState.h"

SeekToFlagRam* SeekToFlagRam::Instance()
{
  static SeekToFlagRam instance;
  return &instance;
}

bool SeekToFlagRam::Enter(AIRam* pRam)
{
    pRam->setAnimation(WALK);
    pRam->setStop(false); 
    // pRam->setOri(pRam->getObjetive()->getPosition() - pRam->getPosition());
    return true;
}

bool SeekToFlagRam::Execute(AIRam* pRam)
{  

    if (pRam->getActualHealth() == 0)
    {
        pRam->GetFSM()->ChangeState(DieRam::Instance());
        return true;
    }

    // if see muralla
    if(pRam->seeEnemy(pRam->IsAttacking()))
    {
        // if está a rango de disparo
        if(pRam->attackRange(pRam->getObjetive()))
        {
            pRam->GetFSM()->ChangeState(AttackRam::Instance());
            return true;
        }
        /*
        if(pRam->surrounded(pRam))
        {
            pRam->GetFSM()->ChangeState(WaitRam::Instance());
            return true;
        } 
        */
        pRam->GetFSM()->ChangeState(ChargeRam::Instance());
        return true;
    
    }   
 
    pRam->seekFlag();
    pRam->setOri(pRam->getObjetive()->getPosition() - pRam->getPosition());

    if(pRam->getCollisions()->evaluate(pRam, pRam->getObjetive()))
    {
        pRam->GetFSM()->ChangeState(AttackRam::Instance());
    }

    return true;
}

bool SeekToFlagRam::Exit(AIRam* pRam)
{
    return false;
}

bool SeekToFlagRam::OnMessage(AIRam* pRam, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()))>0)
          {
            pRam->setActualHealth(pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()));

            // Muralla->Objetivo
            pRam->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));

            // pRam->GetFSM()->ChangeState(ChargeRam::Instance());
          }
          else
            pRam->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeRam* ChargeRam::Instance()
{
  static ChargeRam instance;
  return &instance;
}

bool ChargeRam::Enter(AIRam* pRam)
{  
    pRam->setStop(false);
    pRam->setOri(pRam->getObjetive()->getPosition() - pRam->getPosition());
    return true;
}

bool ChargeRam::Execute(AIRam* pRam)
{

    if(pRam->getActualHealth() == 0)
    {
        pRam->GetFSM()->ChangeState(DieRam::Instance());
        return true;
    }
    
    if (pRam->attackRange(pRam->getObjetive()))
    {
        pRam->GetFSM()->ChangeState(AttackRam::Instance());
        return true;
    }

    if(pRam->getCollisions()->evaluate(pRam, pRam->getObjetive()))
    {
        pRam->GetFSM()->ChangeState(AttackRam::Instance());
        return true;
    }

    // SeeEnemy2 doesn´t change the Objetive
    if (!pRam->seeEnemy2(pRam->IsAttacking()))
    {
        pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
        return true;
    }

/*   
    if(pRam->surrounded(pRam))
    {
        pRam->GetFSM()->ChangeState(WaitRam::Instance());
        return true;
    }
*/  
    pRam->charge();

    return false;
}

bool ChargeRam::Exit(AIRam* pRam)
{
    return false;
}

bool ChargeRam::OnMessage(AIRam* pRam, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()))>0)
          {
            /*
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);           
            if((pRam->getPosition() - aux->getPosition()).length() < (pRam->getPosition() - pRam->getObjetive()->getPosition()).length())
                pRam->setObjetive(aux);
            */
            pRam->setActualHealth(pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()));
          }
          else
            pRam->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackRam* AttackRam::Instance()
{
    static AttackRam instance;
    return &instance;
}

bool AttackRam::Enter(AIRam* pRam)
{   
    pRam->setStop(true);
    pRam->setAnimation(ATTACK);
    pRam->setOri(pRam->getObjetive()->getPosition() - pRam->getPosition());
    return true;
}

bool AttackRam::Execute(AIRam* pRam)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pRam->ID(),
                              pRam->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pRam->getActualHealth() == 0)
    {
        pRam->GetFSM()->ChangeState(DieRam::Instance());
        return true;
    }
    
    if(pRam->getCollisions()->evaluate(pRam, pRam->getObjetive()))
    {
        return true;
    }
    
    if (!pRam->attackRange(pRam->getObjetive()))
    {
        pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
        return true;
    }

    return true;
}

bool AttackRam::Exit(AIRam* pRam)
{ 
    return false;
}

bool AttackRam::OnMessage(AIRam* pRam, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
          // pRam->setObjetive(NULL);
          break;
      }

      case LMSG_AttackTo:
      {
          if((pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()))>0)
            pRam->setActualHealth(pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()));
          else
            pRam->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieRam* DieRam::Instance()
{
  static DieRam instance;
  return &instance;
}

bool DieRam::Enter(AIRam* pRam)
{
    pRam->setAnimation(DIE);  
    return true;
}

bool DieRam::Execute(AIRam* pRam)
{ 
    pRam->GetFSM()->ChangeState(DieRam::Instance()); 
    return true;
}

bool DieRam::Exit(AIRam* pRam)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRam).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pRam->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pRam)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pRam->die(pRam);
    return true;
}

bool DieRam::OnMessage(AIRam* pRam, const Telegram& msg)
{
    return false;
}

WaitRam* WaitRam::Instance()
{
  static WaitRam instance;
  return &instance;
}

bool WaitRam::Enter(AIRam* pRam)
{
    pRam->setStop(true);
    pRam->setAnimation(BREATH);
    pRam->setOri(pRam->getObjetive()->getPosition() - pRam->getPosition());
    return true;
}

bool WaitRam::Execute(AIRam* pRam)
{
    if (pRam->getActualHealth() == 0)
    {
        pRam->GetFSM()->ChangeState(DieRam::Instance());
        return true;
    }
/*
    if(!pRam->surrounded(pRam))
    {   
        pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
        return true;
    } 
*/
    if (pRam->seeEnemy(pRam->IsAttacking()))
    {
        if (pRam->attackRange(pRam->getObjetive()))
        {
            pRam->GetFSM()->ChangeState(AttackRam::Instance());
            return true;
        }
    // pRam->GetFSM()->ChangeState(ChargeRam::Instance());
    }
return false;
}

bool WaitRam::Exit(AIRam* pRam)
{ 
    return true;
}

bool WaitRam::OnMessage(AIRam* pRam, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pRam->GetFSM()->ChangeState(SeekToFlagRam::Instance());
          //pRam->setObjetive(NULL);
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()))>0)
          {
            pRam->setActualHealth(pRam->getActualHealth() - pRam->calculateDamage(msg.Sender, pRam->getTime()));
            // pRam->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pRam->GetFSM()->ChangeState(ChargeRam::Instance());
          }
          else
            pRam->setActualHealth(0.0f);
          break;
      }
      return true;     
  }
  return false;
}