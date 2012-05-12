#include "FactoryGraphics.h"

template<> FactoryGraphics* Singleton<FactoryGraphics>::msSingleton = 0;

FactoryGraphics::FactoryGraphics()
{

}

FactoryGraphics::~FactoryGraphics()
{

}
bool FactoryGraphics::HandleMessage(const Telegram& msg)
{
    if(msg.Msg == SMSG_AICreateGraphUnit)
    {
        newGraphUnit(msg.Sender, msg.iType, msg.iRace, Vector2 (msg.Position.x,msg.Position.y), Vector2(msg.Orientation.x,msg.Orientation.y), Vector2(msg.dActualHealth, msg.dTotalHealth));      
        return true;
    }

    if(msg.Msg == SMSG_AICreateGraphBuilding)
    {
        newGraphBuilding(msg.Sender, msg.iType, msg.iRace, Vector2 (msg.Position.x,msg.Position.y), Vector2(msg.Orientation.x,msg.Orientation.y), Vector2(msg.dActualHealth, msg.dTotalHealth));   
        return true;        
    }

    if(msg.Msg == SMSG_AIDeleteGraphSystem)
    {
        RemoveGraph(FactoryGraphics::getSingletonPtr()->GetGraphFromID(msg.Sender));   
        return true;        
    }

    return true;
}

void FactoryGraphics::newGraphBuilding(int IDsender, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth)
{
    switch (iType)
    {
    case HALL:
    {
        m_pGraph = new GraphHall(IDsender, iType, iRace, vPos, vRot, vHealth);
        FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
    break;
    }
    case BARRACK:
    {
        m_pGraph = new GraphBarrack(IDsender, iType, iRace, vPos, vRot, vHealth);
        FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
    break;
    }
    case WALLS:
    {
        m_pGraph = new GraphWalls(IDsender, iType, iRace, vPos, vRot, vHealth);
        FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
    break;
    }
  }
}

void FactoryGraphics::newGraphUnit(int IDsender, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot, Ogre::Vector2 vHealth)
{
  
  switch (iType)
  {
  case MELEE:
    {
        m_pGraph = new GraphMelee(IDsender, iType, iRace, vPos, vRot, vHealth);
        FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
        break;
    }
 
  case RANGED:
    {
       m_pGraph = new GraphRanged(IDsender, iType, iRace, vPos, vRot, vHealth);
       FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
       break;
    }

  case RIDER:
    {
       m_pGraph = new GraphRider(IDsender, iType, iRace, vPos, vRot, vHealth);
       FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
       break;
    }

  case MONSTER:
    {
       m_pGraph = new GraphMonster(IDsender, iType, iRace, vPos, vRot, vHealth);
       FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
       break;
    }
   case ELITE:
    {
       m_pGraph = new GraphElite(IDsender, iType, iRace, vPos, vRot, vHealth);
       FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
       break;
    }
   
   case WIZARD:
    {
       m_pGraph = new GraphWizard(IDsender, iType, iRace, vPos, vRot, vHealth);
       FactoryGraphics::getSingletonPtr()->RegisterGraphics(m_pGraph);
       break;
    }
  }
}

//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
GraphSystem* FactoryGraphics::GetGraphFromID(int id)const
{
  //find the entity
  GraphMap::const_iterator graphics = m_GraphMap.find(id);
  return graphics->second;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void FactoryGraphics::RemoveGraph(GraphSystem* pGraph)
{    
  m_GraphMap.erase(m_GraphMap.find(pGraph->ID()));
  delete pGraph;
} 

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void FactoryGraphics::RegisterGraphics(GraphSystem* NewGraph)
{
  m_GraphMap.insert(std::make_pair(NewGraph->ID(), NewGraph));
}

void FactoryGraphics::Update(float delta)
{
    for(int i=0; i<m_GraphMap.size(); i++)
    {
        m_GraphMap[i]->Update(delta);
    }
}
