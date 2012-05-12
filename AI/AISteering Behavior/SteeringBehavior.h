#ifndef STEERINGBEHAVIOR_H
#define STEERINGBEHAVIOR_H
#pragma warning (disable:4786)

#include "Ogre.h"
//#include "EntityCityManager.h"
#include <C2DMatrix.h>

class AIMovil;
class AISystem;

using namespace Ogre;

//--------------------------- Constants ----------------------------------

//the radius of the constraining circle for the wander behavior
const double WanderRad    = 1.2;
//distance the wander circle is projected in front of the agent
const double WanderDist   = 2.0;
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec = 80.0;

//used in path following
const double WaypointSeekDist   = 20;                                          

//------------------------------------------------------------------------

class SteeringBehavior
{
public:
  
  enum summing_method{weighted_average, prioritized, dithered};

private:

  enum behavior_type
  {
    none               = 0x00000,
    seek               = 0x00002,
    flee               = 0x00004,
    arrive             = 0x00008,
    wander             = 0x00010,
    cohesion           = 0x00020,
    separation         = 0x00040,
    allignment         = 0x00080,
    obstacle_avoidance = 0x00100,
    wall_avoidance     = 0x00200,
    follow_path        = 0x00400,
    pursuit            = 0x00800,
    evade              = 0x01000,
    interpose          = 0x02000,
    hide               = 0x04000,
    flock              = 0x08000,
    offset_pursuit     = 0x10000,
  };

private:

  
  //a pointer to the owner of this instance
  AIMovil*      m_pAIMovil;   
  
  //the steering force created by the combined effect of all
  //the selected behaviors
  Vector2       m_vSteeringForce;
 
  //these can be used to keep track of friends, pursuers, or prey
  AIMovil*      m_pTargetAgent1;
  AIMovil*      m_pTargetAgent2;

  //the current target
  Vector2       m_vTarget;

  //length of the 'detection box' utilized in obstacle avoidance
  double        m_dDBoxLength;


  //a vertex buffer to contain the feelers rqd for wall avoidance  
  std::vector<Vector2> m_Feelers;
  
  //the length of the 'feeler/s' used in wall detection
  double     m_dWallDetectionFeelerLength;



  //the current position on the wander circle the agent is
  //attempting to steer towards
  Vector2     m_vWanderTarget; 

  //explained above
  double        m_dWanderJitter;
  double        m_dWanderRadius;
  double        m_dWanderDistance;


  //multipliers. These can be adjusted to effect strength of the  
  //appropriate behavior. Useful to get flocking the way you require
  //for example.
  double        m_dWeightSeparation;
  double        m_dWeightCohesion;
  double        m_dWeightAlignment;
  double        m_dWeightWander;
  double        m_dWeightObstacleAvoidance;
  double        m_dWeightWallAvoidance;
  double        m_dWeightSeek;
  double        m_dWeightFlee;
  double        m_dWeightArrive;
  double        m_dWeightPursuit;
  double        m_dWeightOffsetPursuit;
  double        m_dWeightInterpose;
  double        m_dWeightHide;
  double        m_dWeightEvade;
  double        m_dWeightFollowPath;

  //how far the agent can 'see'
  double        m_dViewDistance;

  //the distance (squared) a AIMovil has to be from a path waypoint before
  //it starts seeking to the next waypoint
  double        m_dWaypointSeekDistSq;

  //any offset used for formations or offset pursuit
  Vector2     m_vOffset;

  //binary flags to indicate whether or not a behavior should be active
  int           m_iFlags;
  
  //Arrive makes use of these to determine how quickly a AIMovil
  //should decelerate to its target
  enum Deceleration{slow = 3, normal = 2, fast = 1};

  //default
  Deceleration m_Deceleration;

  //is cell space partitioning to be used or not?
  bool          m_bCellSpaceOn;
 
  //what type of method is used to sum any active behavior
  summing_method  m_SummingMethod;


  //this function tests if a specific bit of m_iFlags is set
  bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}

  bool      AccumulateForce(Vector2 &sf, Vector2 ForceToAdd);

  //creates the antenna utilized by the wall avoidance behavior
  void      CreateFeelers();


public:

   /* .......................................................

                    BEGIN BEHAVIOR DECLARATIONS

      .......................................................*/

  double SteeringBehavior::Vec2DDistanceSq(const Vector2 &v1, const Vector2 &v2);

  //this behavior moves the agent towards a target position
  Vector2 Seek(Vector2 TargetPos);

  //this behavior returns a vector that moves the agent away
  //from a target position
  Vector2 Flee(Vector2 TargetPos);

  //this behavior is similar to seek but it attempts to arrive 
  //at the target position with a zero velocity
  Vector2 Arrive(Vector2     TargetPos,
                  Deceleration deceleration);

  //this behavior predicts where an agent will be in time T and seeks
  //towards that point to intercept it.
  Vector2 Pursuit(const AIMovil* agent);

  //this behavior maintains a position, in the direction of offset
  //from the target AIMovil
  Vector2 OffsetPursuit(const AIMovil* agent, const Vector2 offset);

  //this behavior attempts to evade a pursuer
  Vector2 Evade(const AIMovil* agent);

  //this behavior makes the agent wander about randomly
  Vector2 Wander();

  //this returns a steering force which will attempt to keep the agent 
  //away from any obstacles it may encounter
  Vector2 ObstacleAvoidance(const std::vector<AISystem*>& obstacles);
  
  //given a series of Vector2s, this method produces a force that will
  //move the agent along the waypoints in order
  Vector2 FollowPath();

  //this results in a steering force that attempts to steer the AIMovil
  //to the center of the vector connecting two moving agents.
  Vector2 Interpose(const AIMovil* AIMovilA, const AIMovil* AIMovilB);

  //given another agent position to hide from and a list of AISystems this
  //method attempts to put an obstacle between itself and its opponent
  Vector2 Hide(const AIMovil* hunter, const std::vector<AISystem*>& obstacles);


  // -- Group Behaviors -- //

  Vector2 Cohesion(const std::vector<AIMovil*> &agents);
  
  Vector2 Separation(const std::vector<AISystem*> &agents);

  Vector2 Alignment(const std::vector<AIMovil*> &agents);

  //the following three are the same as above but they use cell-space
  //partitioning to find the neighbors
  Vector2 CohesionPlus(const std::vector<AIMovil*> &agents);
  Vector2 SeparationPlus(const std::vector<AIMovil*> &agents);
  Vector2 AlignmentPlus(const std::vector<AIMovil*> &agents);

    /* .......................................................

                       END BEHAVIOR DECLARATIONS

      .......................................................*/

  //calculates and sums the steering forces from any active behaviors
  Vector2 CalculateWeightedSum();
  Vector2 CalculatePrioritized();
  Vector2 CalculateDithered();

  //helper method for Hide. Returns a position located on the other
  //side of an obstacle to the pursuer
  Vector2 GetHidingPosition(const Vector2& posOb,
                              const double     radiusOb,
                              const Vector2& posHunter);

  SteeringBehavior(AIMovil* agent);

  ~SteeringBehavior();

  //calculates and sums the steering forces from any active behaviors
  Vector2 Calculate();

  //calculates the component of the steering force that is parallel
  //with the AIMovil heading
  double    ForwardComponent();

  //calculates the component of the steering force that is perpendicuar
  //with the AIMovil heading
  double    SideComponent();



  //renders visual aids and info for seeing how each behavior is
  //calculated
  void      RenderAids();

  void      SetTarget(const Vector2 t){m_vTarget = t;}

  void      SetTargetAgent1(AIMovil* Agent){m_pTargetAgent1 = Agent;}
  void      SetTargetAgent2(AIMovil* Agent){m_pTargetAgent2 = Agent;}

  void      SetOffset(const Vector2 offset){m_vOffset = offset;}
  Vector2  GetOffset()const{return m_vOffset;}

  Vector2 Force()const{return m_vSteeringForce;}

  void      ToggleSpacePartitioningOnOff(){m_bCellSpaceOn = !m_bCellSpaceOn;}
  bool      isSpacePartitioningOn()const{return m_bCellSpaceOn;}

  void      SetSummingMethod(summing_method sm){m_SummingMethod = sm;}


  void FleeOn(){m_iFlags |= flee;}
  void SeekOn(){m_iFlags |= seek;}
  void ArriveOn(){m_iFlags |= arrive;}
  void WanderOn(){m_iFlags |= wander;}
  void PursuitOn(AIMovil* v){m_iFlags |= pursuit; m_pTargetAgent1 = v;}
  void EvadeOn(AIMovil* v){m_iFlags |= evade; m_pTargetAgent1 = v;}
  void CohesionOn(){m_iFlags |= cohesion;}
  void SeparationOn(){m_iFlags |= separation;}
  void AlignmentOn(){m_iFlags |= allignment;}
  void ObstacleAvoidanceOn(){m_iFlags |= obstacle_avoidance;}
  void WallAvoidanceOn(){m_iFlags |= wall_avoidance;}
  void FollowPathOn(){m_iFlags |= follow_path;}
  void InterposeOn(AIMovil* v1, AIMovil* v2){m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2;}
  void HideOn(AIMovil* v){m_iFlags |= hide; m_pTargetAgent1 = v;}
  void OffsetPursuitOn(AIMovil* v1, const Vector2 offset){m_iFlags |= offset_pursuit; m_vOffset = offset; m_pTargetAgent1 = v1;}  
  void FlockingOn(){CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn();}

  void FleeOff()  {if(On(flee))   m_iFlags ^=flee;}
  void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
  void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
  void WanderOff(){if(On(wander)) m_iFlags ^=wander;}
  void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
  void EvadeOff(){if(On(evade)) m_iFlags ^=evade;}
  void CohesionOff(){if(On(cohesion)) m_iFlags ^=cohesion;}
  void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
  void AlignmentOff(){if(On(allignment)) m_iFlags ^=allignment;}
  void ObstacleAvoidanceOff(){if(On(obstacle_avoidance)) m_iFlags ^=obstacle_avoidance;}
  void WallAvoidanceOff(){if(On(wall_avoidance)) m_iFlags ^=wall_avoidance;}
  void FollowPathOff(){if(On(follow_path)) m_iFlags ^=follow_path;}
  void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}
  void HideOff(){if(On(hide)) m_iFlags ^=hide;}
  void OffsetPursuitOff(){if(On(offset_pursuit)) m_iFlags ^=offset_pursuit;}
  void FlockingOff(){CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff();}

  bool isFleeOn(){return On(flee);}
  bool isSeekOn(){return On(seek);}
  bool isArriveOn(){return On(arrive);}
  bool isWanderOn(){return On(wander);}
  bool isPursuitOn(){return On(pursuit);}
  bool isEvadeOn(){return On(evade);}
  bool isCohesionOn(){return On(cohesion);}
  bool isSeparationOn(){return On(separation);}
  bool isAlignmentOn(){return On(allignment);}
  bool isObstacleAvoidanceOn(){return On(obstacle_avoidance);}
  bool isWallAvoidanceOn(){return On(wall_avoidance);}
  bool isFollowPathOn(){return On(follow_path);}
  bool isInterposeOn(){return On(interpose);}
  bool isHideOn(){return On(hide);}
  bool isOffsetPursuitOn(){return On(offset_pursuit);}

  double DBoxLength()const{return m_dDBoxLength;}
  const std::vector<Vector2>& GetFeelers()const{return m_Feelers;}
  
  double WanderJitter()const{return m_dWanderJitter;}
  double WanderDistance()const{return m_dWanderDistance;}
  double WanderRadius()const{return m_dWanderRadius;}

  double SeparationWeight()const{return m_dWeightSeparation;}
  double AlignmentWeight()const{return m_dWeightAlignment;}
  double CohesionWeight()const{return m_dWeightCohesion;}

//--------------------- PointToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline Vector2 PointToLocalSpace(const Vector2 &point,
                             Vector2 &AgentHeading,
                             Vector2 &AgentSide,
                              Vector2 &AgentPosition)
{

	//make a copy of the point
  Vector2 TransPoint = point;
  
  //create a transformation matrix
  C2DMatrix matTransform;

  double Tx = -AgentPosition.dotProduct(AgentHeading);
  double Ty = -AgentPosition.dotProduct(AgentSide);

  //create the transformation matrix
  matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
  matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
  matTransform._31(Tx);           matTransform._32(Ty);
	
  //now transform the vertices
  matTransform.TransformVector2Ds(Vector2D(TransPoint.x, TransPoint.y));

  return TransPoint;
}

//--------------------- VectorToWorldSpace --------------------------------
//
//  Transforms a vector from the agent's local space into world space
//------------------------------------------------------------------------
inline Vector2 VectorToWorldSpace(const Vector2 &vec,
                                     const Vector2 &AgentHeading,
                                     const Vector2 &AgentSide)
{
    //make a copy of the point
    Vector2 TransVec = vec;
  
    //create a transformation matrix
    C2DMatrix matTransform;

    //rotate
    matTransform.Rotate(Vector2D(AgentHeading.x, AgentHeading.y), Vector2D(AgentSide.x, AgentSide.y));

    //now transform the vertices
    matTransform.TransformVector2Ds(Vector2D(TransVec.x, TransVec.y));

    return TransVec;
}

};

#endif