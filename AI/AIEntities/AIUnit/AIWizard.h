#ifndef AIWIZARD_H_
#define AIWIZARD_H_

#include "AIUnit.h"
#include "AIWizardState.h"
#include "AIStateSystem.h"
#include "Ogre.h"
#include "FactoryGraphics.h"
#include "Framework.h"
#include "MessageSystem.h"

struct Telegram;

class AIWizard : public AIUnit
{
private:

    AIStateSystem<AIWizard>*  m_pStateMachine;

protected:

public:

    AIWizard();

    AIWizard(bool bIsAttacking, int type, int race, Ogre::Vector2 position, Ogre::Vector2 orientation);

	void Update         (double time_elapsed);

    Ogre::Vector2       seekFlag();
    Ogre::Vector2       seekUnit();
    Vector2             WizardSeekToFlag();

    Ogre::Vector2       charge();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    AIStateSystem<AIWizard>*   GetFSM()const{return m_pStateMachine;}

};

#endif