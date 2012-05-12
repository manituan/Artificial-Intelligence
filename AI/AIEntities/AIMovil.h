#ifndef _MOVIL_AI_H_
#define _MOVIL_AI_H_

#include "AIGame.h"
#include <cassert>
#include "SteeringBehavior.h"

class AIMovil: public AIGame
{
public:

	AIMovil():AIGame()
	{
        m_MaxSpeed = 0;		
        m_MaxForce = 0;
        m_vVelocity = 0;
        m_vHeading = 0;
        m_vSide = 0;
        m_pSteering = 0;
        m_vSteeringForce = 0;
        m_dMass = 0;

        m_pSteering = new SteeringBehavior(this);
    } 

	//accessors
	Vector2	velocity()const						
    {
        return m_vVelocity;
    }
	void setVelocity(const Vector2& NewVel)	
	{
		m_vVelocity = NewVel;
		//m_vVelocity.Truncate(MaxSpeed());
	}

	Vector2	side()const	
    {
        return m_vSide;
    }

	double speed()const		
    {
        return m_vVelocity.length();
    }

	Vector2 heading()const					
    {
        return m_vHeading;
    }

	void setHeading(Vector2 new_heading);

	double maxSpeed()const			
    {
        return m_MaxSpeed;
    }

	double maxForce()const
    {
        return m_MaxForce;
    }

protected:
    
    //the maximum speed this entity may travel at.
	double	                m_MaxSpeed;		
	double	                m_MaxForce;

	//its location in the environment
	Vector2	                m_vVelocity;

	//a normalized vector pointing in the direction the entity is heading. 
	Vector2	                m_vHeading;

	//a vector perpendicular to the heading vector
	Vector2                 m_vSide;

    SteeringBehavior*       m_pSteering;

    Vector2                 m_vSteeringForce;

    Real                    m_dMass;

/*
//------------------------- SetHeading ----------------------------------------
//
//  first checks that the given heading is not a vector of zero length. If the
//  new heading is valid this fumction sets the entity's heading and side 
//  vectors accordingly
//-----------------------------------------------------------------------------

inline void setHeading(Vector2 new_heading)
{
  assert( (new_heading.length() - 1.0) < 0.00001);
  
  m_vHeading = new_heading;

  //the side vector must always be perpendicular to the heading
  m_vSide = m_vHeading.perpendicular();
}
*/
};



#endif