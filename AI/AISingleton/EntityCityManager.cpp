#include "EntityCityManager.h"

template<> EntityCityManager* Singleton<EntityCityManager>::msSingleton = 0;

void EntityCityManager::Add(AISystem* entity)
{
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i) == entity) return; //para ver si metemos dos veces el mismo objeto
		i++;
	}
	newEntities.push_back(entity);

    return;
}

void EntityCityManager::Remove( AISystem* entity )
{
	garbage.push_back(entity);
}

void EntityCityManager::Remove( int ID )
{
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->ID() == ID)
		{
			garbage.push_back(*i);
			//solo podrá haber una entidad con ese id, por eso se puede meter return
			return;
		}
		i++;
	}
}

void EntityCityManager::RemoveType( int Type )
{
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
        if( (*i)->getEntityType() == Type)
		{
			garbage.push_back(*i);	
		}
		i++;
	}
}

void EntityCityManager::RemoveAll()
{
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		delete ((*i));
		i++;
	}
	entitiesList.clear();
}

AISystem* EntityCityManager::GetEntity( int ID )
{
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->ID() == ID) return (*i);
		i++;
	}
	return NULL;
}

std::vector<AISystem*>& EntityCityManager::GetAttackMode(bool AttackMode)
{
	static std::vector<AISystem*> FindEntity;

	FindEntity.clear();

	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->IsAttacking() == AttackMode) FindEntity.push_back( *i );
		i++;
	}

	return FindEntity;
}

std::vector<AISystem*>& EntityCityManager::GetType( int Type )
{
	static std::vector<AISystem*> FindEntity;

	FindEntity.clear();

	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
		if( (*i)->getEntityType() == Type) FindEntity.push_back( *i );
		i++;
	}

	return FindEntity;
}

void EntityCityManager::Update( float delta )
{
	//Add new entities
	for(unsigned int idx=0;idx<newEntities.size();idx++)
    {
	    entitiesList.push_back( newEntities[idx] );
	}

	newEntities.clear();

	//Del garbageEntities
	for(unsigned idx=0;idx<garbage.size();idx++)
	{	
        delete garbage[idx];
	    entitiesList.remove( garbage[idx] );
    }
	garbage.clear();

	std::list<AISystem*>::iterator i = entitiesList.begin();
	while(i != entitiesList.end())
	{
		(*i)->Update(delta);
		i++;
	}
}

void EntityCityManager::Enter()
{
	//Add new entities
	for(unsigned int idx=0;idx<newEntities.size();idx++)
    {	
	    entitiesList.push_back( newEntities[idx] );	
	}
	newEntities.clear();

	//Del garbageEntities
	for(unsigned idx=0;idx<garbage.size();idx++)
	{
	    delete garbage[idx];	
	    entitiesList.remove(garbage[idx]);
    }
	garbage.clear();
}


std::vector<AISystem*> EntityCityManager::getObjetivesFrom(AISystem* m_pEntity)
{
   	static std::vector<AISystem*> FindEntity;
	FindEntity.clear();
	std::list<AISystem*>::iterator i = entitiesList.begin();
	while( i != entitiesList.end() )
	{
        if(((AIUnit*)*i)->getObjetive() == m_pEntity) FindEntity.push_back(*i);
		i++;
	}
	return FindEntity; 
}