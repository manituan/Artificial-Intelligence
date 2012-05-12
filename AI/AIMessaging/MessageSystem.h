#ifndef MESSAGE_SYSTEM_H
#define MESSAGE_SYSTEM_H

#pragma warning (disable:4786)

#include <set>

#include "EntityCityManager.h"
#include "Opcode.h"
#include "Ogre.h"
#include "AISystem.h"
#include "FactoryGraphics.h"
#include "GraphUnit.h"

class AISystem;

//to make code easier to read
const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO   = 0;

class MessageSystem: public Singleton<MessageSystem>
{
private:  
  
    //a std::set is used as the container for the delayed messages
    //because of the benefit of automatic sorting and avoidance
    //of duplicates. Messages are sorted by their dispatch time.
    std::set<Telegram> PriorityQ;

    //this method is utilized by DispatchMessageEntity or DispatchDelayedMessages.
    //This method calls the message handling member function of the receiving
    //entity, pReceiver, with the newly created telegram
    void Discharge(AISystem* pReceiver, const Telegram& msg);
    void DischargeFactoryGraphics(FactoryGraphics* pReceiver, const Telegram& telegram);
    void DischargeGraphUnit(GraphSystem* pReceiver, const Telegram& telegram);

    //copy ctor and assignment should be private
    MessageSystem(const MessageSystem&);
    MessageSystem& operator=(const MessageSystem&);

public:

  void DispatchMessageToGraphFactory(int sender, int receiver, int msg, Vector2 pos, Vector2 ori, int type, int race, Vector2 health); 

  //send a message to another agent. Receiving agent is referenced by ID.
  void DispatchMessageEntity(double  delay, int sender, int receiver, int msg);

  void DispatchMessageGraphUnit(int sender, int receiver, int msg, Vector2 pos, Vector2 ori, float delta, int type, int race, int animation, double health);

    void DispatchMessageToDeleteGraphSystem(int sender, int receiver, int msg);

  //send out any delayed messages. This method is called each time through   
  //the main game loop.
  void DispatchDelayedMessages();

  MessageSystem();
  ~MessageSystem();

};

#endif