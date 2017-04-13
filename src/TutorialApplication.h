#ifndef BASICAPP_H
#define BASICAPP_H
 
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreEntity.h>
#include <OgreFrameListener.h>
#include <OgreManualObject.h>
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
 




#define PI 3.1415926535897

class TdBike {
public:
    float x, y;
    float wheel; // of front wheel
    float dir;
    float r;

    float mass;
    float vel;

    TdBike(float x, float y, float mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;

        wheel = 0.0;
        dir = 0.0;
        r = 40.0;
        vel = 0.0;
    }

    /** 
     * update
     * updates physics of bike
     */
    void update() {
        // printf("%f\n", vel);
        float change = 0.02 * (wheel) * fabs(vel);
        vel -= 0.5f * fabs(change);
        dir += change;
        wheel -= change;

        float vx = vel * sin(dir + wheel);
        float vy = vel * cos(dir + wheel);

        /** check if head of bike is in bounds */
        // if (x + vx >= 25.0 && x + vx <= 1024.0 - 25.0) {
            x += vx;
        // } else {
        //     vel = fabs(vy);
        // }

        // if (y + vy >= 25.0 && y + vy <= 768.0 - 25.0) {
            y += vy;
        // } else {
        //     vel = fabs(vx);
        // }
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
};





















 
#endif