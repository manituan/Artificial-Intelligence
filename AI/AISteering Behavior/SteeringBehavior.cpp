#include "SteeringBehavior.h"
#include "AIMovil.h"
#include "AISystem.h"
#include <cassert>

SteeringBehavior::SteeringBehavior(AIMovil* agent):                                 
    m_pAIMovil(agent),
    m_iFlags(0),
    m_Feelers(3),
    m_Deceleration(normal),
    m_pTargetAgent1(NULL),
    m_pTargetAgent2(NULL),
    m_dWanderDistance(WanderDist),
    m_dWanderJitter(WanderJitterPerSec),
    m_dWanderRadius(WanderRad),
    m_dWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
    m_bCellSpaceOn(false),
    m_SummingMethod(prioritized)
{

}

//------------------------- ForwardComponent -----------------------------
//
//  returns the forward oomponent of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::ForwardComponent()
{
    return m_pAIMovil->heading().dotProduct(m_vSteeringForce);
}

//--------------------------- SideComponent ------------------------------
//  returns the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::SideComponent()
{
  return m_pAIMovil->side().dotProduct(m_vSteeringForce);
}


//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  AIMovil has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::AccumulateForce(Vector2 &RunningTot,
                                       Vector2 ForceToAdd)
{
  
  //calculate how much steering force the AIMovil has used so far
    double MagnitudeSoFar = RunningTot.length();

  //calculate how much steering force remains to be used by this AIMovil
  double MagnitudeRemaining = m_pAIMovil->maxForce() - MagnitudeSoFar;

  //return false if there is no more force left to use
  if (MagnitudeRemaining <= 0.0) return false;

  //calculate the magnitude of the force we want to add
  double MagnitudeToAdd = ForceToAdd.length();
  
  //if the magnitude of the sum of ForceToAdd and the running total
  //does not exceed the maximum force available to this AIMovil, just
  //add together. Otherwise add as much of the ForceToAdd vector is
  //possible without going over the max.
  if (MagnitudeToAdd < MagnitudeRemaining)
  {
    RunningTot += ForceToAdd;
  }

  else
  {
    //add it to the steering force
      RunningTot += (ForceToAdd.normalise()) * MagnitudeRemaining; 
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Seek(Vector2 TargetPos)
{
    Vector2 DesiredVelocity = TargetPos - m_pAIMovil->getPosition();
    DesiredVelocity = DesiredVelocity.normalisedCopy() * m_pAIMovil->maxSpeed();
    return (DesiredVelocity - m_pAIMovil->velocity());
}

//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Flee(Vector2 TargetPos)
{  
  /*
  //only flee if the target is within 'panic distance'. Work in distance
  //squared space.
  const double PanicDistanceSq = 1.0f * 1.0;
  if (Vec2DDistanceSq(m_pAIMovil->getPosition(), TargetPos) < PanicDistanceSq)
  {
    return Vector2(0,0);
  } 
  */
  Vector2 DesiredVelocity = m_pAIMovil->getPosition() - TargetPos;
  DesiredVelocity = DesiredVelocity.normalisedCopy() * m_pAIMovil->maxSpeed();
  return (DesiredVelocity - m_pAIMovil->velocity());
}

double SteeringBehavior::Vec2DDistanceSq(const Vector2 &v1, const Vector2 &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation * xSeparation;
}

Vector2 SteeringBehavior::Calculate()
{

  m_vSteeringForce.ZERO;
  
  // Si seek incremento la fuerza de steering
  if (On(seek))
  {
      m_vSteeringForce += Seek(m_pAIMovil->getPosition().normalisedCopy()) * m_dWeightSeek;
  }

  if (On(flee))
  {
      m_vSteeringForce += Flee(m_pAIMovil->getPosition().normalisedCopy()) * m_dWeightFlee;
  }

  return m_vSteeringForce;
}

//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Arrive(Vector2     TargetPos,
                                  Deceleration deceleration)
{
  Vector2 ToTarget = TargetPos - m_pAIMovil->getPosition();

  //calculate the distance to the target
  double dist = ToTarget.length();

  if (dist > 0)
  {
    //because Deceleration is enumerated as an int, this value is required
    //to provide fine tweaking of the deceleration..
    const double DecelerationTweaker = 0.3;

    //calculate the speed required to reach the target given the desired
    //deceleration
    double speed =  dist / ((double)deceleration * DecelerationTweaker);     

    //make sure the velocity does not exceed the max
    if(speed< m_pAIMovil->maxSpeed())
        speed = speed;
    else
        speed = m_pAIMovil->maxSpeed();

    //from here proceed just like Seek except we don't need to normalize 
    //the ToTarget vector because we have already gone to the trouble
    //of calculating its length: dist. 
    Vector2 DesiredVelocity =  ToTarget * speed / dist;

    return (DesiredVelocity - m_pAIMovil->velocity());
  }

  return Vector2(0,0);
}

//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Pursuit(const AIMovil* evader)
{
  //if the evader is ahead and facing the agent then we can just seek
  //for the evader's current position.
  Vector2 ToEvader = evader->getPosition() - m_pAIMovil->getPosition();

  double RelativeHeading = m_pAIMovil->heading().dotProduct(evader->heading());

  if ( (ToEvader.dotProduct(m_pAIMovil->heading()) > 0) &&  
       (RelativeHeading < -0.95))  //acos(0.95)=18 degs
  {
    return Seek(evader->getPosition());
  }

  //Not considered ahead so we predict where the evader will be.
 
  //the lookahead time is propotional to the distance between the evader
  //and the pursuer; and is inversely proportional to the sum of the
  //agent's velocities
  double LookAheadTime = ToEvader.length() / 
                        (m_pAIMovil->maxSpeed() + evader->speed());
  
  //now seek to the predicted future position of the evader
  return Seek(evader->getPosition() + evader->velocity() * LookAheadTime);
}


//----------------------------- Evade ------------------------------------
//
//  similar to pursuit except the agent Flees from the estimated future
//  position of the pursuer
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Evade(const AIMovil* pursuer)
{
  /* Not necessary to include the check for facing direction this time */

  Vector2 ToPursuer = pursuer->getPosition() - m_pAIMovil->getPosition();

  //uncomment the following two lines to have Evade only consider pursuers 
  //within a 'threat range'
  const double ThreatRange = 100.0;
  if (ToPursuer.length() > ThreatRange * ThreatRange) return Vector2();
 
  //the lookahead time is propotional to the distance between the pursuer
  //and the pursuer; and is inversely proportional to the sum of the
  //agents' velocities
  double LookAheadTime = ToPursuer.length() / 
                         (m_pAIMovil->maxSpeed() + pursuer->speed());
  
  //now flee away from predicted future position of the pursuer
  return Flee(pursuer->getPosition() + pursuer->velocity() * LookAheadTime);
}

/*
//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Wander()
{ 
  //this behavior is dependent on the update rate, so this line must
  //be included when using time independent framerate.
  double JitterThisTimeSlice = m_dWanderJitter * m_pAIMovil->TimeElapsed();

  //first, add a small random vector to the target's position
  m_vWanderTarget += Vector2(RandomClamped() * JitterThisTimeSlice,
                              RandomClamped() * JitterThisTimeSlice);

  //reproject this new vector back on to a unit circle
  m_vWanderTarget.Normalize();

  //increase the length of the vector to the same as the radius
  //of the wander circle
  m_vWanderTarget *= m_dWanderRadius;

  //move the target into a position WanderDist in front of the agent
  Vector2 target = m_vWanderTarget + Vector2(m_dWanderDistance, 0);

  //project the target into world space
  Vector2 Target = PointToWorldSpace(target,
                                       m_pAIMovil->Heading(),
                                       m_pAIMovil->Side(), 
                                       m_pAIMovil->Pos());

  //and steer towards it
  return Target - m_pAIMovil->Pos(); 
}
*/

//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
Vector2 SteeringBehavior::ObstacleAvoidance(const std::vector<AISystem*>& obstacles)
{
  //the detection box length is proportional to the agent's velocity
  m_dDBoxLength = 50.0 + ((m_pAIMovil->velocity().length()/m_pAIMovil->maxSpeed()) * 50.0);

  //this will keep track of the closest intersecting obstacle (CIB)
  AISystem* ClosestIntersectingObstacle = NULL;
 
  //this will be used to track the distance to the CIB
  double DistToClosestIP = 20.0; // ¿?¿?¿?¿?¿?¿

  //this will record the transformed local coordinates of the CIB
  Vector2 LocalPosOfClosestObstacle = Vector2(0.0, 0.0);
  
  std::vector<AISystem*>::const_iterator curOb = obstacles.begin();

  while(curOb != obstacles.end())
  {
    //if the obstacle has been tagged within range proceed
    if ((*curOb)->IsTagged())
    {
 
      //calculate this obstacle's position in local space
      Vector2 LocalPos = PointToLocalSpace((*curOb)->getPosition(),
                                                m_pAIMovil->heading(),
                                                m_pAIMovil->side(),
                                             m_pAIMovil->getPosition());

      //if the local position has a negative x value then it must lay
      //behind the agent. (in which case it can be ignored)
      if (LocalPos.x >= 0)
      {
        //if the distance from the x axis to the object's position is less
        //than its radius + half the width of the detection box then there
        //is a potential intersection.
          double ExpandedRadius = (*curOb)->getRadius() + m_pAIMovil->getRadius();

        if (fabs(LocalPos.y) < ExpandedRadius)
        {
          //now to do a line/circle intersection test. The center of the 
          //circle is represented by (cX, cY). The intersection points are 
          //given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
          //We only need to look at the smallest positive value of x because
          //that will be the closest point of intersection.
          double cX = LocalPos.x;
          double cY = LocalPos.y;
          
          //we only need to calculate the sqrt part of the above equation once
          double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

          double ip = cX - SqrtPart;

          if (ip <= 0.0)
          {
            ip = cX + SqrtPart;
          }

          //test to see if this is the closest so far. If it is keep a
          //record of the obstacle and its local coordinates
          if (ip < DistToClosestIP)
          {
            DistToClosestIP = ip;

            ClosestIntersectingObstacle = *curOb;

            LocalPosOfClosestObstacle = LocalPos;
          }         
        }
      }
    }

    ++curOb;
  }

  //if we have found an intersecting obstacle, calculate a steering 
  //force away from it
  Vector2 SteeringForce = Vector2(0.0, 0.0);

  if (ClosestIntersectingObstacle)
  {
    //the closer the agent is to an object, the stronger the 
    //steering force should be
    double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
                        m_dDBoxLength;

    //calculate the lateral force
    SteeringForce.y = (ClosestIntersectingObstacle->getRadius()-
                       LocalPosOfClosestObstacle.y)  * multiplier;   

    //apply a braking force proportional to the obstacles distance from
    //the AIMovil. 
    const double BrakingWeight = 0.2;

    SteeringForce.x = (ClosestIntersectingObstacle->getRadius() - 
                       LocalPosOfClosestObstacle.x) * 
                       BrakingWeight;
  }

  //finally, convert the steering vector from local to world space

  return VectorToWorldSpace(SteeringForce,
                            m_pAIMovil->heading(),
                            m_pAIMovil->side());
}

/*
//--------------------------- WallAvoidance --------------------------------
//
//  This returns a steering force that will keep the agent away from any
//  walls it may encounter
//------------------------------------------------------------------------
Vector2 SteeringBehavior::WallAvoidance(const std::vector<AISystem*>& walls)
{
  //the feelers are contained in a std::vector, m_Feelers
  CreateFeelers();
  
  double DistToThisIP    = 0.0;
  double DistToClosestIP = MaxDouble;

  //this will hold an index into the vector of walls
  int ClosestWall = -1;

  Vector2 SteeringForce,
            point,         //used for storing temporary info
            ClosestPoint;  //holds the closest intersection point

  //examine each feeler in turn
  for (unsigned int flr=0; flr<m_Feelers.size(); ++flr)
  {
    //run through each wall checking for any intersection points
    for (unsigned int w=0; w<walls.size(); ++w)
    {
      if (LineIntersection2D(m_pAIMovil->Pos(),
                             m_Feelers[flr],
                             walls[w].From(),
                             walls[w].To(),
                             DistToThisIP,
                             point))
      {
        //is this the closest found so far? If so keep a record
        if (DistToThisIP < DistToClosestIP)
        {
          DistToClosestIP = DistToThisIP;

          ClosestWall = w;

          ClosestPoint = point;
        }
      }
    }//next wall

  
    //if an intersection point has been detected, calculate a force  
    //that will direct the agent away
    if (ClosestWall >=0)
    {
      //calculate by what distance the projected position of the agent
      //will overshoot the wall
      Vector2 OverShoot = m_Feelers[flr] - ClosestPoint;

      //create a force in the direction of the wall normal, with a 
      //magnitude of the overshoot
      SteeringForce = walls[ClosestWall].Normal() * OverShoot.Length();
    }

  }//next feeler

  return SteeringForce;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void SteeringBehavior::CreateFeelers()
{
  //feeler pointing straight in front
  m_Feelers[0] = m_pAIMovil->Pos() + m_dWallDetectionFeelerLength * m_pAIMovil->Heading();

  //feeler to left
  Vector2 temp = m_pAIMovil->Heading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
  m_Feelers[1] = m_pAIMovil->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;

  //feeler to right
  temp = m_pAIMovil->Heading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
  m_Feelers[2] = m_pAIMovil->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;
}
*/

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Separation(const std::vector<AISystem*> &neighbors)
{  
  Vector2 SteeringForce = Vector2(0.0, 0.0);

  for (unsigned int a=0; a<neighbors.size(); ++a)
  {
    //make sure this agent isn't included in the calculations and that
    //the agent being examined is close enough. ***also make sure it doesn't
    //include the evade target ***
    if((neighbors[a] != m_pAIMovil) && neighbors[a]->IsTagged() &&
      (neighbors[a] != m_pTargetAgent1))
    {
        Vector2 ToAgent = m_pAIMovil->getPosition() - neighbors[a]->getPosition();

      //scale the force inversely proportional to the agents distance  
      //from its neighbor.
        if(ToAgent.length() != 0.0)
            SteeringForce += ToAgent.normalisedCopy() / ToAgent.length();
    }
  }

  return SteeringForce;
}

/*
//---------------------------- Alignment ---------------------------------
//
//  returns a force that attempts to align this agents heading with that
//  of its neighbors
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Alignment(const vector<AIMovil*>& neighbors)
{
  //used to record the average heading of the neighbors
  Vector2 AverageHeading;

  //used to count the number of AIMovils in the neighborhood
  int    NeighborCount = 0;

  //iterate through all the tagged AIMovils and sum their heading vectors  
  for (unsigned int a=0; a<neighbors.size(); ++a)
  {
    //make sure *this* agent isn't included in the calculations and that
    //the agent being examined  is close enough ***also make sure it doesn't
    //include any evade target ***
    if((neighbors[a] != m_pAIMovil) && neighbors[a]->IsTagged() &&
      (neighbors[a] != m_pTargetAgent1))
    {
      AverageHeading += neighbors[a]->Heading();

      ++NeighborCount;
    }
  }

  //if the neighborhood contained one or more AIMovils, average their
  //heading vectors.
  if (NeighborCount > 0)
  {
    AverageHeading /= (double)NeighborCount;

    AverageHeading -= m_pAIMovil->Heading();
  }
  
  return AverageHeading;
}

//-------------------------------- Cohesion ------------------------------
//
//  returns a steering force that attempts to move the agent towards the
//  center of mass of the agents in its immediate area
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Cohesion(const vector<AIMovil*> &neighbors)
{
  //first find the center of mass of all the agents
  Vector2 CenterOfMass, SteeringForce;

  int NeighborCount = 0;

  //iterate through the neighbors and sum up all the position vectors
  for (unsigned int a=0; a<neighbors.size(); ++a)
  {
    //make sure *this* agent isn't included in the calculations and that
    //the agent being examined is close enough ***also make sure it doesn't
    //include the evade target ***
    if((neighbors[a] != m_pAIMovil) && neighbors[a]->IsTagged() &&
      (neighbors[a] != m_pTargetAgent1))
    {
      CenterOfMass += neighbors[a]->Pos();

      ++NeighborCount;
    }
  }

  if (NeighborCount > 0)
  {
    //the center of mass is the average of the sum of positions
    CenterOfMass /= (double)NeighborCount;

    //now seek towards that position
    SteeringForce = Seek(CenterOfMass);
  }

  //the magnitude of cohesion is usually much larger than separation or
  //allignment so it usually helps to normalize it.
  return Vec2DNormalize(SteeringForce);
}

*/
/* NOTE: the next three behaviors are the same as the above three, except
          that they use a cell-space partition to find the neighbors
*/
/*

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//
//  USES SPACIAL PARTITIONING
//------------------------------------------------------------------------
Vector2 SteeringBehavior::SeparationPlus(const vector<AIMovil*> &neighbors)
{  
  Vector2 SteeringForce;

  //iterate through the neighbors and sum up all the position vectors
  for (AISystem* pV = m_pAIMovil->World()->CellSpace()->begin();
                         !m_pAIMovil->World()->CellSpace()->end();     
                       pV = m_pAIMovil->World()->CellSpace()->next())
  {    
    //make sure this agent isn't included in the calculations and that
    //the agent being examined is close enough
    if(pV != m_pAIMovil)
    {
      Vector2 ToAgent = m_pAIMovil->Pos() - pV->Pos();

      //scale the force inversely proportional to the agents distance  
      //from its neighbor.
      SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
    }

  }

  return SteeringForce;
}
//---------------------------- Alignment ---------------------------------
//
//  returns a force that attempts to align this agents heading with that
//  of its neighbors
//
//  USES SPACIAL PARTITIONING
//------------------------------------------------------------------------
Vector2 SteeringBehavior::AlignmentPlus(const vector<AIMovil*> &neighbors)
{
  //This will record the average heading of the neighbors
  Vector2 AverageHeading;

  //This count the number of AIMovils in the neighborhood
  double    NeighborCount = 0.0;

  //iterate through the neighbors and sum up all the position vectors
  for (MovingEntity* pV = m_pAIMovil->World()->CellSpace()->begin();
                         !m_pAIMovil->World()->CellSpace()->end();     
                     pV = m_pAIMovil->World()->CellSpace()->next())
  {
    //make sure *this* agent isn't included in the calculations and that
    //the agent being examined  is close enough
    if(pV != m_pAIMovil)
    {
      AverageHeading += pV->Heading();

      ++NeighborCount;
    }

  }

  //if the neighborhood contained one or more AIMovils, average their
  //heading vectors.
  if (NeighborCount > 0.0)
  {
    AverageHeading /= NeighborCount;

    AverageHeading -= m_pAIMovil->Heading();
  }
  
  return AverageHeading;
}


//-------------------------------- Cohesion ------------------------------
//
//  returns a steering force that attempts to move the agent towards the
//  center of mass of the agents in its immediate area
//
//  USES SPACIAL PARTITIONING
//------------------------------------------------------------------------
Vector2 SteeringBehavior::CohesionPlus(const vector<AIMovil*> &neighbors)
{
  //first find the center of mass of all the agents
  Vector2 CenterOfMass, SteeringForce;

  int NeighborCount = 0;

  //iterate through the neighbors and sum up all the position vectors
  for (AISystem* pV = m_pAIMovil->World()->CellSpace()->begin();
                         !m_pAIMovil->World()->CellSpace()->end();     
                       pV = m_pAIMovil->World()->CellSpace()->next())
  {
    //make sure *this* agent isn't included in the calculations and that
    //the agent being examined is close enough
    if(pV != m_pAIMovil)
    {
      CenterOfMass += pV->Pos();

      ++NeighborCount;
    }
  }

  if (NeighborCount > 0)
  {
    //the center of mass is the average of the sum of positions
    CenterOfMass /= (double)NeighborCount;

    //now seek towards that position
    SteeringForce = Seek(CenterOfMass);
  }

  //the magnitude of cohesion is usually much larger than separation or
  //allignment so it usually helps to normalize it.
  return Vec2DNormalize(SteeringForce);
}


//--------------------------- Interpose ----------------------------------
//
//  Given two agents, this method returns a force that attempts to 
//  position the AIMovil between them
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Interpose(const AIMovil* AgentA,
                                     const AIMovil* AgentB)
{
  //first we need to figure out where the two agents are going to be at 
  //time T in the future. This is approximated by determining the time
  //taken to reach the mid way point at the current time at at max speed.
  Vector2 MidPoint = (AgentA->Pos() + AgentB->Pos()) / 2.0;

  double TimeToReachMidPoint = Vec2DDistance(m_pAIMovil->Pos(), MidPoint) /
                               m_pAIMovil->MaxSpeed();

  //now we have T, we assume that agent A and agent B will continue on a
  //straight trajectory and extrapolate to get their future positions
  Vector2 APos = AgentA->Pos() + AgentA->Velocity() * TimeToReachMidPoint;
  Vector2 BPos = AgentB->Pos() + AgentB->Velocity() * TimeToReachMidPoint;

  //calculate the mid point of these predicted positions
  MidPoint = (APos + BPos) / 2.0;

  //then steer to Arrive at it
  return Arrive(MidPoint, fast);
}

//--------------------------- Hide ---------------------------------------
//
//------------------------------------------------------------------------
Vector2 SteeringBehavior::Hide(const AIMovil*           hunter,
                                 const vector<AISystem*>& obstacles)
{
  double    DistToClosest = MaxDouble;
  Vector2 BestHidingSpot;

  std::vector<AISystem*>::const_iterator curOb = obstacles.begin();
  std::vector<AISystem*>::const_iterator closest;

  while(curOb != obstacles.end())
  {
    //calculate the position of the hiding spot for this obstacle
    Vector2 HidingSpot = GetHidingPosition((*curOb)->Pos(),
                                             (*curOb)->BRadius(),
                                              hunter->Pos());
            
    //work in distance-squared space to find the closest hiding
    //spot to the agent
    double dist = Vec2DDistanceSq(HidingSpot, m_pAIMovil->Pos());

    if (dist < DistToClosest)
    {
      DistToClosest = dist;

      BestHidingSpot = HidingSpot;

      closest = curOb;
    }  
            
    ++curOb;

  }//end while
  
  //if no suitable obstacles found then Evade the hunter
  if (DistToClosest == MaxFloat)
  {
    return Evade(hunter);
  }
      
  //else use Arrive on the hiding spot
  return Arrive(BestHidingSpot, fast);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position DistanceFromBoundary 
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
Vector2 SteeringBehavior::GetHidingPosition(const Vector2& posOb,
                                              const double     radiusOb,
                                              const Vector2& posHunter)
{
  //calculate how far away the agent is to be from the chosen obstacle's
  //bounding radius
  const double DistanceFromBoundary = 30.0;
  double       DistAway    = radiusOb + DistanceFromBoundary;

  //calculate the heading toward the object from the hunter
  Vector2 ToOb = Vec2DNormalize(posOb - posHunter);
  
  //scale it to size and add to the obstacles position to get
  //the hiding spot.
  return (ToOb * DistAway) + posOb;
}
*/

//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2s, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
/*
Vector2 SteeringBehavior::FollowPath()
{ 
    
  //move to next target if close enough to current target (working in
  //distance squared space)
    if(Vec2DDistanceSq(m_pPath->CurrentWaypoint(), m_pAIMovil->getPosition()) <
     m_dWaypointSeekDistSq)
     // distancia de seguridad antes de un waypoint tras la cual debe cambiar de currentwaypoint
  {
    m_pPath->SetNextWaypoint();
  }

  if (!m_pPath->Finished())
  {
    return Seek(m_pPath->CurrentWaypoint());
  }
  else
  {
    return Arrive(m_pPath->CurrentWaypoint(), normal);
  }

}
*/
/*
//------------------------- Offset Pursuit -------------------------------
//
//  Produces a steering force that keeps a AIMovil at a specified offset
//  from a leader AIMovil
//------------------------------------------------------------------------
Vector2 SteeringBehavior::OffsetPursuit(const AIMovil*  leader,
                                          const Vector2 offset)
{
  //calculate the offset's position in world space
  Vector2 WorldOffsetPos = PointToWorldSpace(offset,
                                                  leader->Heading(),
                                                  leader->Side(),
                                                  leader->Pos());

  Vector2 ToOffset = WorldOffsetPos - m_pAIMovil->Pos();

  //the lookahead time is propotional to the distance between the leader
  //and the pursuer; and is inversely proportional to the sum of both
  //agent's velocities
  double LookAheadTime = ToOffset.Length() / 
                        (m_pAIMovil->MaxSpeed() + leader->Speed());
  
  //now Arrive at the predicted future position of the offset
  return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
}

*/



