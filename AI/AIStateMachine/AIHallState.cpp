#include "AIHallState.h"

PeaceHall* PeaceHall::Instance()
{
  static PeaceHall instance;
  return &instance;
}

bool PeaceHall::Enter(AIHall* pHall)
{
return true;
}

bool PeaceHall::Execute(AIHall* pHall)
{  
    if (pHall->getActualHealth() == 0)
    {
        pHall->GetFSM()->ChangeState(DieHall::Instance());
        return true;
    }
       
    pHall->GetFSM()->ChangeState(IncreaseHealthHall::Instance());
    return true;   
}

bool PeaceHall::Exit(AIHall* pHall)
{
return true;
}

bool PeaceHall::OnMessage(AIHall* pHall, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pHall->getActualHealth() - pHall->calculateDamage(msg.Sender, pHall->getTime()))>0)
          {
            pHall->setActualHealth(pHall->getActualHealth() - pHall->calculateDamage(msg.Sender, pHall->getTime()));
          }
          else
            pHall->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DecreaseHealthHall* DecreaseHealthHall::Instance()
{
  static DecreaseHealthHall instance;
  return &instance;
}

bool DecreaseHealthHall::Enter(AIHall* pHall)
{  
return true;
}

bool DecreaseHealthHall::Execute(AIHall* pHall)
{
return true;
}

bool DecreaseHealthHall::Exit(AIHall* pHall)
{
return true;
}

bool DecreaseHealthHall::OnMessage(AIHall* pHall, const Telegram& msg)
{
    return false;
}

IncreaseHealthHall* IncreaseHealthHall::Instance()
{
    static IncreaseHealthHall instance;
    return &instance;
}

bool IncreaseHealthHall::Enter(AIHall* pHall)
{
return true;
}

bool IncreaseHealthHall::Execute(AIHall* pHall)
{ 
return true;
}

bool IncreaseHealthHall::Exit(AIHall* pHall)
{ 
return true;
}

bool IncreaseHealthHall::OnMessage(AIHall* pHall, const Telegram& msg)
{
    return false;
}

DieHall* DieHall::Instance()
{
  static DieHall instance;
  return &instance;
}

bool DieHall::Enter(AIHall* pHall)
{
    return true;
}

bool DieHall::Execute(AIHall* pHall)
{ 
    pHall->GetFSM()->ChangeState(DieHall::Instance()); 
    return true;
}

bool DieHall::Exit(AIHall* pHall)
{ 
    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pHall).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pHall->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pHall)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pHall->die(pHall);
    return true;
}

bool DieHall::OnMessage(AIHall* pHall, const Telegram& msg)
{
    return false;
}