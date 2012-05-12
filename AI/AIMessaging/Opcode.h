#ifndef OPCODE
#define OPCODE

#include <string>
#include <iostream>
#include <math.h>
#include "Ogre.h"

// Local messages in server side

using namespace Ogre;

enum {default_entity_type = 0L};

enum classesEnum
{
    factoryEnt,
    factoryGraph
};

enum message_Local
{
    // States
    LMSG_EnemyNear,
    LMSG_AttackTo,
    LMSG_ChargingTo,
    LMSG_SeekFlag,
    LMSG_Leave,
    LMSG_ImDying,
};

/*
enum message_Client
{
    //POR AHORA NO TIENE NINGUNO
};
*/

enum message_Server
{
    // AI-Online
    SMSG_AISystemToEntityCity,
    SMSG_AICreateGraphUnit,
    SMSG_AICreateGraphBuilding,
    SMSG_AISystemToGraphSystem,
    SMSG_AIDeleteGraphSystem,
    SMSG_AIDeleteGraphAnimation
};

//Define Server To Client Message ( All with PACKED macros ending)
struct MsgServerBase
{
	unsigned int	Msg;				//Code of the message
	unsigned long	ToGraphEntityID;	//Id of the Ogre entity ( 0 = Ogre_Factory )
};

struct MsgCreateUnit: public MsgServerBase
{
	int				fromGameEntityID;
	unsigned int	UnitType;
	unsigned int	Race;
};

struct MsgUpdateUnit: public MsgServerBase
{
	float PosX;
	float PosY;
	float RotZ;
	int   Health;
};

/*
struct MsgSelectUnit: public MsgServerBase
{
	char SelectModify;	//0 - Selected, 1 - Unselected
};
*/

//Define Client To Server Message ( All with PACKED macros ending)
struct MsgClientBase
{
	unsigned int	Msg;				//Code of the message
	int				ToGameEntityID;		//Id of the AI entity ( -1 = GameWorld )
};

//Return the Ogre_Unit ID to the IA
struct MsgReqCreateUnit: public MsgClientBase
{
	unsigned long UnitType;
	float X;
	float Y;
};

//Return the Ogre_Unit ID to the IA ???
struct MsgCreatedUnit: public MsgClientBase
{
	unsigned long GraphEntityID;
};

/*
//Process Mouse Clicks Over the Game Field
struct MsgMouseClick:public MsgClientBase
{
	char MouseButton;	//0 - Left, 1 - Right, 2 - Middle
	float X;			//Ogre World Coordinate
	float Y;
	float Z;
};
*/

/*
//Process Mouse Clicks Over the Game Field
struct MsgClickOnUnit:public MsgClientBase
{
	char SelectModify;	//0 - Add, 1 - Remove
};
*/

struct Telegram
{
  //the entity that sent this telegram

  int          Sender;

  //the entity that is to receive this telegram
  int          Receiver;

  //the message itself. These are all enumerated in the file
  //"MessageTypes.h"
  int          Msg;

  //messages can be dispatched immediately or delayed for a specified amount
  //of time. If a delay is necessary this field is stamped with the time 
  //the message should be dispatched.
  double       DispatchTime;

  //any additional information that may accompany the message
  Vector2 Position;

  //any additional information that may accompany the message
  Vector2 Orientation; 

  float   DeltaTime;

  int iType;

  int iRace;

  int iAnimation;

  double dActualHealth;

  double dTotalHealth;

    Telegram(double time,
            int    sender,
            int    receiver,
            int    msg):DispatchTime(time),
                                Sender(sender),
                                Receiver(receiver),
                                Msg(msg)
    {}

    Telegram(double time,
            int    sender,
            int    receiver,
            int    msg,
            Vector2  pos,
            Vector2  ori,
            float    delta,
            int      type,
            int      race,
            double  actualHealth,
            double  totalHealth): DispatchTime(time),
                                Sender(sender),
                                Receiver(receiver),
                                Msg(msg),
                                Position(pos),
                                Orientation(ori),
                                DeltaTime(delta),
                                iType(type),
                                iRace(race),
                                dActualHealth(actualHealth),
                                dTotalHealth(totalHealth)
    {}

    Telegram(double time,
            int    sender,
            int    receiver,
            int    msg,
            Vector2  pos,
            Vector2  ori,
            float    delta,
            int      type,
            int      race,
            int      animation,
            double  actualHealth): DispatchTime(time),
                                Sender(sender),
                                Receiver(receiver),
                                Msg(msg),
                                Position(pos),
                                Orientation(ori),
                                DeltaTime(delta),
                                iType(type),
                                iRace(race),
                                iAnimation(animation),
                                dActualHealth(actualHealth)
    {}

    Telegram(double time,
            int    sender,
            int    receiver,
            int    msg,
            Vector2  pos,
            Vector2  ori): DispatchTime(time),
                                Sender(sender),
                                Receiver(receiver),
                                Msg(msg),
                                Position(pos),
                                Orientation(ori)
    {}
};

//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
  return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
          (t1.Sender == t2.Sender)        &&
          (t1.Receiver == t2.Receiver)    &&
          (t1.Msg == t2.Msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
  if (t1 == t2)
  {
    return false;
  }

  else
  {
    return  (t1.DispatchTime < t2.DispatchTime);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t)
{
  os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
     << "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;

  return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}
/*
inline std::string MsgToStr(int msg)
{
  switch (msg)
  {

  case SMSG_AISystemToEntityCity:
    
    return "SMSG_AISystemToEntityCity";

  case SMSG_AISystemToGraphFactory:
    
    return "SMSG_AISystemToGraphFactory";

  case SMSG_AISystemToGraphSystem:
    
    return "SMSG_AISystemToGraphSystem";

  // ... 
    

  default:

    return "Not recognized!";
  }
}
*/

#endif