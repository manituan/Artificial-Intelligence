#include "FactoryEntity.h"

template<> FactoryEntity* Singleton<FactoryEntity>::msSingleton = 0;

FactoryEntity::FactoryEntity()
{

}

FactoryEntity::~FactoryEntity()
{

}

AISystem* FactoryEntity::newAIBuilding(bool bIsAttacking, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot)
{
  switch (iType)
  {
  case HALL:
    { 
        m_pAISystem = new AIHall(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem; 
    }

  case BARRACK:
    {
        m_pAISystem = new AIBarrack(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem;     
    }

  case WALLS:
    {
        m_pAISystem = new AIWalls(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem;     
    }
  }
}

AISystem* FactoryEntity::newAIUnit(bool bIsAttacking, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot)
{
  switch (iType)
  {
  case MELEE:
    { 
        m_pAISystem = new AIMelee(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem; 
    }

  case RANGED:
    {
        m_pAISystem = new AIRanged(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem;     
    }
 
  case RIDER:
    {
        m_pAISystem = new AIRider(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem;     
    }

  case MONSTER:
    {
        m_pAISystem = new AIMonster(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem; 
    }

  case WIZARD:
    {
        m_pAISystem = new AIWizard(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem; 
    }

  case ELITE:
    {
        m_pAISystem = new AIElite(bIsAttacking, iType, iRace, vPos, vRot);
        EntityCityManager::getSingletonPtr()->Add(m_pAISystem);
        return m_pAISystem; 
    } 
  }
}