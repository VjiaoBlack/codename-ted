#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
 
#include "BaseApplication.h"
 
 #ifndef __TutorialApplication_h_

class TutorialApplication : public BaseApplication
{
public:
  TutorialApplication();
  virtual ~TutorialApplication();
 
protected:
  virtual void createScene();
  virtual void createFrameListener();
  virtual void destroyScene();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
 
private:
  void defineTerrain(long x, long y);
  void initBlendMaps(Ogre::Terrain* terrain);
  void configureTerrainDefaults(Ogre::Light* light);
 
  bool mTerrainsImported;
  Ogre::TerrainGroup* mTerrainGroup;
  Ogre::TerrainGlobalOptions* mTerrainGlobals;
 
  OgreBites::Label* mInfoLabel;
 
};
 
#endif // 