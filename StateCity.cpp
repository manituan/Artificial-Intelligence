//|||||||||||||||||||||||||||||||||||||||||||||||

#include "StateCity.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

StateCity::StateCity()
{

    m_pCamera = 0;

    m_pLightP = 0;
    m_pLightD_Left = 0;
    m_pLightS = 0;

    m_pMouse = 0;
    m_pKey = 0;
    m_pAnima = 0;

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::enter()
{

    // Create SceneManager
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering StateCity...");
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_EXTERIOR_CLOSE, "GameSceneMgr");
    m_pSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
    // m_pSceneMgr->setWorldGeometry("terrain.cfg");
    
    // BeastMELEE
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastMelee = m_pSceneMgr->createInstanceManager("InstanceBeastMelee", "unit_beast_orc_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);
    
    // UndeadMELEE
    OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadMelee = m_pSceneMgr->createInstanceManager("InstanceUndeadMelee", "undead_zombie_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);
    
    // HumanMELEE
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanMelee = m_pSceneMgr->createInstanceManager("InstanceMelee", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // BeastRANGED
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastRanged = m_pSceneMgr->createInstanceManager("InstanceBeastRanged", "unit_beast_goblin_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // UndeadRANGED
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadRanged = m_pSceneMgr->createInstanceManager("InstanceUndeadRanged", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // HumanRANGED
    OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanRanged = m_pSceneMgr->createInstanceManager("InstanceHumanRanged", "unit_human_level01_elf.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // BeastRIDER
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastRider = m_pSceneMgr->createInstanceManager("InstanceBeastRider", "unit_beast_centaur_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // UndeadRIDER
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadRider = m_pSceneMgr->createInstanceManager("InstanceUndeadRider", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // HumanRIDER
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanRider = m_pSceneMgr->createInstanceManager("InstanceHumanRider", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // UndeadMONSTER
    OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadMonster = m_pSceneMgr->createInstanceManager("InstanceUndeadMonster", "unit_undead_level01_demon.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // BeastMONSTER
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastMonster = m_pSceneMgr->createInstanceManager("InstanceBeastMonster", "unit_beast_minotaur_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // HumanMONSTER
    OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanMonster = m_pSceneMgr->createInstanceManager("InstanceHumanMonster", "unit_human_level01_golem.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // BeastWIZARD
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastWizard = m_pSceneMgr->createInstanceManager("InstanceBeastWizard", "unit_beast_level01_shaman.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // UndeadWIZARD
    OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadWizard = m_pSceneMgr->createInstanceManager("InstanceUndeadWizard", "unit_undead_nicromante_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // HumanWIZARD
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanWizard = m_pSceneMgr->createInstanceManager("InstanceHumanWizard", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // HumanELITE
    OgreFramework::getSingletonPtr()->m_pInstanceMgrHumanElite = m_pSceneMgr->createInstanceManager("InstanceHumanElite", "unit_human_level01_dwarf.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);
    
    // BeastELITE
    OgreFramework::getSingletonPtr()->m_pInstanceMgrBeastElite = m_pSceneMgr->createInstanceManager("InstanceBeastElite", "unit_beast_werewolf_level01.mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);
    
    // UndeadELITE
    // OgreFramework::getSingletonPtr()->m_pInstanceMgrUndeadElite = m_pSceneMgr->createInstanceManager("InstanceUndeadElite", ".mesh", ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::TextureVTF, 300, IM_USEALL);

    // Initialize CameraCity
    m_pCamera = new CameraCity();
    m_pCamera->enter();

    // Initialize the Input
    m_pMouse = new InputMouse();
    m_pKey = new InputKey();
    m_pKey->openXML("keyKOK.xml");
    m_pKey->parseXML();

    // Initialize Lights
    m_pLight_Custom = new LightDir(m_pSceneMgr, "Dir7", ColourValue(1.0f, 1.0f, 1.0f), ColourValue (0.0f, 0.0f, 0.0f), true, true);
    
// TERRAIN
//////////////////////////////////////////////////////////////////////////////////

    m_pCamera->getOgreCamera()->setPosition(Ogre::Vector3(1683, 50, 2116));
    m_pCamera->getOgreCamera()->lookAt(Ogre::Vector3(1963, 50, 1660));
    m_pCamera->getOgreCamera()->setNearClipDistance(0.1);
    m_pCamera->getOgreCamera()->setFarClipDistance(50000);
 
    if (OgreFramework::getSingletonPtr()->m_pRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        m_pCamera->getOgreCamera()->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
 
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

    createScene();

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::createScene()
{
  /*  
    Plane plane;
    plane.normal = Ogre::Vector3::UNIT_Y;
    plane.d = 0;
    MeshManager::getSingleton().createPlane("Myplane",
    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    1500,1500,25,25,true,1,5,5,Ogre::Vector3::UNIT_Z);

    Entity* pPlaneEnt = m_pSceneMgr->createEntity( "plane", "Myplane" );
    pPlaneEnt->setMaterialName("Examples/Rockwall");
    pPlaneEnt->setCastShadows(true);
    m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,0))->attachObject(pPlaneEnt);
       
    // Configure Lights
    m_pLight_Custom->dirDirection(Ogre::Vector3(-0.5, -0.5, 0.0));

    // Configure CameraCity
    m_pCamera->getOgreCamera()->setNearClipDistance(0.1f);
    m_pCamera->getOgreCamera()->setPosition(0, 0, -10);
    m_pCamera->getOgreCamera()->lookAt(0, 0, 10);
    m_pCamera->getOgreCamera()->getViewport()->setClearEveryFrame(true);

    buildGUI();

    FactoryEntity::getSingletonPtr()->newAIBuilding(ATTACK_MODE, BARRACK, 0, Ogre::Vector2(200, 200), Ogre::Vector2(0,0));
    FactoryEntity::getSingletonPtr()->newAIBuilding(DEFENSE_MODE, HALL, 0, Ogre::Vector2(-200, -200), Ogre::Vector2(0,0));
    FactoryEntity::getSingletonPtr()->newAIBuilding(DEFENSE_MODE, WALLS, 0, Ogre::Vector2(0, 0), Ogre::Vector2(0,0));

    EntityCityManager::getSingletonPtr()->Enter();

    for(int i = 0; i<2; i++)
    {
        // ATTACK A BARRACKS DE HALL
        for(int j = 0; j<2; j++)
        {
            for(int k = 0; k<2; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, ELITE, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));      
            }

            for(int k = 2; k<4; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, ELITE, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }

            for(int k = 4; k<6; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, MONSTER, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));
            }

            for(int k = 6; k<8; k++)
            {

                FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, MONSTER, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));      
            }
            for(int k = 8; k<10; k++)
            {

            FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, RANGED, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }
            for(int k = 10; k<12; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(DEFENSE_MODE, RANGED, HUMAN, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().y + j*30),
                Vector2(0, 0));  
            }
        }
    }

    for(int i = 0; i<2; i++)
    {
        // ATTACK A BARRACKS DE HALL
        for(int j = 0; j<2; j++)
        {
            for(int k = 0; k<2; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, MELEE, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));      
            }

            for(int k = 2; k<4; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, MELEE, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }

            for(int k = 4; k<6; k++)
            {
                FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, RANGED, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));
            }
            for(int k = 6; k<8; k++)
            {

                FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, RANGED, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));      
            }
            for(int k = 8; k<10; k++)
            {

            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, RIDER, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }
            for(int k = 10; k<12; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, RIDER, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));  
            }
            for(int k = 12; k<14; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, WIZARD, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }
            for(int k = 14; k<16; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, WIZARD, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));  
            }
            for(int k = 16; k<18; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, MONSTER, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));        
            }
            for(int k = 18; k<20; k++)
            {
            FactoryEntity::getSingletonPtr()->newAIUnit(ATTACK_MODE, MONSTER, BEAST, Vector2(EntityCityManager::getSingletonPtr()->GetType(HALL)[0]->getPosition().x + k*30, EntityCityManager::getSingletonPtr()->GetType(BARRACK)[0]->getPosition().y + j*30),
                Vector2(0, 0));  
            }
        }
    }*/
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::update(double timeSinceLastFrame)
{
/*
    EntityCityManager::getSingletonPtr()->Update(timeSinceLastFrame * 0.03);

    FactoryGraphics::getSingletonPtr()->Update(timeSinceLastFrame);
*/
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

//	getInput();
	
//    m_pCamera->update(timeSinceLastFrame);


}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing StateCity...");
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming StateCity...");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving StateCity...");

    delete m_pCamera;
    delete m_pKey;
    delete m_pMouse;
    delete m_pAnima;

    if (m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if (m_pKey->keyESC())
    {
        shutdown();
        return true;
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::mouseMoved(const OIS::MouseEvent &evt)
{
    m_pMouse->setMouseInUpRegion(false);
    m_pMouse->setMouseInDownRegion(false);
    m_pMouse->setMouseInLeftRegion(false);
    m_pMouse->setMouseInRightRegion(false);

    m_pMouse->mouseMov(evt);

    // Zoom Camera
    if (m_pCamera) m_pCamera->zoomCam(evt.state.Z.rel);

    // Rotate Camera
    if (m_pMouse->getRMouseDown())
        m_pCamera->rotCam(evt.state.X.rel,evt.state.Y.rel);
    else
    {
        float w = (m_pMouse->getWidth() / float(evt.state.width));
        float v = (m_pMouse->getHeight() / float(evt.state.height));

        // Move Camera to Right when cursor is in the Right zone
        if (w>0.95)
        {
            m_pMouse->setMouseInRightRegion(true);
            return true;
        }
        // Move Camera to Left when cursor is in the Left zone
        if (w<0.05)
        {
            m_pMouse->setMouseInLeftRegion(true);
            return true;
        }
        // Move Camera to Down when cursor is in the Down zone
        if (v>0.95)
        {
            m_pMouse->setMouseInDownRegion(true);
            return true;
        }
        // Move Camera to Up when cursor is in the Up zone
        if (v<0.05)
        {
            m_pMouse->setMouseInUpRegion(true);
            return true;
        }
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id))
        return true;

    // Left Button Mouse is pressed
    if (id == OIS::MB_Left)
    {
        onLeftPressed(evt);
        m_pMouse->setLMouseDown(true);
    }
    // Right Button Mouse is pressed
    else if (id == OIS::MB_Right)
    {
        m_pMouse->hide();
        m_pMouse->setRMouseDown(true);
    }

    if (evt.state.buttonDown(OIS::MB_Left)||evt.state.buttonDown(OIS::MB_Right))
    {
        m_pMouse->convertB(id);
    }
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (m_pMouse->mouseRel(evt, id))
    {
        // Left Button Mouse is released
        if (id == OIS::MB_Left)
        {
            m_pMouse->setLMouseDown(false);
        }
        // Right Button Mouse is released
        else if (id == OIS::MB_Right)
        {
           m_pMouse->show();
           m_pMouse->setRMouseDown(false);
        }
    }
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::onLeftPressed(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StateCity::getInput()
{
    m_pCamera->cameraDir();
    m_pCamera->setTranslateVector(Ogre::Vector3::ZERO);

    if (m_pKey->keyLeft() || m_pMouse->getMouseInLeftRegion())
    {
        m_pCamera->moveLeft();
        return true;
    }

    if (m_pKey->keyRight() || m_pMouse->getMouseInRightRegion())
    {
        m_pCamera->moveRight();
        return true;
    }

    if (m_pKey->keyUp() || m_pMouse->getMouseInUpRegion())
    {
        m_pCamera->moveUp();
        return true;
    }

    if (m_pKey->keyDown() || m_pMouse->getMouseInDownRegion())
    {
        m_pCamera->moveDown();
        return true;
    }

    Ogre::Vector3 auxT = m_pCamera->getTranslateVector();
    auxT.normalise();
    m_pCamera->setTranslateVector(auxT);
    m_pCamera->setTranslateVector(m_pCamera->getTranslateVector()*m_pCamera->getMoveScale());

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void StateCity::buildGUI()
{
    
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();

    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("Mode");
    
}