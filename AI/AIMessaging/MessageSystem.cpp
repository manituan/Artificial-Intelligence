#include "MessageSystem.h"

#include <iostream>
using std::cout;

using std::set;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

template<> MessageSystem* Singleton<MessageSystem>::msSingleton = 0;

//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageSystem::Discharge(AISystem* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    //telegram could not be handled
    cout << "Message not handled";
  }
}

void MessageSystem::DischargeFactoryGraphics(FactoryGraphics* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    //telegram could not be handled
    cout << "Message not handled";
  }
}

void MessageSystem::DischargeGraphUnit(GraphSystem* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    //telegram could not be handled
    cout << "Message not handled";
  }
}

MessageSystem::MessageSystem(){}
MessageSystem::~MessageSystem(){}


void MessageSystem::DispatchMessageEntity(double  delay, int sender, int receiver, int msg)
{
   //get pointers to the sender and receiver
  AISystem* pSender   = EntityCityManager::getSingletonPtr()->GetEntity(sender);
  AISystem* pReceiver = EntityCityManager::getSingletonPtr()->GetEntity(receiver);

  //make sure the receiver is valid
  if (pReceiver == NULL)
  {
    cout << "\nWarning! No Receiver with ID of " << receiver << " found";

    return;
  }
  
  //create the telegram
  Telegram telegram(delay, sender, receiver, msg);
  
  //if there is no delay, route telegram immediately                       
  if (delay <= 0.0f)                                                        
  {
      /*
    cout << "\nInstant telegram dispatched at time: " << Clock->GetCurrentTime()
         << " by " << GetNameOfEntity(pSender->ID()) << " for " << GetNameOfEntity(pReceiver->ID()) 
         << ". Msg is "<< MsgToStr(msg);
         */
    //send the telegram to the recipient
    Discharge(pReceiver, telegram);
  }

  //else calculate the time when the telegram should be dispatched
  else
  {
    //double CurrentTime = Clock->GetCurrentTime(); 

    //telegram.DispatchTime = CurrentTime + delay;

    //and put it in the queue
    PriorityQ.insert(telegram);   

    /*cout << "\nDelayed telegram from " << GetNameOfEntity(pSender->ID()) << " recorded at time " 
            << Clock->GetCurrentTime() << " for " << GetNameOfEntity(pReceiver->ID())
            << ". Msg is "<< MsgToStr(msg);
   */         
  }
}

void MessageSystem::DispatchMessageToGraphFactory(int sender, int receiver, int msg, Vector2 pos, Vector2 ori, int type, int race, Vector2 health)
{

  // get pointers to the sender and receiver
  // pSender no está creado en este punto todavía...
  // AISystem* pSender = EntityCityManager::getSingletonPtr()->GetEntityFromID(sender);

  FactoryGraphics* pReceiver = FactoryGraphics::getSingletonPtr();

  //create the telegram
  Telegram telegram(0, sender, receiver, msg, pos, ori, NULL, type, race, health.x, health.y);

  DischargeFactoryGraphics(pReceiver, telegram);
  
}

void MessageSystem::DispatchMessageToDeleteGraphSystem(int sender, int receiver, int msg)
{

  //get pointers to the sender and receiver
  // pSender no está creado en este punto todavía...
  //AISystem* pSender = EntityCityManager::getSingletonPtr()->GetEntityFromID(sender);

  FactoryGraphics* pReceiver = FactoryGraphics::getSingletonPtr();

  //create the telegram
  Telegram telegram(0, sender, receiver, msg);

  DischargeFactoryGraphics(pReceiver, telegram);
  
}

void MessageSystem::DispatchMessageGraphUnit(int sender, int receiver, int msg,  Vector2 pos, Vector2 ori, float delta, int type, int race, int animation, double health)
{

    //get pointers to the sender and receiver
    AISystem* pSender = EntityCityManager::getSingletonPtr()->GetEntity(sender);

    // Receiver is null
    GraphSystem* pReceiver = FactoryGraphics::getSingletonPtr()->GetGraphFromID(receiver);

    //make sure the receiver is valid
    if (pReceiver == NULL)
    {
        cout << "\nWarning! No Receiver with ID of " << receiver << " found";
        return;
    }
  
    // create the telegram
    Telegram telegram(0, sender, receiver, msg, pos, ori, delta, type, race, animation, health);

    DischargeGraphUnit(pReceiver, telegram);

}

//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void MessageSystem::DispatchDelayedMessages()
{
    /*
  //SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
  
  //get current time
  //double CurrentTime = Clock->GetCurrentTime();

  //now peek at the queue to see if any telegrams need dispatching.
  //remove all telegrams from the front of the queue that have gone
  //past their sell by date
  
    while( !PriorityQ.empty() &&
         (PriorityQ.begin()->DispatchTime < CurrentTime) && 
         (PriorityQ.begin()->DispatchTime > 0) )
  {
  
    //read the telegram from the front of the queue
    const Telegram& telegram = *PriorityQ.begin();

    //find the recipient
    AISystem* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);
    /*
    cout << "\nQueued telegram ready for dispatch: Sent to " 
         << GetNameOfEntity(pReceiver->ID()) << ". Msg is " << MsgToStr(telegram.Msg);

    //send the telegram to the recipient
    Discharge(pReceiver, telegram);

    //remove it from the queue
    PriorityQ.erase(PriorityQ.begin());
  }
  */
}



