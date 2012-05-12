#ifndef AI_STATE_SYSTEM_H
#define AI_STATE_SYSTEM_H

#include "AIState.h"
#include "Telegram.h"
#include <assert.h>

template <class entityType>

class AIStateSystem
{
private:

    // a pointer to the agent that owns this instance
    entityType*                  m_pOwner;

    AIState<entityType>*         m_pCurrentState;
  
    // a record of the last state the agent was in
    AIState<entityType>*         m_pPreviousState;

    // this is called every time the FSM is updated
    AIState<entityType>*         m_pGlobalState; 

public:

    AIStateSystem(entityType* owner):m_pOwner(owner),
                                    m_pCurrentState(NULL),
                                    m_pPreviousState(NULL),
                                    m_pGlobalState(NULL)
    {}

    virtual ~AIStateSystem(){}

    // use these methods to initialize the FSM
    void SetCurrentState(AIState<entityType>* s){m_pCurrentState = s;}
    void SetGlobalState(AIState<entityType>* s) {m_pGlobalState = s;}
    void SetPreviousState(AIState<entityType>* s){m_pPreviousState = s;}
  
    // call this to update the FSM
    void  Update()const
    {
        // if a global state exists, call its execute method, else do nothing
        if(m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

        // same for the current state
        if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);

    }

    bool  AIStateSystem::HandleMessage(const Telegram& msg)const
    {
        //first see if the current state is valid and that it can handle
        //the message
        if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
        {
            return true;
        }
  
        //if not, and if a global state has been implemented, send 
        //the message to the global state
        if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
        {
            return true;
        }

        return false;
    }

    // change to a new state
    void  ChangeState(AIState<entityType>* pNewState)
    {
        assert(pNewState && "<AIState::ChangeState>:trying to assign null state to current");

        // keep a record of the previous state
        m_pPreviousState = m_pCurrentState;

        // call the exit method of the existing state
        m_pCurrentState->Exit(m_pOwner);

        // change state to the new state
        m_pCurrentState = pNewState;

        // call the entry method of the new state
        m_pCurrentState->Enter(m_pOwner);
    }

    // change state back to the previous state
    void  RevertToPreviousState()
    {
        ChangeState(m_pPreviousState);
    }

    // returns true if the current state's type is equal to the type of the
    // class passed as a parameter. 
    bool  isInState(const AIState<entityType>& st)const
    {
        if (typeid(*m_pCurrentState) == typeid(st)) return true;
        return false;
    }

    AIState<entityType>*  CurrentState()  const{return m_pCurrentState;}
    AIState<entityType>*  GlobalState()   const{return m_pGlobalState;}
    AIState<entityType>*  PreviousState() const{return m_pPreviousState;}

};

#endif