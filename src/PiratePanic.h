#ifndef BASICAPP_H
#define BASICAPP_H
 
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreCommon.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreEntity.h>
#include <OgreFrameListener.h>
#include <OgreManualObject.h>
#include <OgreMaterial.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
 
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
 
#include <SdkCameraMan.h>

#include "hydrax/Hydrax.h"
#include "hydrax/Noise/Perlin/Perlin.h"
#include "hydrax/Modules/ProjectedGrid/ProjectedGrid.h"

#include "skyx/SkyX.h"
#include "skyx/AtmosphereManager.h"

#include "PiSkyX.h"
#include "TdBike.h"

#include "../game_server/client.hpp"

#include <algorithm>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#define _def_SkyBoxNum 3

#define K_SERVER_STRING "localhost"
#define K_PORT_STRING "8888"

// Hydrax pointer
Hydrax::Hydrax *mHydrax = 0;

Ogre::String mSkyBoxes[_def_SkyBoxNum] = 
           {"Sky/ClubTropicana",
            "Sky/EarlyMorning",
            "Sky/Clouds"};

Ogre::Vector3 mSunPosition[_def_SkyBoxNum] = 
           {Ogre::Vector3(0,10000,0),
            Ogre::Vector3(0,10000,90000),
            Ogre::Vector3(0,10000,0)};

Ogre::Vector3 mSunColor[_def_SkyBoxNum] = 
           {Ogre::Vector3(1, 0.9, 0.6),
            Ogre::Vector3(1,0.6,0.4),
            Ogre::Vector3(0.45,0.45,0.45)};

int mCurrentSkyBox = 0;



class BasicApp
        : public Ogre::WindowEventListener
        , public Ogre::FrameListener
        , public OIS::KeyListener
        , public OIS::MouseListener {
public:
    BasicApp();
    ~BasicApp();
 
    void go();
 
private:
    bool mShutdown;
 
    Ogre::Root* mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
 
    OgreBites::SdkCameraMan* mCameraMan;
 
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
 
    virtual bool keyPressed(const OIS::KeyEvent& ke);
    virtual bool keyReleased(const OIS::KeyEvent& ke);
 
    virtual bool mouseMoved(const OIS::MouseEvent& me);
    virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);
 
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
 
    bool setup();
    bool configure();
    void chooseSceneManager();
    void createCamera();
    void createScene();
    void destroyScene();
    void createFrameListener();
    void createViewports();
    void setupResources();
    void createResourceListener();
    void loadResources();
 
    // CEGUI
    CEGUI::OgreRenderer* mRenderer;
 
    bool setupCEGUI();
 
    // OIS
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;
    OIS::InputManager* mInputMgr;
 
    // Terrain
    void setupTerrain(Ogre::Light* light);
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
 
    bool mTerrainsImported;
 
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
 
    void handleCameraCollision();
 
    float mRotSpd;
    bool mLMouseDown, mRMouseDown;
 
    Ogre::SceneNode* mCurObject;
    Ogre::SceneNode* mBikeObject;

    float mHeight;
    TdBike mBike;

    UDPClient* mGameLoopClient;
};





















 
#endif