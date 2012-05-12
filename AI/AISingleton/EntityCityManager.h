#ifndef ENTITYCITYMANAGER_H
#define ENTITYCITYMANAGER_H
#pragma warning (disable:4786)

#include <map>
#include <cassert>
#include <string>
#include "Ogre.h"
#include "AISystem.h"
#include "AIUnit.h"
class AISystem;

using namespace Ogre;

class EntityCityManager: public Singleton<EntityCityManager>
{
private:

  //typedef std::map<int, AISystem*> EntityMap;

private:
  
  //to facilitate quick lookup the entities are stored in a std::map, in which
  //pointers to entities are cross referenced by their identifying number
  //EntityMap m_EntityMap;
  std::list<AISystem*> entitiesList;
  std::vector<AISystem*> garbage;
  std::vector<AISystem*> newEntities;
  std::vector<AISystem*> m_vObj;
  std::list<AISystem*> playerList;

  //copy ctor and assignment should be private
  EntityCityManager(const EntityCityManager&);
  EntityCityManager& operator=(const EntityCityManager&);

public:

    EntityCityManager(){}
    ~EntityCityManager(){}

    void Update(float delta);

    void Add(AISystem* entity);
    void Remove(AISystem* entity);
    void Remove(int ID);
    void RemoveType(int Type);
    std::vector<AISystem*>& GetAttackMode(bool AttackMode);
    //void RemovePlayer(int Player);
    void RemoveAll();
    void Enter();
    bool surrounded(AISystem* m_pEntity);

    void tagEntity(AISystem* entity)
    {
        std::vector<AISystem*> entities = EntityCityManager::getSingletonPtr()->getEntityVector();
        for(int i=0; (unsigned int)i<entities.size(); i++)
        {
            entities[i]->UnTag();
            Ogre::Vector2 aux = entities[i]->getPosition() - entity->getPosition();

            double range = entities[i]->getRadius();
            if ( (entities[i] != entity) && (aux.length() < range * range))
            {
                entities[i]->Tag();
            }
        }
    }

    std::vector<AISystem*> getObjetivesFrom(AISystem* m_pEntity);

    AISystem* GetEntity(int ID);
    //AbIsAttacking* GetPlayer(int IDPlayer);

    std::vector<AISystem*>& GetType( int Type );

    std::vector<AISystem*>& getEntityVector()
    {
	    static std::vector<AISystem*> FindEntity;

	    FindEntity.clear();

	    std::list<AISystem*>::iterator i = entitiesList.begin();
	    while(i != entitiesList.end())
	    {
		    FindEntity.push_back(*i);
		    i++;
	    }

	    return FindEntity;
    }

    //EntityMap getEntityMap()const{return m_EntityMap;}

    //this method stores a pointer to the entity in the std::vector
    //m_Entities at the index position indicated by the entity's ID
    //(makes for faster access)
    //void            RegisterEntity(AISystem* NewEntity);

    //std::vector<AISystem*> GetEntityType(int Type);

    //returns a pointer to the entity with the ID given as a parameter
    //AISystem*       GetEntityFromID(int id)const;

    //this method removes the entity from the list
    //void            RemoveEntity(AISystem* pEntity);

    /*
    void            Remove(int ID);
    void            RemoveType(int Type);
    */
    //clears all entities from the entity map
    //void Reset(){entitiesList.clear();}
};

#endif