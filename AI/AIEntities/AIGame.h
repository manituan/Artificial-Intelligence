#ifndef AIGAME_
#define AIGAME_

#include"AISystem.h"

class Collisions;

class AIGame: public AISystem
{
public:

	AIGame():AISystem(AISystem::getNextValidID()) 
    {
        m_dActualHealth = 0;
        m_dTotalHealth = 0;
        m_iRace = 0;
        m_pCollisions = 0;
        m_iVision = 0;
        m_iAttackDistance = 0;
        m_iAttack = 0;
        m_defense = 0;
        m_dps = 0;
    }

    //virtual void    Update(double time_elapsed) = 0; 

    virtual bool    HandleMessage(const Telegram& msg){return false;}

    double          getActualHealth()const{return m_dActualHealth;}
    void            setActualHealth(double health){m_dActualHealth = health;}

    double          getTotalHealth()const{return m_dTotalHealth;}
    void            setTotalHealth(double health){m_dTotalHealth = health;}

    Collisions*     getCollisions()const{return m_pCollisions;}

    int             race()const{return m_iRace;}
    int             setRace(int newRace){m_iRace = newRace;}

    float           getAttack()const{return m_iAttack;}
    void            setAttack(float attack){m_iAttack = attack;}

    float           getDefense()const{return m_defense;}
    void            setDefense(float defense){m_defense = defense;}

    float           getDps()const{return m_dps;}
    void            setDps(float dps){m_dps = dps;}

    float calculateDamage(int IDsender, float delta);

    int                     getVision(){return m_iVision;}
    AIGame*                 getObjetive (){return m_pObjetive;}
    void                    setObjetive(AIGame* unit){m_pObjetive = unit;}

bool attackRange(AISystem* pObjetive);


void die(AISystem* unit);

protected:

    double          m_dActualHealth;
    double          m_dTotalHealth;
    int             m_iRace;
    Collisions*     m_pCollisions;
    float                   m_iAttack;
    float                   m_defense;
    float                   m_dps;
    int                     m_iVision;
    int                     m_iAttackDistance;
    AIGame*                 m_pObjetive;



private:
    
};
#endif
