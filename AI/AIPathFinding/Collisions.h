#ifndef COLLISIONS_
#define COLLISIONS_

#include "Ogre.h"
#include "AIGame.h"
#include "KOKDefine.h"
#include "EntityCityManager.h"

using namespace Ogre;

class Collisions
{

public:

    Collisions()
    {
        m_pEntityMovil = 0;
        m_pEntityStatic = 0;
    };

    ~Collisions(){};

    Real getWeightRadius(){return rect.weightRadius;}
    Real getHeightRadius(){return rect.heightRadius / 2;}

    // void   setEntityMovil(AIMovil* movil){m_pEntityMovil = movil;};
    // void   setEntityStatic(AIGame* sta){m_pEntityStatic = sta;};

    // Según el tipo de colision de entity2 se llamará a uno u otro método.
    // será el segundo parámetro de la creación de Collisions, entity2. entity1 siempre será circular.
    // Será mejor que se pase como parámetro de evaluate. Paso la entidad 1 y la entidad 2
    bool evaluate(AIMovil* entity1, AIGame* entity2)
    {
        // setEntityMovil(entity1);
        // setEntityStatic(entity2);

        switch (entity2->getCollisionType())
        {
            case square:
            {
                return CollisionBetweenCircleAndSquare(entity1, entity2);      
            } 

            case circle1:
            {
                return CollisionBetweenCircleAndCircle(entity1, entity2); 
            } 

            case rectangle:
            {
                 return CollisionBetweenCircleAndRectangle(entity1, entity2);
            } 
        }
        return false;
    }

    // Según el nombre de la entidad
    bool CollisionBetweenCircleAndCircle(AIMovil* entity1, AIGame* entity2)
    {

        // colisiones entre dos círculos.
        Vector2 mVelocity = entity1->velocity();
        Vector2 mDistanceV = entity2->getPosition() - entity1->getPosition();
        double mDistance = mDistanceV.length();
        double mMovilRadius = entity1->getRadius();
        double mStaticRadius = entity2->getRadius();
        Vector2 mVelocityNor = mVelocity.normalisedCopy();
        double sumRadius = mMovilRadius + mStaticRadius;
        double sumRadiusSqr = sumRadius * sumRadius;

        mDistance -= sumRadius;
        if(mVelocity.length() < mDistance){
            return false;
        }

        double d = mVelocity.normalisedCopy().dotProduct(mDistanceV);
        if(d <= 0)
            return false;
        
        double f = (mDistance * mDistance) - (d * d);
        if(f >= sumRadiusSqr){
            return false;
        }

        double t = sumRadiusSqr - f;
        if(t < 0)
            return false;

        double distance = d - sqrt(t);
        double mVelocityLength = mVelocity.length();
        if(mVelocityLength < distance)
            return false;

        // Set the length of the movevec so that the circles will just
        // movevec.normalize();
        // movevec.times(distance);

        return true;  

    }

    bool CollisionBetweenCircleAndSquare(AIMovil* entity1, AIGame* entity2)
    {
        double mMovilRadius = entity1->getRadius();
        double mStaticRadius = entity2->getRadius();
        Vector2 mCirclePos = entity1->getPosition();
        Vector2 mSquarePos = entity2->getPosition();

        bool a = mCirclePos.x < (mSquarePos.x - mMovilRadius - mStaticRadius);
        bool b = mCirclePos.x > (mSquarePos.x + mMovilRadius + mStaticRadius);
        bool c = mCirclePos.y > (mSquarePos.y + mMovilRadius + mStaticRadius);
        bool d = mCirclePos.y < (mSquarePos.y - mMovilRadius - mStaticRadius);

        if((mCirclePos.x < (mSquarePos.x - mMovilRadius - mStaticRadius))||
            (mCirclePos.x > (mSquarePos.x + mMovilRadius + mStaticRadius))||
            (mCirclePos.y > (mSquarePos.y + mMovilRadius + mStaticRadius))||
            (mCirclePos.y < (mSquarePos.y - mMovilRadius - mStaticRadius)))
            return false;

        return true;

    };

    bool CollisionBetweenCircleAndRectangle(AIMovil* entity1, AIGame* entity2)
    {
        // colisiones entre un circulo y un rectángulo
        //Vector2 mVelocity = m_pEntityMovil->velocity();
        //Vector2 mDistanceV = m_pEntityStatic->getPosition() - m_pEntityMovil->getPosition();
        //double mDistance = mDistanceV.length();
        double mMovilRadius = entity1->getRadius();
        double mStaticRadius = entity2->getRadius();

        //Vector2 mVelocityNor = mVelocity.normalisedCopy();
        double sumRadius = mMovilRadius + mStaticRadius;
        // double sumRadiusSqr = sumRadius * sumRadius;  
        return false;
    };

protected:

    struct Rect {
       Real weightRadius; 
       Real heightRadius;  
    }rect;

    struct Circle
    {
        Real Radius;
        Real Circunference;
    }circle;

private:
    
    AIMovil*    m_pEntityMovil;
    AIGame*     m_pEntityStatic;

};

#endif COLLISIONS_