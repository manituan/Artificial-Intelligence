#ifndef AIMOVABLE_H_
#define AIMOVABLE_H_

#include "AIGame.h"
#include <cassert>

#include "AbstractVehicle.h"
#include "SteerLibrary.h"
#include "Annotation.h"
#include "Ogre.h"

namespace OpenSteer
{

// SimpleVehicle_1 adds concrete LocalSpace methods to AbstractVehicle
typedef LocalSpaceMixin<AbstractVehicle> Movable_1;

// SimpleVehicle_2 adds concrete annotation methods to SimpleVehicle_1
typedef AnnotationMixin<Movable_1> Movable_2;

// SimpleVehicle_3 adds concrete steering methods to SimpleVehicle_2
typedef SteerLibraryMixin<Movable_2> Movable_3;

class AIMovable : public AIGame, Movable_3
{

protected:
  
    double      m_dViewRadius;

    float       m_fMass;       // mass (defaults to unity so acceleration=force)

    float       m_fBRadius;     // size of bounding sphere, for obstacle avoidance, etc.

    float       m_fSpeed;      // speed along Forward direction.  Because local space
                        // is velocity-aligned, velocity = Forward * Speed

    float       m_fMaxForce;   // the maximum steering force this vehicle can apply
                        // (steering force is clipped to this magnitude)

    float       m_fMaxSpeed;   // the maximum speed this vehicle is allowed to move
                        // (velocity is clipped to this magnitude)

    float       m_fCurvature;
    Vec3        m_vLastForward;
    Vec3        m_vLastPosition;
    Vec3        m_vSmoothedPosition;
    float       m_fSmoothedCurvature;
    Vec3        m_vSmoothedAcceleration;

    // measure path curvature (1/turning-radius), maintain smoothed version
    void measurePathCurvature (const float elapsedTime);

public:

	AIMovable(){reset();}

    //so must this
    virtual bool    HandleMessage(const Telegram& msg){return false;}

    virtual         ~AIMovable();

    //virtual void    Update(double time_elapsed) = 0;  

    void reset (void)
    {
        // reset LocalSpace state
        resetLocalSpace ();

        // reset SteerLibraryMixin state
        // (XXX this seems really fragile, needs to be redesigned XXX)
        Movable_3::reset ();

        setMass (1);          // mass (defaults to 1 so acceleration=force)
        setSpeed (0);         // speed along Forward direction.

        setRadius (0.5f);     // size of bounding sphere

        setMaxForce (0.1f);   // steering force is clipped to this magnitude
        setMaxSpeed (1.0f);   // velocity is clipped to this magnitude

        // reset bookkeeping to do running averages of these quanities
        resetSmoothedPosition ();
        resetSmoothedCurvature ();
        resetSmoothedAcceleration ();
    }

    OpenSteer::Vec3 wander(float f)
    {
        OpenSteer::Vec3 aux = this->steerForWander(f);
        return aux;
    }

    OpenSteer::Vec3 seek(AISystem* entity)
    {
        OpenSteer::Vec3 aux = this->steerForSeek(OpenSteer::Vec3(entity->getPosition().x, 0, entity->getPosition().y));
        const Vec3 desiredVelocity = OpenSteer::Vec3(entity->getPosition().x, 0, entity->getPosition().y) - position();
        return desiredVelocity - velocity();
        return aux;
    }

    OpenSteer::Vec3 orientation()
    {
        OpenSteer::Vec3 aux = this->forward();
        return aux;
    }

    // get/set mass
    float mass (void) const {return m_fMass;}
    float setMass (float m) {return m_fMass = m;}

    // get velocity of vehicle
    Vec3 velocity (void) const {return forward() * m_fSpeed;}

    // get/set speed of vehicle  (may be faster than taking mag of velocity)
    float speed (void) const {return m_fSpeed;}
    float setSpeed (float s) {return m_fSpeed = s;}

    // size of bounding sphere, for obstacle avoidance, etc.
    float radius (void) const {return m_fBRadius;}
    float setRadius (float m) {return m_fBRadius = m;}

    // get/set maxForce
    float maxForce (void) const {return m_fMaxForce;}
    float setMaxForce (float mf) {return m_fMaxForce = mf;}

    // get/set maxSpeed
    float maxSpeed (void) const {return m_fMaxSpeed;}
    float setMaxSpeed (float ms) {return m_fMaxSpeed = ms;}

    // apply a given steering force to our momentum,
    // adjusting our orientation to maintain velocity-alignment.
    void applySteeringForce (const Vec3& force, const float deltaTime);

    // the default version: keep FORWARD parallel to velocity, change
    // UP as little as possible.
    virtual void regenerateLocalSpace (const Vec3& newVelocity,
                                        const float elapsedTime);

    // alternate version: keep FORWARD parallel to velocity, adjust UP
    // according to a no-basis-in-reality "banking" behavior, something
    // like what birds and airplanes do.  (XXX experimental cwr 6-5-03)
    void regenerateLocalSpaceForBanking (const Vec3& newVelocity,
                                            const float elapsedTime);

    // adjust the steering force passed to applySteeringForce.
    // allows a specific vehicle class to redefine this adjustment.
    // default is to disallow backward-facing steering at low speed.
    // xxx experimental 8-20-02
    virtual Vec3 adjustRawSteeringForce (const Vec3& force,
                                            const float deltaTime);

    // apply a given braking force (for a given dt) to our momentum.
    // xxx experimental 9-6-02
    void applyBrakingForce (const float rate, const float deltaTime);

    // predict position of this vehicle at some time in the future
    // (assumes velocity remains constant)
    Vec3 predictFuturePosition (const float predictionTime) const;

    // get instantaneous curvature (since last update)
    float curvature (void) {return m_fCurvature;}

    // get/reset smoothedCurvature, smoothedAcceleration and smoothedPosition
    float smoothedCurvature (void) {return m_fSmoothedCurvature;}
    float resetSmoothedCurvature (float value = 0)
    {
        m_vLastForward = Vec3::zero;
        m_vLastPosition = Vec3::zero;
        return m_fSmoothedCurvature = m_fCurvature = value;
    }
        Vec3 smoothedAcceleration (void) {return m_vSmoothedAcceleration;}
        Vec3 resetSmoothedAcceleration (const Vec3& value = Vec3::zero)
    {
        return m_vSmoothedAcceleration = value;
    }
        Vec3 smoothedPosition (void) {return m_vSmoothedPosition;}
        Vec3 resetSmoothedPosition (const Vec3& value = Vec3::zero)
    {
        return m_vSmoothedPosition = value;
    }

    // give each vehicle a unique number
    int serialNumber;
    static int serialNumberCounter;

    // draw lines from vehicle's position showing its velocity and acceleration
    void annotationVelocityAcceleration (float maxLengthA, float maxLengthV);
    void annotationVelocityAcceleration (float maxLength)
        {annotationVelocityAcceleration (maxLength, maxLength);}
    void annotationVelocityAcceleration (void)
        {annotationVelocityAcceleration (3, 3);}

    // set a random "2D" heading: set local Up to global Y, then effectively
    // rotate about it by a random angle (pick random forward, derive side).
    void randomizeHeadingOnXZPlane (void)
    {
        setUp (Vec3::up);
        setForward (RandomUnitVectorOnXZPlane ());
        setSide (localRotateForwardToSide (forward()));
    }  
  
    double		    viewRadius()const{return m_dViewRadius;}
    void		    setViewRadius(double newViewR){m_dViewRadius = newViewR;}
    /*
    //Graphics side
    void orientation(Vector2 goalDir)
    {			//Orientation
        Vector2 actualDir = goalDir - m_vPos;			
        Ogre::Vector3 src = m_vOri * Ogre::Vector3::UNIT_X;
        if ((1.0f + src.dotProduct(actualDir)) < 0.0001f) 
        {
	        m_Node->yaw(Ogre::Degree(180));
        }
        else
        {
	        Ogre::Quaternion quat = src.getRotationTo(m_Direction);
	        m_Node->rotate(quat + 90.0f);
        } // else
    }
    */
};

}
#endif