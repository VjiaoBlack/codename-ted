
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
#include "../game_server/game_structs.hpp"

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <map>

#define _def_SkyBoxNum 3

// #define K_SERVER_STRING "10.150.110.106"
#define K_SERVER_STRING "localhost"
#define K_PORT_STRING "8888"

// Hydrax pointer
Hydrax::Hydrax *mHydrax = 0;

int mCurrentSkyBox = 0;

char mCurPressedKeys[256] = {};

class PiGoldBlock {
public:
    Ogre::SceneNode* mSceneNode;
    double vx;
    double vy;
    double vz;
    double x;
    double y;
    double z;

    PiGoldBlock(Ogre::SceneManager* sceneMgr, double _x, double _y, double _z)
        : vx(1.0), vy(1.0), vz(1.0), x(_x), y(_y), z(_z) {

        float theta = (float) (rand() % 360) * (3.1415926535) / (180.0);

        vx = 0.1 * sin(theta);
        vz = 0.1 * cos(theta);

        Ogre::Entity* ent = sceneMgr->createEntity("sphere.mesh");
        Ogre::SharedPtr<Ogre::Material> m_pMat = ent->getSubEntity(0)->getMaterial();
        m_pMat->getTechnique(0)->getPass(0)->setAmbient(Ogre::ColourValue(0.8, 0.4, 0.0, 1.0));
        m_pMat->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue(0.8, 0.4, 0.0, 1.0));
        m_pMat->getTechnique(0)->getPass(0)->setEmissive(Ogre::ColourValue(0.8, 0.4, 0.0, 0.2));
        ent->setMaterialName(m_pMat->getName());
        mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
        mSceneNode->setPosition(Ogre::Vector3(x, y, z));

        mSceneNode->setScale(0.03, 0.03, 0.03);
        mSceneNode->attachObject(ent);
    }

    bool update(Ogre::SceneManager* sceneMgr) {
        vy -= 0.02f;
        vy *= 0.99;

        x += vx;
        y += vy;
        z += vz;
        mSceneNode->setPosition(Ogre::Vector3(x, y, z));

        if (y < 600.0) {
            mSceneNode->removeAndDestroyAllChildren();
            sceneMgr->destroySceneNode(mSceneNode);

            return false;
        } else {
            return true;
        }
    }       
};


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

    bool updateCurrentGameState();

 
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

    // server client code stuff
    UDPClient* mGameLoopClient;
    TCPClient* mGameRegistrationClient;
    int mPlayerID;

    std::map<int, PiMerchant> mMerchants;
    PiMerchant mPlayerMerchant;

    // TODO make these pointers... holy FUCK
    std::map<int, Ogre::SceneNode*> mOgreMerchants;
    std::map<int, Ogre::SceneNode*> mOgreMerchantsGold;

    std::vector<PiGoldBlock> mGoldBlocks;

    PiPirate mPirate;
    Ogre::SceneNode* mOgrePirate;

    // for sky update shadows
    int mCurFrameCounter;

    // gold blocks

    // broken
    // PiGameState mGameState;
};


#endif