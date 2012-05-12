#ifndef AI_STATE_H
#define AI_STATE_H

struct Telegram;

template <class entityType>

class AIState
{
public:

  virtual ~AIState(){}

  //this will execute when the state is entered
  virtual bool Enter(entityType*)=0;

  //this is the states normal update function
  virtual bool Execute(entityType*)=0;

  //this will execute when the state is exited. 
  virtual bool Exit(entityType*)=0;

  //this executes if the agent receives a message from the 
  //message dispatcher
  virtual bool OnMessage(entityType*, const Telegram&)=0;

};

#endif