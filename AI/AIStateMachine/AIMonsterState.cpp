#include "AIMonsterState.h"

SeekToFlagMonster* SeekToFlagMonster::Instance()
{
  static SeekToFlagMonster instance;
  return &instance;
}

bool SeekToFlagMonster::Enter(AIMonster* pMonster)
{
    pMonster->setAnimation(WALK);
    pMonster->setStop(false); 
    //pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    return true;
}

bool SeekToFlagMonster::Execute(AIMonster* pMonster)
{  

    if (pMonster->getActualHealth() == 0)
    {
        pMonster->GetFSM()->ChangeState(DieMonster::Instance());
        return true;
    }

    if (pMonster->seeEnemy(pMonster->IsAttacking()))
    {

        if (pMonster->attackRange(pMonster->getObjetive()))
        {
            pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
            return true;
        }

        if(pMonster->surrounded(pMonster))
        {
            pMonster->GetFSM()->ChangeState(WaitMonster::Instance());
            return true;
        } 
    
        pMonster->GetFSM()->ChangeState(ChargeMonster::Instance());
        return true;
    
    }  

    pMonster->seekFlag();
    pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    if(pMonster->getCollisions()->evaluate(pMonster, pMonster->getObjetive()))
    {
        pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
    }
    return true;
}

bool SeekToFlagMonster::Exit(AIMonster* pMonster)
{
    return false;
}

bool SeekToFlagMonster::OnMessage(AIMonster* pMonster, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()))>0)
          {
            pMonster->setActualHealth(pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()));
            pMonster->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pMonster->GetFSM()->ChangeState(ChargeMonster::Instance());
          }
          else
            pMonster->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeMonster* ChargeMonster::Instance()
{
  static ChargeMonster instance;
  return &instance;
}

bool ChargeMonster::Enter(AIMonster* pMonster)
{  
    pMonster->setStop(false);
    pMonster->setAnimation(CHARGE);
    pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    return true;
}

bool ChargeMonster::Execute(AIMonster* pMonster)
{

    if (pMonster->getActualHealth() == 0)
    {
        pMonster->GetFSM()->ChangeState(DieMonster::Instance());
        return true;
    }

    if (pMonster->attackRange(pMonster->getObjetive()))
    {
        pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
        return true;
    }

    if(pMonster->getCollisions()->evaluate(pMonster, pMonster->getObjetive()))
    {
        pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
    }

    if(pMonster->surrounded(pMonster))
    {
        pMonster->GetFSM()->ChangeState(WaitMonster::Instance());
        return true;
    }

    if (!pMonster->seeEnemy2(pMonster->IsAttacking()))
    {
        pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
        return true;
    }

    pMonster->charge();

    return false;
}

bool ChargeMonster::Exit(AIMonster* pMonster)
{
    return false;
}

bool ChargeMonster::OnMessage(AIMonster* pMonster, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pMonster->getPosition() - aux->getPosition()).length() < (pMonster->getPosition() - pMonster->getObjetive()->getPosition()).length())
                pMonster->setObjetive(aux);
            pMonster->setActualHealth(pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()));
          }
          else
            pMonster->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackMonster* AttackMonster::Instance()
{
    static AttackMonster instance;
    return &instance;
}

bool AttackMonster::Enter(AIMonster* pMonster)
{   
    pMonster->setStop(true);
    pMonster->setAnimation(ATTACK);
    pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    return true;
}

bool AttackMonster::Execute(AIMonster* pMonster)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pMonster->ID(),
                              pMonster->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pMonster->getActualHealth() == 0)
    {
        pMonster->GetFSM()->ChangeState(DieMonster::Instance());
        return true;
    }

    if(pMonster->getCollisions()->evaluate(pMonster, pMonster->getObjetive()))
    {
        pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
    }

    if (!pMonster->attackRange(pMonster->getObjetive()))
    {
        pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
        return true;
    }

    return true;
}

bool AttackMonster::Exit(AIMonster* pMonster)
{ 
    return false;
}

bool AttackMonster::OnMessage(AIMonster* pMonster, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()))>0)
            pMonster->setActualHealth(pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()));
          else
            pMonster->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieMonster* DieMonster::Instance()
{
  static DieMonster instance;
  return &instance;
}

bool DieMonster::Enter(AIMonster* pMonster)
{
    pMonster->setAnimation(DIE);  
    pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    return true;
}

bool DieMonster::Execute(AIMonster* pMonster)
{ 
    pMonster->GetFSM()->ChangeState(DieMonster::Instance()); 
    return true;
}

bool DieMonster::Exit(AIMonster* pMonster)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pMonster).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pMonster->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pMonster)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pMonster->die(pMonster);

    return true;
}

bool DieMonster::OnMessage(AIMonster* pMonster, const Telegram& msg)
{
    return false;
}

WaitMonster* WaitMonster::Instance()
{
  static WaitMonster instance;
  return &instance;
}

bool WaitMonster::Enter(AIMonster* pMonster)
{
    pMonster->setStop(true);
    pMonster->setAnimation(BREATH);
    pMonster->setOri(pMonster->getObjetive()->getPosition() - pMonster->getPosition());
    return true;
}

bool WaitMonster::Execute(AIMonster* pMonster)
{ 
    if (pMonster->getActualHealth() == 0)
    {
        pMonster->GetFSM()->ChangeState(DieMonster::Instance());
        return true;
    }

    if(!pMonster->surrounded(pMonster))
    {
        pMonster->GetFSM()->ChangeState(WaitMonster::Instance());
        return true;
    } 

    if (pMonster->seeEnemy(pMonster->IsAttacking()))
    {
        if (pMonster->attackRange(pMonster->getObjetive()))
        {
            pMonster->GetFSM()->ChangeState(AttackMonster::Instance());
            return true;
        }
        // pMonster->GetFSM()->ChangeState(ChargeMonster::Instance());
        // return true;   
    }  

    return true;
}

bool WaitMonster::Exit(AIMonster* pMonster)
{ 
    return true;
}

bool WaitMonster::OnMessage(AIMonster* pMonster, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()))>0)
          {
            pMonster->setActualHealth(pMonster->getActualHealth() - pMonster->calculateDamage(msg.Sender, pMonster->getTime()));
                    pMonster->GetFSM()->ChangeState(SeekToFlagMonster::Instance());
          }
          else
            pMonster->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}