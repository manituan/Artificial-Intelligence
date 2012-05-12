#ifndef AIUNIT_H_
#define AIUNIT_H_

#include "AIMovil.h"
#include "EntityCityManager.h"
#include "KOKDefine.h"
#include <math.h>
#include "Collisions.h"

class AIUnit : public AIMovil
{
protected:


    int                     m_iAnimation;
    bool                    m_bStop;

public:

    AIUnit()
    {
        m_bStop = false;
        m_pObjetive = 0;
        m_iAnimation = 0;


    };

    ~AIUnit(){};


    bool                    getStop(){return m_bStop;}
    void                    setStop(bool mov){m_bStop = mov;}
    int                     getAnimation(){return m_iAnimation;}
    void                    setAnimation(int ani){m_iAnimation = ani;}
    SteeringBehavior*       getSteering(){return m_pSteering;}

	void Update(double time_elapsed) {};
    virtual bool HandleMessage(const Telegram& msg){return false;}

    void translateEntity(double timeElapsed);

    bool seeEnemy(bool attackMode);

    bool seeEnemy2(bool attackMode);

    bool attackRange(AISystem* pObjetive);

    void separateEntity(double timeElapsed);

    bool surrounded(AISystem* m_pEntity);

    double angle(const Vector2 &U, const Vector2 &V)
    {
        return atan2(Vector2(-U.y, U.x).dotProduct(V), U.dotProduct(V));
    }

    Ogre::Vector2 rotate90Entity(Vector2 mVector);

};

#endif