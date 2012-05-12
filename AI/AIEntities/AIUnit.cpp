#include "AIUnit.h"

float AIGame::calculateDamage(int IDsender, float delta)
{
    float aux = ((AIUnit*)(EntityCityManager::getSingletonPtr()->GetEntity(IDsender)))->getAttack() * delta;
    return aux;
}

void AIUnit::translateEntity(double timeElapsed)
{
    
    std::vector<AISystem*> lEntityVector = EntityCityManager::getSingletonPtr()->getEntityVector();  
    EntityCityManager::getSingletonPtr()->tagEntity((AISystem*)this);
    
    if(m_bStop == false)
    {
/*
        if(this->surrounded(this))
        {
            Vector2 aux = this->rotate90Entity(this->getOrientation());
            this->setPos(this->getPosition() + aux);
            m_vOri = this->getOrientation().normalisedCopy();

            
            m_vSteeringForce += m_pSteering->Separation(lEntityVector) * 10;   
            // m_vSteeringForce += m_pSteering->ObstacleAvoidance(EntityCityManager::getSingletonPtr()->getEntityVector());

            Vector2 acceleration = m_vSteeringForce / m_dMass;
            m_vVelocity += acceleration * timeElapsed;

            if (abs(m_vVelocity.length()) > m_MaxSpeed)
            {
                m_vVelocity.normalise();
                m_vVelocity *= m_MaxSpeed;
            }

            m_vPrevPos = m_vPos;
            m_vPos += m_vVelocity * timeElapsed;
            m_vOri = m_vVelocity.normalisedCopy();
            
            // m_vSteeringForce += m_pSteering->Separation(lEntityVector) * 10; 

        }else{*/

            m_vSteeringForce += m_pSteering->Separation(lEntityVector) * 10;   
            // m_vSteeringForce += m_pSteering->ObstacleAvoidance(EntityCityManager::getSingletonPtr()->getEntityVector());

            Vector2 acceleration = m_vSteeringForce / m_dMass;
            m_vVelocity += acceleration * timeElapsed;

            if (abs(m_vVelocity.length()) > m_MaxSpeed)
            {
                m_vVelocity.normalise();
                m_vVelocity *= m_MaxSpeed;
            }

            m_vPrevPos = m_vPos;
            m_vPos += m_vVelocity * timeElapsed;
            // m_vOri = m_vVelocity.normalisedCopy();

        //}
            m_vOri = this->getOrientation().normalisedCopy();
            // m_vOri = m_vPos.normalisedCopy();

    }else
        m_vPos += 0.0;

}

void AIUnit::separateEntity(double timeElapsed)
{

}

bool AIUnit::seeEnemy(bool attackMode)
{
    // tiene que evaluar todas las distancias de todas las entidades que ve en el rango y pillar la más cercana, no la
    // primera que entre en el vector.
    m_pObjetive = NULL;
    std::vector<AISystem*> enemies = EntityCityManager::getSingletonPtr()->GetAttackMode(!attackMode);
    double minDistance = 100000.0;
    for(int i=0; (unsigned int)i<enemies.size(); i++)
    {
        Ogre::Vector2 aux = enemies[i]->getPosition();
        if((aux.distance(this->getPosition()) < this->getVision()) && (aux.distance(this->getPosition()) < minDistance) )
        {
            minDistance = aux.distance(this->getPosition());
            m_pObjetive = (AIUnit*)enemies[i];
        }
    }

    if(m_pObjetive == NULL)
        return false;
    else
        return true;
}

bool AIUnit::seeEnemy2(bool attackMode)
{
    std::vector<AISystem*> enemies = EntityCityManager::getSingletonPtr()->GetAttackMode(!attackMode);

    for(int i=0; (unsigned int)i<enemies.size(); i++)
    {
        Ogre::Vector2 aux = enemies[i]->getPosition();
        if((aux.distance(this->getPosition()) < this->getVision()))
            return true;
    }
    return false;
}

bool AIUnit::attackRange(AISystem* pObjetive)
{
    if(pObjetive != NULL)
    {
        Ogre::Vector2 aux = pObjetive->getPosition();
        if(aux.distance(this->getPosition()) < m_iAttackDistance)
        {
            //m_pObjetive = (AIUnit*)pObjetive;
            return true;
        }
    }
    return false;
}

void AIGame::die(AISystem* unit)
{
    EntityCityManager::getSingletonPtr()->Remove(unit->ID());
}

bool AIUnit::surrounded(AISystem* m_pEntity)
{
    static std::vector<AISystem*> FindEntity;
	FindEntity.clear();

    std::vector<AISystem*> entities = EntityCityManager::getSingletonPtr()->getEntityVector();
    for(int i=0; (unsigned int)i<entities.size(); i++)
    {
        Ogre::Vector2 aux = m_pEntity->getPosition() - entities[i]->getPosition();
        Real angleBetween = aux.normalisedCopy().dotProduct(m_pEntity->getOrientation().normalisedCopy());
        // float aux2 = angleBetween.valueDegrees();
        if ((entities[i] != m_pEntity) && (aux.length() < m_pEntity->getRadius() * 7) && (entities[i]->IsAttacking() == m_pEntity->IsAttacking()) && (angleBetween < cos(90.0)))
        {
            FindEntity.push_back(entities[i]);
        }
    }

    if(FindEntity.size() > 2)
    {
        return true;
    }

	return false; 
}

Ogre::Vector2 AIUnit::rotate90Entity(Vector2 mVector)
{
    mVector.perpendicular();
    return mVector;
}