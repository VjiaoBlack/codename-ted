/**
 * PiratePanic.cpp
 *
 * Holds the main stuff
 */

#include "PiratePanic.h"

BasicApp::BasicApp()
    : mShutdown(false)
    , mRoot(0)
    , mCamera(0)
    , mSceneMgr(0)
    , mWindow(0)
    , mResourcesCfg(Ogre::StringUtil::BLANK)
    , mPluginsCfg(Ogre::StringUtil::BLANK)
    , mCameraMan(0)
    , mRenderer(0)
    , mMouse(0)
    , mKeyboard(0)
    , mInputMgr(0)
    , mTerrainsImported(false)
    , mTerrainGroup(0)
    , mTerrainGlobals(0)
    , mRotSpd(0.1)
    , mLMouseDown(false)
    , mRMouseDown(false)
    , mCurObject(0)
    , mHeight(0)
    , mBike(0, 0, 100)
    , mBikeObject(0) 
    , mGameLoopClient(NULL)
    , mGameState()
    , mOtherPlayers() {

    // create more pirates 
    mGameState.add_player(0,0,0,true,"Clone");
}

BasicApp::~BasicApp() {
    if (mCameraMan) delete mCameraMan;
 
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

    delete mRoot;
}
 
void BasicApp::go() {
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
 
    if (!setup())
        return;
 
    mRoot->startRendering();
 
    destroyScene();
}
 
bool BasicApp::frameRenderingQueued(const Ogre::FrameEvent& fe) {
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mShutdown = true;
 
    if (mShutdown)
        return false;
 
    if (mWindow->isClosed())
        return false;

    // Switch SkyX presets 
    if (mKeyboard->isKeyDown(OIS::KC_1)) {
        mCurrentPreset = 0;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    } else if (mKeyboard->isKeyDown(OIS::KC_2)) {
        mCurrentPreset = 1;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    } else if (mKeyboard->isKeyDown(OIS::KC_3)) {
        mCurrentPreset = 2;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    } else if (mKeyboard->isKeyDown(OIS::KC_4)) {
        mCurrentPreset = 3;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    } else if (mKeyboard->isKeyDown(OIS::KC_5)) {
        mCurrentPreset = 4;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    } else if (mKeyboard->isKeyDown(OIS::KC_6)) {
        mCurrentPreset = 5;
        setPreset(mPresets[mCurrentPreset], mCamera); 
        Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
        mSceneMgr->getLight("SceneLight")->setDirection(-sunDir);
        mSceneMgr->getLight("SceneLight")->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                                mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);

        mTerrainGlobals->setLightMapDirection(mSceneMgr->getLight("SceneLight")->getDerivedDirection());
        mTerrainGlobals->setCompositeMapDiffuse(mSceneMgr->getLight("SceneLight")->getDiffuseColour());

        mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
        mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());
        
        mTerrainGroup->getTerrain(0,0)->dirty();
        mTerrainGroup->getTerrain(0,0)->update();

        mTerrainGroup->getTerrain(0,0)->dirtyLightmap();
        mTerrainGroup->getTerrain(0,0)->updateDerivedData();
    }

    mBike.processInput(mKeyboard);
    mBike.update();

    // Update Hydrax
    mHydrax->update(fe.timeSinceLastFrame);

    mKeyboard->capture();
    mMouse->capture();

    // TODO make camera respond differently to keys
    mCameraMan->frameRenderingQueued(fe);
        
    // CLIENT CODE
    std::vector<int> pressedKeysToSend;
    mKeyboard->copyKeyStates(mCurPressedKeys);
    for (int i = 0; i < 256; i++) {
        if (mCurPressedKeys[i]) {
            pressedKeysToSend.push_back(i);
        }
    }
    
    if (pressedKeysToSend.size() > 0) {
        mGameLoopClient->send_keystrokes(pressedKeysToSend, 20);
    }

    PiGameState state = mGameLoopClient->get_gamestate();
    printf("%d\n", state.players.size());
    for (auto player = state.players.begin(); player != state.players.end(); player++) {
        // id = player.first;
        
        printf("asdf %f\n", mGameState.map.merchants[player->first].coord_pos.x);
        printf("ijsdfadsfkl %d\n", state.map.merchants.size());
        printf("ijkl %f\n", state.map.merchants[0].coord_pos.x);


        mGameState.map.merchants[player->first].coord_pos.x = state.map.merchants[player->first].coord_pos.x;
        mGameState.map.merchants[player->first].coord_pos.y = state.map.merchants[player->first].coord_pos.y;
        // mGameState = state;

        mGameState.map.merchants[player->first].coord_pos.x *= 50.0;
        mGameState.map.merchants[player->first].coord_pos.y *= 50.0;
        printf("player->first: %d... %f\n", player->first, mGameState.map.merchants[player->first].coord_pos.x);
    }


    // update game state
    CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);
 
    CEGUI::Window* terrainLabel =
        CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
            "TerrainLabel");
 
    if (mTerrainGroup->isDerivedDataUpdateInProgress()) {
        if (mTerrainsImported)
            terrainLabel->setText("Building terrain, please wait...");
        else
            terrainLabel->setText("Updating textures, patience...");
    } else {
        terrainLabel->setText("");

        if (mTerrainsImported) {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }

    handleCameraCollision();

    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    long ns = spec.tv_sec * 1000000000 + spec.tv_nsec;
    double nsf = (double) ns;
    nsf /= 1000000000.0f;

    // whoo hoo i figured out quaternions yay neverforgetti pls
    mBikeObject->setPosition(Ogre::Vector3(mBike.x, -10.0f + mHydrax->getHeigth(Ogre::Vector2(mBike.x, mBike.y)) + (2.0f * sin(nsf * 2.0f)), mBike.y));

    // fake bob
    // TODO: create REAL bob physics WOW
    mBikeObject->setOrientation(Ogre::Quaternion(
            (double) cos(mBike.dir / (2.0f)),   
            (double) 0.05f * cos(nsf * 2.0f) * cos(mBike.dir / 2.0f), 
            (double) sin(mBike.dir / (2.0f)), 
            (double) 0.05f * cos(nsf * 2.0f) * cos(3.14f / 2.0f + mBike.dir / 2.0f)));


    for (auto id_ent : mOtherPlayers) {

        printf("why %f\n", mGameState.map.merchants[id_ent.first].coord_pos.x);

        id_ent.second->setPosition(Ogre::Vector3(mGameState.map.merchants[id_ent.first].coord_pos.x,  
                                                    -10.0f + mHydrax->getHeigth(Ogre::Vector2(mGameState.map.merchants[id_ent.first].coord_pos.x,
                                                                                              mGameState.map.merchants[id_ent.first].coord_pos.y)) + (2.0f * sin((nsf + id_ent.first * 100.0f) * 2.0f)),
                                                    mGameState.map.merchants[id_ent.first].coord_pos.y));

        id_ent.second->setOrientation(Ogre::Quaternion(
                (double) cos(0.0f / (2.0f)),   
                (double) 0.05f * cos((nsf + id_ent.first * 100.0f) * 2.0f) * cos(0.0f / 2.0f), 
                (double) sin(0.0f / (2.0f)), 
                (double) 0.05f * cos((nsf + id_ent.first * 100.0f) * 2.0f) * cos(3.14f / 2.0f + 0.0f / 2.0f)));
    }

    // TODO quikfix for water color circle bug weird
    // Ogre::Vector3 camDir = mCamera->getDirection();
    // camDir.y = -camDir.y;
    // camDir.x = -camDir.x;
    // camDir.z = -camDir.z;

    // mHydrax->setWaterColor(mSkyX->getAtmosphereManager()->getColorAt(camDir));

    float lastFPS = mWindow->getLastFPS();
    terrainLabel->setText("FPS: " + to_string(lastFPS));

    return true;
}
 
bool BasicApp::keyPressed(const OIS::KeyEvent& ke) {
    mCameraMan->injectKeyDown(ke);

    return true;
}
 
bool BasicApp::keyReleased(const OIS::KeyEvent& ke) {
    mCameraMan->injectKeyUp(ke);
 
    return true;
}
 
bool BasicApp::mouseMoved(const OIS::MouseEvent& me) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(me.state.X.rel, me.state.Y.rel);
 
    if (mLMouseDown) {
        CEGUI::Vector2f mousePos = context.getMouseCursor().getPosition();
 
        Ogre::Ray mouseRay =
            mCamera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height));
 
        Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);
 
        if (result.terrain)
            mCurObject->setPosition(result.position + Ogre::Vector3(0.0, 800.0, 0.0));
    } else if (mRMouseDown) {
        mCamera->yaw(Ogre::Degree(-me.state.X.rel * mRotSpd));
        mCamera->pitch(Ogre::Degree(-me.state.Y.rel * mRotSpd));
    }
 
    return true;
}
 
// Helper function for mouse events
CEGUI::MouseButton convertButton(OIS::MouseButtonID id) {
    switch (id) {
        case OIS::MB_Left:
            return CEGUI::LeftButton;
        case OIS::MB_Right:
            return CEGUI::RightButton;
        case OIS::MB_Middle:
            return CEGUI::MiddleButton;
        default:
            return CEGUI::LeftButton;
    }
}
 
bool BasicApp::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseButtonDown(convertButton(id));
 
    if (id == OIS::MB_Left) {
        mLMouseDown = true;
 
        CEGUI::Vector2f mousePos = context.getMouseCursor().getPosition();
 
        Ogre::Ray mouseRay =
            mCamera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height));
 
        Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);
 
    } else if (id == OIS::MB_Right) {
        mRMouseDown = true;
        context.getMouseCursor().hide();
    }
 
    return true;
}
 
bool BasicApp::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseButtonUp(convertButton(id));
 
    if (id == OIS::MB_Left) {
        mLMouseDown = false;
    } else if (id == OIS::MB_Right) {
        mRMouseDown = false;
        context.getMouseCursor().show();
    }
 
    return true;
}
 
void BasicApp::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState& ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
void BasicApp::windowClosed(Ogre::RenderWindow* rw) {
    if (rw == mWindow) {
        if (mInputMgr) {
            mInputMgr->destroyInputObject(mMouse);
            mInputMgr->destroyInputObject(mKeyboard);
 
            OIS::InputManager::destroyInputSystem(mInputMgr);
            mInputMgr = 0;
        }
    }
}
 
bool BasicApp::setup() {
    mRoot = new Ogre::Root(mPluginsCfg);
 
    setupResources();
 
    if (!configure())
        return false;
 
    chooseSceneManager();
    createCamera();
    createViewports();
 
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
 
    createResourceListener();
    loadResources();
 
    setupCEGUI();
 
    createScene();
 
    createFrameListener();

    // setup UDP client 
    boost::asio::io_service io_service;
    mGameLoopClient = new UDPClient(io_service, K_SERVER_STRING, K_PORT_STRING);
 
    return true;
}
 
bool BasicApp::configure() {
    if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())) {
        return false;
    }
 
    mWindow = mRoot->initialise(true, "ITutorial");
 
    return true;
}
 
void BasicApp::chooseSceneManager() {
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}
 
void BasicApp::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
 
    mCamera->setPosition(Ogre::Vector3(0, 0, 80));
    mCamera->lookAt(Ogre::Vector3(0, 0, -300));
    mCamera->setNearClipDistance(5);
 
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
 
void BasicApp::createScene() {
    mCamera->setPosition(40, 900, 580);
    mCamera->pitch(Ogre::Degree(-30));
    mCamera->yaw(Ogre::Degree(-45));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
    // mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);


    // mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

    /**
     * Create SkyX
     */
    mBasicController = new SkyX::BasicController();
    mSkyX = new SkyX::SkyX(mSceneMgr, mBasicController);
    mSkyX->create();

    mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,-1));
    
    // Register SkyX listeners
    mRoot->addFrameListener(mSkyX);
    mWindow->addListener(mSkyX);

    setPreset(mPresets[mCurrentPreset], mCamera);

    /** 
     * Setup Terrain
     */
    Ogre::Vector3 sunDir = mSkyX->getController()->getSunDirection();
 
    Ogre::Light* light = mSceneMgr->createLight("SceneLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(-sunDir);
    light->setDiffuseColour(mSkyX->getVCloudsManager()->getVClouds()->getSunColor().x,
                            mSkyX->getVCloudsManager()->getVClouds()->getSunColor().y,
                            mSkyX->getVCloudsManager()->getVClouds()->getSunColor().z);
    light->setSpecularColour(Ogre::ColourValue(0.2, 0.18, 0.18));
    // light->setCastShadows(true);

    // mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);


    setupTerrain(light);




    /**
     * Setup CEGUI
     */
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");
 
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);

    /**
     * Setup HydraX
     */
    mHydrax = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

    Hydrax::Module::ProjectedGrid *mModule 
        = new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
                                            mHydrax,
                                            // Noise module
                                            new Hydrax::Noise::Perlin(/*Generic one*/),
                                            // Base plane
                                            Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
                                            // Normal mode
                                            Hydrax::MaterialManager::NM_VERTEX,
                                            // Projected grid options
                                            Hydrax::Module::ProjectedGrid::Options(/*264 /*Generic one*/));

    mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));
    mHydrax->loadCfg("HydraxDemo.hdx");
    mHydrax->create();
    mHydrax->getMaterialManager()->addDepthTechnique(
        mTerrainGroup->getTerrain(0, 0)->getMaterial()
        ->createTechnique());
    
    // mHydrax->getMaterialManager()->addDepthTechnique( 
    //     static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Island")) 
    //     ->createTechnique());
// 
    mHydrax->setSunPosition(mSkyX->getController()->getSunDirection()*mCamera->getFarClipDistance()*50.0f);
    mHydrax->setSunColor(mSkyX->getVCloudsManager()->getVClouds()->getSunColor());

    // TODO: quikfix for water color circle bug weird
    // Ogre::Vector3 camDir = mCamera->getDirection();
    // camDir.y = -camDir.y;
    // camDir.x = -camDir.x;
    // camDir.z = -camDir.z;
    // mHydrax->setWaterColor(mSkyX->getAtmosphereManager()->getColorAt(camDir));

    /**
     * Setup Entities
     */
    Ogre::Entity* ent = mSceneMgr->createEntity("Cube.mesh");
    // ent->setMaterialName("Examples/CelShading");
    ent->setMaterialName("Ship/Material");
 
    mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCurObject->setPosition(Ogre::Vector3(0.0, 800.0, 0.0));
    mCurObject->setScale(30.0, 30.0, 30.0);
    mCurObject->attachObject(ent);


    mBikeObject = mCurObject;

    // for (int i = 0; i < mGameState.map.pirates.size(); i++) {
    //     Ogre::Entity* ent = mSceneMgr->createEntity("Cube.mesh");
    //     ent->setMaterialName("Ship/Material");

    //     mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    //     mCurObject->setPosition(Ogre::Vector3(mGameState.players[id_ent.first].x, 800.0, mGameState.players[id_ent.first].y));
    //     mCurObject->setScale(30.0, 30.0, 30.0);
    //     mCurObject->attachObject(ent);


    //     mOtherPlayers[i] = mCurObject;
    // }

    for (auto id_player : mGameState.players) {
        Ogre::Entity* ent = mSceneMgr->createEntity("Cube.mesh");
        ent->setMaterialName("Ship/Material");

        mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        mCurObject->setPosition(Ogre::Vector3(mGameState.map.merchants[id_player.first].coord_pos.x, 800.0, mGameState.map.merchants[id_player.first].coord_pos.y));
        mCurObject->setScale(30.0, 30.0, 30.0);
        mCurObject->attachObject(ent);

        mOtherPlayers[id_player.first] = mCurObject;
    }

}
 
void BasicApp::destroyScene() {
}
 
void BasicApp::createFrameListener() {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
 
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputMgr = OIS::InputManager::createInputSystem(pl);
 
    mKeyboard = static_cast<OIS::Keyboard*>(
        mInputMgr->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(
        mInputMgr->createInputObject(OIS::OISMouse, true));
 
    mKeyboard->setEventCallback(this);
    mMouse->setEventCallback(this);
 
    windowResized(mWindow);
 
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mRoot->addFrameListener(this);
 
    Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
}
 
void BasicApp::createViewports() {
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
 
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));
}
 
void BasicApp::setupResources() {
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
 
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
 
    while (secIt.hasMoreElements()) {
        secName = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator setIt;
 
        for (setIt = settings->begin(); setIt != settings->end(); ++setIt) {
            typeName = setIt->first;
            archName = setIt->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        archName, typeName, secName);
        }
    }
}
 
void BasicApp::createResourceListener() {
}
 
void BasicApp::loadResources() {
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
 
bool BasicApp::setupCEGUI() {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");
 
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
 
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
 
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
 
    context.setDefaultFont("DejaVuSans-10");
    context.getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
 
    Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
 
    return true;
}
 
void BasicApp::setupTerrain(Ogre::Light* light) {
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
        mSceneMgr,
        Ogre::Terrain::ALIGN_X_Z, 513, 36000.0);
    mTerrainGroup->setFilenameConvention(Ogre::String("terrain"),Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
    configureTerrainDefaults(light);
 
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
 
    mTerrainGroup->loadAllTerrains(true);
 
    if (mTerrainsImported) {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
 
        while (ti.hasMoreElements()) {
            Ogre::Terrain* terrain = ti.getNext()->instance;
            initBlendMaps(terrain);
        }
    }
 
    mTerrainGroup->freeTemporaryResources();

    // Toggle LOD updating
    // mTerrainGroup->setAutoUpdateLod(NULL);

}
 
// Terrain helper function
void getTerrainImage(bool flip_x, bool flip_y, Ogre::Image& img) {
    img.load("victor2_terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 
    if (flip_x)
        img.flipAroundY();
    if (flip_y)
        img.flipAroundX();
}
 
void BasicApp::defineTerrain(long x, long y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
 
    bool resource_exists =
        Ogre::ResourceGroupManager::getSingleton().resourceExists(
            mTerrainGroup->getResourceGroup(), filename);
 
    if (resource_exists)
        mTerrainGroup->defineTerrain(x, y);
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}
 
void BasicApp::initBlendMaps(Ogre::Terrain* terrain) {
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap2 = terrain->getLayerBlendMap(2);

    float* blend1 = blendMap1->getBlendPointer();
    float* blend2 = blendMap2->getBlendPointer();

    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
            Ogre::Real tx, ty;
 
            blendMap1->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);

            *blend2++ = Ogre::Math::Clamp((50.0f + 800.0f - height) / 80.0f, 0.0f, 1.0f); //dirt;
            *blend1++ = Ogre::Math::Clamp((50.0f + 940.0f - height) / 100.0f, 0.0f, 1.0f); //grass; 
        }
    }
    blendMap1->dirty();
    blendMap2->dirty();
    blendMap1->update();
    blendMap2->update();
}
 
void BasicApp::configureTerrainDefaults(Ogre::Light* light) {
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);

    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
    importData.terrainSize = 513;
    importData.worldSize = 36000.0f;
    importData.inputScale = 2000;
    importData.minBatchSize = 33; //33
    importData.maxBatchSize = 65; // 65

    importData.layerList.resize(3);

    importData.layerList[2].worldSize = 50;
    importData.layerList[2].textureNames.push_back(
        "dirt_grayrocky_diffusespecular.dds");
    importData.layerList[2].textureNames.push_back(
        "dirt_grayrocky_normalheight.dds");

    importData.layerList[0].worldSize = 250;
    importData.layerList[0].textureNames.push_back(
        "growth_weirdfungus-03_diffusespecular.dds");
    importData.layerList[0].textureNames.push_back(
        "growth_weirdfungus-03_normalheight.dds");

    importData.layerList[1].worldSize = 80;
    importData.layerList[1].textureNames.push_back(
        "grass_green-01_diffusespecular.dds");
    importData.layerList[1].textureNames.push_back(
        "grass_green-01_normalheight.dds");
}
 
void BasicApp::handleCameraCollision() {
    Ogre::Vector3 camPos = mCamera->getPosition();
    Ogre::Ray camRay(
        Ogre::Vector3(camPos.x, 5000.0, camPos.z),
        Ogre::Vector3::NEGATIVE_UNIT_Y);
 
    Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(camRay);
 
    if (result.terrain) {
        Ogre::Real terrainHeight = result.position.y;
 
        if (camPos.y < (terrainHeight + 10.0))
            mCamera->setPosition(camPos.x, terrainHeight + 10.0, camPos.z);
    }
}
 
int main(int argc, char *argv[]) {
    BasicApp app;
 
    try {
        app.go();
    } catch(Ogre::Exception& e) {
        std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
    }
 
    return 0;
}
