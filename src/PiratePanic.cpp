/**
 * PiratePanic.cpp
 *
 * Holds the main stuff
 */

#include "PiratePanic.h"
#include "PiratePanicImpl.cpp"

bool BasicApp::updateCurrentGameState() {
    PiGameState state = mGameLoopClient->get_gamestate();
    for (auto player = state.players.begin(); player != state.players.end(); player++) {

        // needs to create new merchant
        if (mMerchants.find(player->first) == mMerchants.end()) {
            // create new merchant
            Ogre::Entity* ent = mSceneMgr->createEntity("Cube.mesh");
            ent->setMaterialName("Ship/Material");

            mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
            mCurObject->setPosition(Ogre::Vector3(mMerchants[player->first].coord_pos.x, 800.0, mMerchants[player->first].coord_pos.y));
            mCurObject->setScale(30.0, 30.0, 30.0);
            mCurObject->attachObject(ent);

            mOgreMerchants[player->first] = mCurObject;
            mMerchants[player->first] = PiMerchant();

            // create new merchant gold
            ent = mSceneMgr->createEntity("cube.mesh");
            Ogre::SharedPtr<Ogre::Material> m_pMat = ent->getSubEntity(0)->getMaterial();
            m_pMat->getTechnique(0)->getPass(0)->setAmbient(Ogre::ColourValue(0.8, 0.5, 0.15, 1.0));
            m_pMat->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue(0.8, 0.5, 0.15, 1.0));
            m_pMat->getTechnique(0)->getPass(0)->setEmissive(Ogre::ColourValue(0.8, 0.5, 0.15, 0.2));
            ent->setMaterialName(m_pMat->getName());
            mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
            mCurObject->setPosition(Ogre::Vector3(mMerchants[player->first].coord_pos.x, 930.0, mMerchants[player->first].coord_pos.y));
            mCurObject->setScale(0.3, 0.3, 0.3);
            mCurObject->attachObject(ent);

            mOgreMerchantsGold[player->first] = mCurObject;
        }

        // update game state player coordinates based on
        mMerchants[player->first].coord_pos.x 
                = state.map.merchants[player->first].coord_pos.x - 16000.0;

        mMerchants[player->first].coord_pos.y 
                = state.map.merchants[player->first].coord_pos.y - 16000.0;

        // auto convert to radians
        // TODO fix the -90... its very hard though, be careful
        mMerchants[player->first].orientation
                = (state.map.merchants[player->first].orientation - 90) * (3.1415926535 / 180.0);

                // TODO fake
        state.map.merchants[player->first].goldAmount = 500;
        // drop some gold if gold should be dropped
        if (state.map.merchants[player->first].goldAmount < mMerchants[player->first].goldAmount) {
            // drop gold
            
            // create a gold block
            mGoldBlocks.push_back(PiGoldBlock(mSceneMgr, mMerchants[player->first].coord_pos.x, 930, mMerchants[player->first].coord_pos.y));
            
        }

        mMerchants[player->first].goldAmount = state.map.merchants[player->first].goldAmount;
    }

    mPirate.coord_pos.x = state.map.pirates[0].coord_pos.x - 16000.0;


    mPirate.coord_pos.y = state.map.pirates[0].coord_pos.y - 16000.0;

    // auto convert to radians
    mPirate.orientation = (state.map.pirates[0].orientation - 90) * (3.1415926535 / 180.0);

    printf("Pirate pos: %d, %d\n",state.map.pirates[0].coord_pos.x,state.map.pirates[0].coord_pos.y);
}

bool BasicApp::frameRenderingQueued(const Ogre::FrameEvent& fe) {
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mShutdown = true;
 
    if (mShutdown)
        return false;
 
    if (mWindow->isClosed())
        return false;

    // Update Sun-related variables every once in a while
    mCurFrameCounter++;
    if (mCurFrameCounter > 1000) {
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
        printf("UPDATE %d\n", mCurFrameCounter);
        mCurFrameCounter = 0;
    }

    if (mKeyboard->isKeyDown(OIS::KC_G) && mCurFrameCounter % 10 == 0) {
        mMerchants[mPlayerID].goldAmount = 10000;
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
        mGameLoopClient->send_keystrokes(pressedKeysToSend);
    }

    // Update game state from game loop client
    updateCurrentGameState();

    // change CEGUI system
    CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);
    CEGUI::Window* terrainLabel =
        CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
            "TerrainLabel");
 
    // set build terram etc. labels
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
    mBikeObject->setPosition(Ogre::Vector3(mBike.x, 
                                           -15.0f + mHydrax->getHeigth(Ogre::Vector2(mBike.x, mBike.y)) 
                                                  + (2.0f * sin(nsf * 2.0f)),
                                           mBike.y));

    // fake bob
    // TODO: create REAL bob physics WOW
    mBikeObject->setOrientation(Ogre::Quaternion(
            (double) cos(mBike.dir / (2.0f)),   
            (double) 0.05f * cos(nsf * 2.0f) * cos(mBike.dir / 2.0f), 
            (double) sin(mBike.dir / (2.0f)), 
            (double) 0.05f * cos(nsf * 2.0f) * cos(3.14f / 2.0f + mBike.dir / 2.0f)));


    for (auto id_ent : mOgreMerchants) {
        id_ent.second->setPosition(Ogre::Vector3(mMerchants[id_ent.first].coord_pos.x,  
                                                 -15.0f + mHydrax->getHeigth(Ogre::Vector2(mMerchants[id_ent.first].coord_pos.x,
                                                                                           mMerchants[id_ent.first].coord_pos.y)) 
                                                        + (2.0f * sin((nsf + id_ent.first * 100.0f) * 2.0f)),
                                                 mMerchants[id_ent.first].coord_pos.y));

        id_ent.second->setOrientation(Ogre::Quaternion(
                (double) cos(mMerchants[id_ent.first].orientation / (2.0f)),   
                (double) 0.05f * cos((nsf + id_ent.first * 100.0f) * 2.0f) * cos(mMerchants[id_ent.first].orientation / 2.0f), 
                (double) -sin(mMerchants[id_ent.first].orientation / (2.0f)), 
                (double) 0.05f * cos((nsf + id_ent.first * 100.0f) * 2.0f) * cos(3.14f / 2.0f + mMerchants[id_ent.first].orientation / 2.0f)));

        mOgreMerchantsGold[id_ent.first]->setPosition(Ogre::Vector3(mMerchants[id_ent.first].coord_pos.x- 16000.0,  
                                                      700.0f + 0.3 * mHydrax->getHeigth(Ogre::Vector2(mMerchants[id_ent.first].coord_pos.x,
                                                                                                      mMerchants[id_ent.first].coord_pos.y)),
                                                      mMerchants[id_ent.first].coord_pos.y- 16000.0));
        mOgreMerchantsGold[id_ent.first]->setOrientation(Ogre::Quaternion(
                sin((nsf + id_ent.first * 100.0f) * 0.5f),
                0.0, 
                cos((nsf + id_ent.first * 100.0f) * 0.5f), 
                0.0));
    }

    mOgrePirate->setPosition(Ogre::Vector3(mPirate.coord_pos.x,  
                                           -10.0f + mHydrax->getHeigth(Ogre::Vector2(mPirate.coord_pos.x,
                                                                                     mPirate.coord_pos.y)) 
                                                  + (2.0f * sin((nsf + -1 * 100.0f) * 2.0f)),
                                           mPirate.coord_pos.y));

    mOgrePirate->setOrientation(Ogre::Quaternion(
            (double) cos(mPirate.orientation / (2.0f)),   
            (double) 0.05f * cos((nsf + -1 * 100.0f) * 2.0f) * cos(mPirate.orientation/ 2.0f), 
            (double) -sin(mPirate.orientation / (2.0f)), 
            (double) 0.05f * cos((nsf + -1 * 100.0f) * 2.0f) * cos(3.14f / 2.0f + mPirate.orientation / 2.0f)));

    for (auto it = mGoldBlocks.begin(); it != mGoldBlocks.end(); it++) {
        if (!it->update(mSceneMgr)) {

            std::iter_swap(it, mGoldBlocks.end() - 1);
            mGoldBlocks.erase(mGoldBlocks.end() - 1);

            it = mGoldBlocks.begin();

            if (mGoldBlocks.size() == 0) {
                break;
            }

        }
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
 

 
void BasicApp::createScene() {
    mCamera->setPosition(-17000, 1200, -17000);
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
    // light->setSpecularColour(Ogre::ColourValue(0.2, 0.18, 0.18));
    light->setSpecularColour(Ogre::ColourValue(0.5, 0.5, 0.5));
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


    //     mOgreMerchants[i] = mCurObject;
    // }

    for (auto id_merchant : mMerchants) {
        Ogre::Entity* ent = mSceneMgr->createEntity("Cube.mesh");
        ent->setMaterialName("Ship/Material");

        mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        mCurObject->setPosition(Ogre::Vector3(mMerchants[id_merchant.first].coord_pos.x, 800.0, mMerchants[id_merchant.first].coord_pos.y));
        mCurObject->setScale(30.0, 30.0, 30.0);
        mCurObject->attachObject(ent);

        mOgreMerchants[id_merchant.first] = mCurObject;
    }

}
