/**
 * PiSkyX.cpp
 *
 * Default settings for our SkyX usage.
 *
 * Largely copied from a SkyX Demo I found.
 *
 * Victor
 */

#include "PiSkyX.h"

SkyX::SkyX* mSkyX = 0;
SkyX::BasicController* mBasicController = 0;
Ogre::uint32 mCurrentPreset = 4;

const SkyXSettings mPresets[] = {
    // Sunset
    // SkyXSettings(Ogre::Vector3(8.85f, 7.5f, 20.5f),  -0.08f, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Ogre::Radian(270), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.35, 0.2, 0.92, 0.1), Ogre::Vector4(0.4, 0.7, 0, 0), Ogre::Vector2(0.8,1)),
    // SkyXSettings(Ogre::Vector3(8.85f, 7.5f, 20.5f),  0.4f, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Ogre::Radian(270), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.35, 0.2, 0.92, 0.1), Ogre::Vector4(0.4, 0.7, 0, 0), Ogre::Vector2(0.8,1)),
    SkyXSettings(Ogre::Vector3(8.85f, 7.5f, 20.5f),  0.0f, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0022f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.52f, 0.44f), -0.991f, 3, 4), false, true, 300, false, Ogre::Radian(270), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.35, 0.2, 0.92, 0.1), Ogre::Vector4(0.4, 0.7, 0, 0), Ogre::Vector2(0.8,1)),
    // Clear
    SkyXSettings(Ogre::Vector3(17.16f, 7.5f, 20.5f), 0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0017f, 0.000675f, 30, Ogre::Vector3(0.57f, 0.54f, 0.44f), -0.991f, 2.5f, 4), false),
    // Thunderstorm 1
    SkyXSettings(Ogre::Vector3(12.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 1, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0.25, 0.4, 0.5, 0.1), Ogre::Vector4(0.45, 0.3, 0.6, 0.1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(1,0.976,0.92), 2),
    // Thunderstorm 2
    SkyXSettings(Ogre::Vector3(10.23, 7.5f, 20.5f),  0, 0, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.00545f, 0.000375f, 30, Ogre::Vector3(0.55f, 0.54f, 0.52f), -0.991f, 0.5, 4), false, true, 300, false, Ogre::Radian(0), Ogre::Vector3(0.63f,0.63f,0.7f), Ogre::Vector4(0, 0.02, 0.34, 0.24), Ogre::Vector4(0.29, 0.3, 0.6, 1), Ogre::Vector2(1,1), true, 0.5, Ogre::Vector3(0.95,1,1), 2),
    // Desert
    // SkyXSettings(Ogre::Vector3(7.59f, 7.5f, 20.5f), 0, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1), true),
    SkyXSettings(Ogre::Vector3(8.59f, 7.5f, 20.5f), 0.01, -0.8f, SkyX::AtmosphereManager::Options(9.77501f, 10.2963f, 0.01f, 0.0072f, 0.000925f, 30, Ogre::Vector3(0.71f, 0.59f, 0.53f), -0.997f, 2.5f, 1), true),
    // Night
    // SkyXSettings(Ogre::Vector3(21.5f, 7.5, 20.5), 0.03, -0.25, SkyX::AtmosphereManager::Options(), true)
    SkyXSettings(Ogre::Vector3(21.5f, 7.5, 20.5), 0.4, -0.25, SkyX::AtmosphereManager::Options(), true)
};

void setPreset(const SkyXSettings& preset, Ogre::Camera* camera) {
    mSkyX->setTimeMultiplier(preset.timeMultiplier);
    mBasicController->setTime(preset.time);
    mBasicController->setMoonPhase(preset.moonPhase);
    mSkyX->getAtmosphereManager()->setOptions(preset.atmosphereOpt);

    // // Layered clouds
    if (preset.layeredClouds)
    {
        // Create layer cloud
        if (mSkyX->getCloudsManager()->getCloudLayers().empty())
        {
            mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
        }
    }
    else
    {
        // Remove layer cloud
        if (!mSkyX->getCloudsManager()->getCloudLayers().empty())
        {
            mSkyX->getCloudsManager()->removeAll();
        }
    }

    mSkyX->getVCloudsManager()->setWindSpeed(preset.vcWindSpeed);
    mSkyX->getVCloudsManager()->setAutoupdate(preset.vcAutoupdate);

    SkyX::VClouds::VClouds* vclouds = mSkyX->getVCloudsManager()->getVClouds();

    vclouds->setWindDirection(preset.vcWindDir);
    vclouds->setAmbientColor(preset.vcAmbientColor);
    vclouds->setLightResponse(preset.vcLightResponse);
    vclouds->setAmbientFactors(preset.vcAmbientFactors);
    vclouds->setWheater(preset.vcWheater.x, preset.vcWheater.y, false);

    if (preset.volumetricClouds)
    {
        // Create VClouds
        if (!mSkyX->getVCloudsManager()->isCreated())
        {
            // SkyX::MeshManager::getSkydomeRadius(...) works for both finite and infinite(=0) camera far clip distances
            mSkyX->getVCloudsManager()->create(mSkyX->getMeshManager()->getSkydomeRadius(camera));
            // mSkyX->getVCloudsManager()->create(999999);
        }
    }
    else
    {
        // Remove VClouds
        if (mSkyX->getVCloudsManager()->isCreated())
        {
            mSkyX->getVCloudsManager()->remove();
        }
    }

    vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
    vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
    vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
    vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);

    mSkyX->update(0);
}
