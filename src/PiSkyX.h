/**
 * PiSkyX.h
 *
 * Default settings for our SkyX usage.
 *
 * Largely copied from a SkyX Demo I found.
 *
 * Victor
 */

#include <OgreRoot.h>
#include "skyx/SkyX.h"
#include "skyx/AtmosphereManager.h"

#ifndef _PI_SKYX_H_
#define _PI_SKYX_H_

extern SkyX::SkyX* mSkyX;
extern SkyX::BasicController* mBasicController;
extern Ogre::uint32 mCurrentPreset;

/** SkyX settings struct
    @remarks These just are the most important SkyX parameters, not all SkyX parameters.
 */
struct SkyXSettings {
    /** Constructor
        @remarks Skydome + vol. clouds + lightning settings
     */
    SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt,
        const bool& lc, const bool& vc, const Ogre::Real& vcws, const bool& vcauto, const Ogre::Radian& vcwd, 
        const Ogre::Vector3& vcac, const Ogre::Vector4& vclr,  const Ogre::Vector4& vcaf, const Ogre::Vector2& vcw,
        const bool& vcl, const Ogre::Real& vclat, const Ogre::Vector3& vclc, const Ogre::Real& vcltm)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws)
        , vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw)
        , vcLightnings(vcl), vcLightningsAT(vclat), vcLightningsColor(vclc), vcLightningsTM(vcltm)
    {}

    /** Constructor
        @remarks Skydome + vol. clouds
     */
    SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt,
        const bool& lc, const bool& vc, const Ogre::Real& vcws, const bool& vcauto, const Ogre::Radian& vcwd, 
        const Ogre::Vector3& vcac, const Ogre::Vector4& vclr,  const Ogre::Vector4& vcaf, const Ogre::Vector2& vcw)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(vc), vcWindSpeed(vcws)
        , vcAutoupdate(vcauto), vcWindDir(vcwd), vcAmbientColor(vcac), vcLightResponse(vclr), vcAmbientFactors(vcaf), vcWheater(vcw), vcLightnings(false)
    {}

    /** Constructor
        @remarks Skydome settings
     */
    SkyXSettings(const Ogre::Vector3 t, const Ogre::Real& tm, const Ogre::Real& mp, const SkyX::AtmosphereManager::Options& atmOpt, const bool& lc)
        : time(t), timeMultiplier(tm), moonPhase(mp), atmosphereOpt(atmOpt), layeredClouds(lc), volumetricClouds(false), vcLightnings(false)
    {}

    /// Time
    Ogre::Vector3 time;
    /// Time multiplier
    Ogre::Real timeMultiplier;
    /// Moon phase
    Ogre::Real moonPhase;
    /// Atmosphere options
    SkyX::AtmosphereManager::Options atmosphereOpt;
    /// Layered clouds?
    bool layeredClouds;
    /// Volumetric clouds?
    bool volumetricClouds;
    /// VClouds wind speed
    Ogre::Real vcWindSpeed;
    /// VClouds autoupdate
    bool vcAutoupdate;
    /// VClouds wind direction
    Ogre::Radian vcWindDir;
    /// VClouds ambient color
    Ogre::Vector3 vcAmbientColor;
    /// VClouds light response
    Ogre::Vector4 vcLightResponse;
    /// VClouds ambient factors
    Ogre::Vector4 vcAmbientFactors;
    /// VClouds wheater
    Ogre::Vector2 vcWheater;
    /// VClouds lightnings?
    bool vcLightnings;
    /// VClouds lightnings average aparition time
    Ogre::Real vcLightningsAT;
    /// VClouds lightnings color
    Ogre::Vector3 vcLightningsColor;
    /// VClouds lightnings time multiplier
    Ogre::Real vcLightningsTM;
};

/** Demo presets
    @remarks The best way of determinate each parameter value is by using a real-time editor.
             These presets have been quickly designed using the Paradise Editor, which is a commercial solution.
             At the time I'm writting these lines, SkyX 0.1 is supported by Ogitor. Hope that the Ogitor team will 
             support soon SkyX 0.4, this way you all are going to be able to quickly create cool SkyX configurations.
 */
extern const SkyXSettings mPresets[];

void setPreset(const SkyXSettings& preset, Ogre::Camera* camera);

#endif