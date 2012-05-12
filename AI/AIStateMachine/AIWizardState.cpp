#include "AIWizardState.h"

SeekToFlagWizard* SeekToFlagWizard::Instance()
{
  static SeekToFlagWizard instance;
  return &instance;
}

bool SeekToFlagWizard::Enter(AIWizard* pWizard)
{
    pWizard->setAnimation(WALK);
    pWizard->setStop(false); 
    // pWizard->setOri(pWizard->getObjetive()->getPosition() - pWizard->getPosition());
    return true;
}

bool SeekToFlagWizard::Execute(AIWizard* pWizard)
{  

    if (pWizard->getActualHealth() == 0)
    {
        pWizard->GetFSM()->ChangeState(DieWizard::Instance());
        return true;
    }

    if (pWizard->seeEnemy(pWizard->IsAttacking()))
    {

        if (pWizard->attackRange(pWizard->getObjetive()))
        {
            pWizard->GetFSM()->ChangeState(AttackWizard::Instance());
            return true;
        }

        if(pWizard->surrounded(pWizard))
        {
            pWizard->GetFSM()->ChangeState(WaitWizard::Instance());
            return true;
        } 

        pWizard->GetFSM()->ChangeState(ChargeWizard::Instance());
        return true;   
    }   

    pWizard->seekFlag();
    pWizard->setOri(pWizard->getObjetive()->getPosition() - pWizard->getPosition());
    if(pWizard->getCollisions()->evaluate(pWizard, pWizard->getObjetive()))
    {
        pWizard->GetFSM()->ChangeState(AttackWizard::Instance());
    }
    return true;
}

bool SeekToFlagWizard::Exit(AIWizard* pWizard)
{
    return false;
}

bool SeekToFlagWizard::OnMessage(AIWizard* pWizard, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()))>0)
          {
            pWizard->setActualHealth(pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()));
            pWizard->setObjetive((AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender));
            pWizard->GetFSM()->ChangeState(ChargeWizard::Instance());
          }
          else
            pWizard->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

ChargeWizard* ChargeWizard::Instance()
{
  static ChargeWizard instance;
  return &instance;
}

bool ChargeWizard::Enter(AIWizard* pWizard)
{  
    pWizard->setStop(false);
    pWizard->setAnimation(CHARGE);
    pWizard->setOri(pWizard->getObjetive()->getPosition() - pWizard->getPosition());
    return true;
}

bool ChargeWizard::Execute(AIWizard* pWizard)
{

    if (pWizard->getActualHealth() == 0)
    {
        pWizard->GetFSM()->ChangeState(DieWizard::Instance());
        return true;
    }
    
    if (pWizard->attackRange(pWizard->getObjetive()))
    {
        pWizard->GetFSM()->ChangeState(AttackWizard::Instance());
        return true;
    }    
 
    if(pWizard->getCollisions()->evaluate(pWizard, pWizard->getObjetive()))
    {
        pWizard->GetFSM()->ChangeState(AttackWizard::Instance());
    }

    if(pWizard->surrounded(pWizard))
    {
        pWizard->GetFSM()->ChangeState(WaitWizard::Instance());
        return true;
    }

    if (!pWizard->seeEnemy2(pWizard->IsAttacking()))
    {
        pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
        return true;
    }

    pWizard->charge();

    return false;
}

bool ChargeWizard::Exit(AIWizard* pWizard)
{
    return false;
}

bool ChargeWizard::OnMessage(AIWizard* pWizard, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
          break;
      }
     
      case LMSG_AttackTo:
      {
          if((pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()))>0)
          {
            AIUnit* aux = (AIUnit*)EntityCityManager::getSingletonPtr()->GetEntity(msg.Sender);
            if((pWizard->getPosition() - aux->getPosition()).length() < (pWizard->getPosition() - pWizard->getObjetive()->getPosition()).length())
                pWizard->setObjetive(aux);
            pWizard->setActualHealth(pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()));
          }
          else
            pWizard->setActualHealth(0.0f);
          break;
      }
      return true;
      
  }
  return false;
}

AttackWizard* AttackWizard::Instance()
{
    static AttackWizard instance;
    return &instance;
}

bool AttackWizard::Enter(AIWizard* pWizard)
{   
    pWizard->setStop(true);
    pWizard->setAnimation(ATTACK);
    pWizard->setOri(pWizard->getObjetive()->getPosition() - pWizard->getPosition());
    return true;
}

bool AttackWizard::Execute(AIWizard* pWizard)
{ 
    MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                              pWizard->ID(),
                              pWizard->getObjetive()->ID(),
                              LMSG_AttackTo);  

    if (pWizard->getActualHealth() == 0)
    {
        pWizard->GetFSM()->ChangeState(DieWizard::Instance());
        return true;
    }

    if(pWizard->getCollisions()->evaluate(pWizard, pWizard->getObjetive()))
    {
        return true;
    }

    if (!pWizard->attackRange(pWizard->getObjetive()))
    {
        pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
        return true;
    }

    return true;
}

bool AttackWizard::Exit(AIWizard* pWizard)
{ 
    return false;
}

bool AttackWizard::OnMessage(AIWizard* pWizard, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()))>0)
            pWizard->setActualHealth(pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()));
          else
            pWizard->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DieWizard* DieWizard::Instance()
{
  static DieWizard instance;
  return &instance;
}

bool DieWizard::Enter(AIWizard* pWizard)
{
    pWizard->setAnimation(DIE);  
    return true;
}

bool DieWizard::Execute(AIWizard* pWizard)
{ 
    pWizard->GetFSM()->ChangeState(DieWizard::Instance()); 
    return true;
}

bool DieWizard::Exit(AIWizard* pWizard)
{ 

    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pWizard).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pWizard->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pWizard)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pWizard->die(pWizard);

    return true;
}

bool DieWizard::OnMessage(AIWizard* pWizard, const Telegram& msg)
{
    return false;
}

WaitWizard* WaitWizard::Instance()
{
  static WaitWizard instance;
  return &instance;
}

bool WaitWizard::Enter(AIWizard* pWizard)
{
    pWizard->setStop(true);
    pWizard->setAnimation(BREATH);
    pWizard->setOri(pWizard->getObjetive()->getPosition() - pWizard->getPosition());
    return true;
}

bool WaitWizard::Execute(AIWizard* pWizard)
{ 
    if (pWizard->getActualHealth() == 0)
    {
        pWizard->GetFSM()->ChangeState(DieWizard::Instance());
        return true;
    }

    if(!pWizard->surrounded(pWizard))
    {
        pWizard->GetFSM()->ChangeState(WaitWizard::Instance());
        return true;
    } 

    if (pWizard->seeEnemy(pWizard->IsAttacking()))
    {
        if (pWizard->attackRange(pWizard->getObjetive()))
        {
            pWizard->GetFSM()->ChangeState(AttackWizard::Instance());
            return true;
        }
        // pWizard->GetFSM()->ChangeState(ChargeWizard::Instance());
        // return true;   
    }   

    return true;
}

bool WaitWizard::Exit(AIWizard* pWizard)
{ 
    return true;
}

bool WaitWizard::OnMessage(AIWizard* pWizard, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_ImDying:
      {
          pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
          break;
      }
      
      case LMSG_AttackTo:
      {
          if((pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()))>0)
          {
            pWizard->setActualHealth(pWizard->getActualHealth() - pWizard->calculateDamage(msg.Sender, pWizard->getTime()));
                    pWizard->GetFSM()->ChangeState(SeekToFlagWizard::Instance());
          }
          else
            pWizard->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}