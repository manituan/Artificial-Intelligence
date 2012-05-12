#ifndef _FACTORY_ENTITY_H_
#define _FACTORY_ENTITY_H_

#include <map>
#include <vector>
#include "Ogre.h"

#include "AISystem.h"
#include "AIMelee.h"
#include "AIRanged.h"
#include "AIRider.h"
#include "AIWizard.h"
#include "AIMonster.h"
#include "AIWizard.h"
#include "AIElite.h"

#include "AIBuilding.h"
#include "AIBarrack.h"
#include "AIHall.h"
#include "AIWalls.h"

#include "KOKDefine.h"
#include "EntityCityManager.h"

using namespace Ogre;

//#include "Unit_Ogre_Definition.h"
//#include "OgreGame/UnitOgre.h"
//#include "OgreGame/BuildOgre.h"

class FactoryEntity: public Singleton<FactoryEntity>
{
public:
    
    FactoryEntity();
	~FactoryEntity();
    
    // create AISystem objects
    //AISystem* newAIUnit(int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot);
    AISystem* newAIUnit(bool bIsAttacking, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot);
    AISystem* newAIBuilding(bool bIsAttacking, int iType, int iRace, Ogre::Vector2 vPos, Ogre::Vector2 vRot);

protected:

private:
	FactoryEntity(const FactoryEntity&);
	FactoryEntity& operator= (const FactoryEntity&);

    AISystem*           m_pAISystem;

    AIMelee*            m_pAIMelee;
    AIRanged*           m_pAIRanged;
    AIRider*            m_pAIRider;
    AIMonster*          m_pAIMonster;
    AIElite*            m_pAIElite;
    AIWizard*           m_pAIWizard;

};

#endif