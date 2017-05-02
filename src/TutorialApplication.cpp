#include "TutorialApplication.h"

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
    , mBikeObject(0)  {
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

    float dx = 0.0f;

    if (mKeyboard->isKeyDown(OIS::KC_L)) {
        dx = 100.0f;
    }

    if (mKeyboard->isKeyDown(OIS::KC_J)) {
        if (mBike.wheel < 1.6f) {
            mBike.wheel += 0.02;
        }
    }
    if (mKeyboard->isKeyDown(OIS::KC_L)) {
        if (mBike.wheel > -1.6f) {
            mBike.wheel -= 0.02;
        }
    }

    if (mKeyboard->isKeyDown(OIS::KC_I)) {
        if (mBike.vel < 2.0) {
            mBike.vel += 0.01;
        }
    } else {
        if (mBike.vel > 0.0005) {
            mBike.vel -= 0.0005;
        } else {
            mBike.vel = 0;
        }
    }

    if (mKeyboard->isKeyDown(OIS::KC_K)) {
        if (mBike.vel > 0.0) {
            mBike.vel -= 0.0095;
        }
    }

    mBike.update();



 
    mKeyboard->capture();
    mMouse->capture();
 
    mCameraMan->frameRenderingQueued(fe);
 
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

    mBikeObject->setPosition(Ogre::Vector3(mBike.x, 300.0 + mHeight, mBike.y));
    mBikeObject->setOrientation(Ogre::Quaternion(Ogre::Radian(mBike.dir + 1.5f), Ogre::Vector3(0, 1, 0)));


    return true;
}
 
bool BasicApp::keyPressed(const OIS::KeyEvent& ke) {
    mCameraMan->injectKeyDown(ke);

    return true;
}
 
bool BasicApp::keyReleased(const OIS::KeyEvent& ke) {
    // CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    // context.injectKeyUp((CEGUI::Key::Scan)ke.key);
 
    mCameraMan->injectKeyUp(ke);
 
    return true;
}
 
bool BasicApp::mouseMoved(const OIS::MouseEvent& me) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(me.state.X.rel, me.state.Y.rel);
 
    // mCameraMan->injectMouseMove(me);
 
    if (mLMouseDown) {
        CEGUI::Vector2f mousePos = context.getMouseCursor().getPosition();
 
        Ogre::Ray mouseRay =
            mCamera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height));
 
        Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);
 
        if (result.terrain)
            mCurObject->setPosition(result.position + Ogre::Vector3(0.0, 20.0, 0.0));
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
 
    // mCameraMan->injectMouseDown(me, id);
 
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
 
    // mCameraMan->injectMouseUp(me, id);
 
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
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
    // mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 13);
    mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 400, true);
 
    mCamera->setPosition(40, 100, 580);
    mCamera->pitch(Ogre::Degree(-30));
    mCamera->yaw(Ogre::Degree(-45));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);
 
    Ogre::Vector3 lightDir(0.55, 0.3, 0.75);
    lightDir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("SceneLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightDir);
    light->setDiffuseColour(Ogre::ColourValue(0.4, 0.4, 0.4));
    light->setSpecularColour(Ogre::ColourValue(0.2, 0.2, 0.2));
 
    setupTerrain(light);
 
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");
 
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);

    /** Create the "boat" we will be using - a fish. */
    Ogre::Entity* ent = mSceneMgr->createEntity("fish.mesh");
 
    mCurObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCurObject->setPosition(Ogre::Vector3(0.0, 100.0, 0.0));
    mCurObject->setScale(10.0, 10.0, 10.0);
    mCurObject->attachObject(ent);

    mBikeObject = mCurObject;
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
        Ogre::Terrain::ALIGN_X_Z, 513, 12000.0);
    mTerrainGroup->setFilenameConvention(
        Ogre::String("terrain"),
        Ogre::String("dat"));
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
}
 
// Terrain helper function
void getTerrainImage(bool flip_x, bool flip_y, Ogre::Image& img) {
    img.load("victor_terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 
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
    // Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    // Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    // Ogre::Real minHeight0 = 70;
    // Ogre::Real fadeDist0 = 40;
    // Ogre::Real minHeight1 = 70;
    // Ogre::Real fadeDist1 = 15;
    // float* blend0 = blendMap0->getBlendPointer();
    // float* blend1 = blendMap1->getBlendPointer();
 
    // for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
    //     for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
    //         Ogre::Real tx, ty;
 
    //         blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
    //         Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
    //         Ogre::Real val = (height - minHeight0) / fadeDist0;
    //         val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
    //         *blend0++ = val;
 
    //         val = (height - minHeight1) / fadeDist1;
    //         val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
    //         *blend1++ = val;
    //     }
    // }
 
    // blendMap0->dirty();
    // blendMap1->dirty();
    // blendMap0->update();
    // blendMap1->update();
}
 
void BasicApp::configureTerrainDefaults(Ogre::Light* light) {
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
    Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
    importData.terrainSize = 513;
    importData.worldSize = 12000.0f;
    importData.inputScale = 2000;
    importData.minBatchSize = 33;
    importData.maxBatchSize = 65;
 
    importData.layerList.resize(3);
    // importData.layerList[0].worldSize = 100;
    // importData.layerList[0].textureNames.push_back(
    //     "dirt_grayrocky_diffusespecular.dds");
    // importData.layerList[0].textureNames.push_back(
    //     "dirt_grayrocky_normalheight.dds");
    // importData.layerList[1].worldSize = 30;
    // importData.layerList[1].textureNames.push_back(
    //     "grass_green-01_diffusespecular.dds");
    // importData.layerList[1].textureNames.push_back(
    //     "grass_green-01_normalheight.dds");
    // importData.layerList[2].worldSize = 200;
    // importData.layerList[2].textureNames.push_back(
    //     "growth_weirdfungus-03_diffusespecular.dds");
    // importData.layerList[2].textureNames.push_back(
    //     "growth_weirdfungus-03_normalheight.dds");
    
    importData.layerList[0].worldSize = 12000;
    importData.layerList[0].textureNames.push_back(
        "victor_diffusespecular.dds");
    importData.layerList[0].textureNames.push_back(
        "victor_normalheight.dds");
    importData.layerList[1].worldSize = 12000;
    importData.layerList[1].textureNames.push_back(
        "victor_diffusespecular.dds");
    importData.layerList[1].textureNames.push_back(
        "victor_normalheight.dds");
    importData.layerList[2].worldSize = 12000;
    importData.layerList[2].textureNames.push_back(
        "victor_diffusespecular.dds");
    importData.layerList[2].textureNames.push_back(
        "victor_normalheight.dds");
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
