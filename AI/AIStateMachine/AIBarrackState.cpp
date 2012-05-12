#include "AIBarrackState.h"

PeaceBarrack* PeaceBarrack::Instance()
{
  static PeaceBarrack instance;
  return &instance;
}

bool PeaceBarrack::Enter(AIBarrack* pBarrack)
{
return true;
}

bool PeaceBarrack::Execute(AIBarrack* pBarrack)
{  
    if (pBarrack->getActualHealth() == 0)
    {
        pBarrack->GetFSM()->ChangeState(DieBarrack::Instance());
        return true;
    }
       
    pBarrack->GetFSM()->ChangeState(IncreaseHealthBarrack::Instance());
    return true;   
}

bool PeaceBarrack::Exit(AIBarrack* pBarrack)
{
return true;
}

bool PeaceBarrack::OnMessage(AIBarrack* pBarrack, const Telegram& msg)
{
  switch(msg.Msg)
  {
      case LMSG_AttackTo:
      {
          if((pBarrack->getActualHealth() - pBarrack->calculateDamage(msg.Sender, pBarrack->getTime()))>0)
          {
            pBarrack->setActualHealth(pBarrack->getActualHealth() - pBarrack->calculateDamage(msg.Sender, pBarrack->getTime()));
          }
          else
            pBarrack->setActualHealth(0.0f);
          break;
      }
      return true;
  }
  return false;
}

DecreaseHealthBarrack* DecreaseHealthBarrack::Instance()
{
  static DecreaseHealthBarrack instance;
  return &instance;
}

bool DecreaseHealthBarrack::Enter(AIBarrack* pBarrack)
{  
return true;
}

bool DecreaseHealthBarrack::Execute(AIBarrack* pBarrack)
{
return true;
}

bool DecreaseHealthBarrack::Exit(AIBarrack* pBarrack)
{
return true;
}

bool DecreaseHealthBarrack::OnMessage(AIBarrack* pBarrack, const Telegram& msg)
{
    return false;
}

IncreaseHealthBarrack* IncreaseHealthBarrack::Instance()
{
    static IncreaseHealthBarrack instance;
    return &instance;
}

bool IncreaseHealthBarrack::Enter(AIBarrack* pBarrack)
{
return true;
}

bool IncreaseHealthBarrack::Execute(AIBarrack* pBarrack)
{ 
return true;
}

bool IncreaseHealthBarrack::Exit(AIBarrack* pBarrack)
{ 
return true;
}

bool IncreaseHealthBarrack::OnMessage(AIBarrack* pBarrack, const Telegram& msg)
{
    return false;
}


DieBarrack* DieBarrack::Instance()
{
  static DieBarrack instance;
  return &instance;
}

bool DieBarrack::Enter(AIBarrack* pBarrack)
{
    return true;
}

bool DieBarrack::Execute(AIBarrack* pBarrack)
{ 
    pBarrack->GetFSM()->ChangeState(DieBarrack::Instance()); 
    return true;
}

bool DieBarrack::Exit(AIBarrack* pBarrack)
{ 
    for(int i=0; (unsigned int)i<EntityCityManager::getSingletonPtr()->getObjetivesFrom(pBarrack).size(); i++)
    {
        MessageSystem::getSingletonPtr()->DispatchMessageEntity(SEND_MSG_IMMEDIATELY,
                            pBarrack->ID(),
                            EntityCityManager::getSingletonPtr()->GetEntity(EntityCityManager::getSingletonPtr()->getObjetivesFrom(pBarrack)[i]->ID())->ID(),
                            LMSG_ImDying);  
    }

    pBarrack->die(pBarrack);
    return true;
}

bool DieBarrack::OnMessage(AIBarrack* pBarrack, const Telegram& msg)
{
    return false;
}