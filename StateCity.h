//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef STATECITY_H
#define STATECITY_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

#include "DotSceneLoader.h"

#include "Camera.h"

#include "Light.h"

#include "InputKey.h"
#include "InputMouse.h"

#include <OgreInstancedEntity.h>

#include "FactoryEntity.h"
#include "EntityCityManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

class StateCity : public AppState
{
public:
    StateCity();

    DECLARE_APPSTATE_CLASS(StateCity);

    void enter();
    void createScene();
    void exit();
    bool pause();
    void resume();
    
    bool getInput();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    bool onLeftPressed(const OIS::MouseEvent &evt);
    bool itemSelected(OgreBites::SelectMenu* menu);

    void update(double timeSinceLastFrame);

    void buildGUI();

    void wait (int seconds)
    {
        clock_t endwait;
        endwait = clock () + seconds * CLOCKS_PER_SEC ;
        while (clock() < endwait) {}
    }

    CameraSystem* getCameraSystem(){return m_pCamera;}

private:

    CameraCity*                  m_pCamera;
    Ogre::Camera*                mCamera;

    LightPoint*                  m_pLightP;
    LightSpot*                   m_pLightS;
    LightDir*                    m_pLightD_Left;
    LightDir*                    m_pLight_Custom;
    LightDir*                    m_pLightD_Front;

    Ogre::ColourValue            m_AmbientColor;
	
    InputKey*                    m_pKey;
    InputMouse*                  m_pMouse;
   
    // Animation
    AnimationState*              m_pAnima;
    std::vector<MovableObject*>  m_vInstances;
    std::vector<AnimationState*> m_vAnimations;
    SceneNode*                   childNode;

    std::vector<String>           managerNames;
    std::vector<InstanceManager*> vInstances;
    std::vector<InstancedEntity*> vEnts;
    std::vector<SceneNode*>       vNodes;
    std::vector<String>           materialNames;
    
    FactoryEntity*                m_pFactory;

    AISystem*                     m_pEntities;

};

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||