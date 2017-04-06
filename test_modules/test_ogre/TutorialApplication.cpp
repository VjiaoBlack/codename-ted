/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
 
#include "TutorialApplication.h"


 
TutorialApplication::TutorialApplication()
	: mRoot(0),
	  mResourcesCfg(Ogre::StringUtil::BLANK),
	  mPluginsCfg(Ogre::StringUtil::BLANK) {
}

TutorialApplication::~TutorialApplication() {
	/** remove ourself as a window listener */
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}
 
bool TutorialApplication::go() {
	/** load config files */
#ifdef _DEBUG
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif	

	mRoot = new Ogre::Root(mPluginsCfg);

	/** load information from config files */
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locType;

	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements()) {
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it) {
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	/** 
	 * automatically load config if possible
	 *
	 * you can also get config options with code, 
	 * http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Basic+Tutorial+6&structure=Tutorials
	 * "Configuring the RenderSystem"
	 * but I don't feel like doing it right now
	 */
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())) {
		return false;
	}

	/** create RenderWindow instance */
	mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

	/** initialize resources */
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	/** initialize scenemanager */
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	/** initialize camera */
	mCamera = mSceneMgr->createCamera("MainCam");

	mCamera->setPosition(0, 0, 80);
	mCamera->lookAt(0, 0, -300);
	mCamera->setNearClipDistance(5);

	/** setup viewport */
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) /
			Ogre::Real(vp->getActualHeight()));

	/** setup entities */
	Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

	Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	/** Initialize OIS */
	Ogre::LogManager::getSingletonPtr()->logMessage("** Initializing OIS ***");
	OIS::ParamList p1;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	p1.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem(p1);

	/** 
	 * setup keyboard and mouse
	 * second param is false because it isn't buffered
	 */
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));

	/** register as event listener */
	windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	// /** old render loop */
	// while (true) {
	// 	Ogre::WindowEventUtilities::messagePump();
	// 	if (mWindow->isClosed()) {
	// 		return false;
	// 	}
	// 	 if (!mRoot->renderOneFrame()) {
	// 	 	return false;
	// 	 }
	// }
	
	/** new render loop 	 */
	mRoot->addFrameListener(this);
	mRoot->startRendering();

	return true;
}

/** adjust mouse clipping area */
void TutorialApplication::windowResized(Ogre::RenderWindow* rw) {
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

/** unattach OIS before window shutdown (VERY IMPORTANT) */
void TutorialApplication::windowClosed(Ogre::RenderWindow* rw) {
	/** only close for window that created OIS */
	if (rw == mWindow) {
		if (mInputManager) {
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if (mWindow->isClosed()) {
		return false;
	}

	/** need to capture / update each device */
	mKeyboard->capture();
	mMouse->capture();

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {
	// Create application object
	TutorialApplication app;

	try {
		app.go();
	} catch( Ogre::Exception& e ) {
		std::cerr << "An exception has occurred: " <<
			e.getFullDescription().c_str() << std::endl;
	}

	return 0;
}
