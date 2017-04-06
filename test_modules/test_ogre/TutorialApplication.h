/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
 
This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
	  Tutorial Framework
	  http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_
 
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>

 
class TutorialApplication : public Ogre::WindowEventListener, public Ogre::FrameListener {
public:
	/** root application */
	Ogre::Root* mRoot;

	/** resources and plugin info stuff */
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	/** the rendering window */
	Ogre::RenderWindow* mWindow;

	/** the scene manager */
	Ogre::SceneManager* mSceneMgr;

	/** the camera */
	Ogre::Camera* mCamera;

	/** Object Orientated Input System */
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	TutorialApplication(void);
	virtual ~TutorialApplication(void);

 	bool go();

protected:
	/** Ogre::WindowEventListener */
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	/** Ogre::FrameListener */
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};
 
#endif // #ifndef __TutorialApplication_h_