#include "AIWallsState.h"

PeaceWalls* PeaceWalls::Instance()
{
  static PeaceWalls instance;
  return &instance;
}

bool PeaceWalls::Enter(AIWalls* pWalls)
{
return true;
}

bool PeaceWalls::Execute(AIWalls* pWalls)
{  
    if (pWalls->getActualHealth() == 0)
    {
        pWalls->GetFSM()->ChangeState(DieWalls::Instance());
        return true;
    }
       
    pWalls->GetFSM()->ChangeState(IncreaseHealthWalls::Instance());
    return true;   
}

bool PeaceWalls::Exit(AIWalls* pWalls)
{
return true;
}

bool PeaceWalls::OnMessage(AIWalls* pWalls, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pWalls->getActualHealth() - pWalls->calculateDamage(msg.Sender, pWalls->getTime()))>0)
          {
            pWalls->setActualHealth(pWalls->getActualHealth() - pWalls->calculateDamage(msg.Sender, pWalls->getTime()));
          }
          else
            pWalls->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DecreaseHealthWalls* DecreaseHealthWalls::Instance()
{
  static DecreaseHealthWalls instance;
  return &instance;
}

bool DecreaseHealthWalls::Enter(AIWalls* pWalls)
{  
return true;
}

bool DecreaseHealthWalls::Execute(AIWalls* pWalls)
{
return true;
}

bool DecreaseHealthWalls::Exit(AIWalls* pWalls)
{
return true;
}

bool DecreaseHealthWalls::OnMessage(AIWalls* pWalls, const Telegram& msg)
{
    return false;
}

IncreaseHealthWalls* IncreaseHealthWalls::Instance()
{
    static IncreaseHealthWalls instance;
    return &instance;
}

bool IncreaseHealthWalls::Enter(AIWalls* pWalls)
{
return true;
}

bool IncreaseHealthWalls::Execute(AIWalls* pWalls)
{ 
return true;
}

bool IncreaseHealthWalls::Exit(AIWalls* pWalls)
{ 
return true;
}

bool IncreaseHealthWalls::OnMessage(AIWalls* pWalls, const Telegram& msg)
{
    return false;
}


DieWalls* DieWalls::Instance()
{
  static DieWalls instance;
  return &instance;
}

bool DieWalls::Enter(AIWalls* pWalls)
{
    return true;
}

bool DieWalls::Execute(AIWalls* pWalls)
{ 
    pWalls->GetFSM()->ChangeState(DieWalls::Instance()); 
    return true;
}

bool DieWalls::Exit(AIWalls* pWalls)
{ 
    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pWalls).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pWalls->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pWalls)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pWalls->die(pWalls);
    return true;
}

bool DieWalls::OnMessage(AIWalls* pWalls, const Telegram& msg)
{
    return false;
}