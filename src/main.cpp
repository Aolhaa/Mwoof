#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>

using namespace geode::prelude;

static float lastMusicVolume = 1.f;
static float lastSFXVolume = 1.f;
static bool mutedByMWoof = false;

static bool isEnabled() {
    return Mod::get()->getSettingValue<bool>("enabled");
}

static bool muteMusic() {
    return Mod::get()->getSettingValue<bool>("mute-music");
}

static bool muteSFX() {
    return Mod::get()->getSettingValue<bool>("mute-sfx");
}

class $modify(MWoofAppDelegate, AppDelegate) {
    void applicationDidEnterBackground() {
        AppDelegate::applicationDidEnterBackground();

        if (!isEnabled()) return;

        auto fmod = FMODAudioEngine::sharedEngine();
        if (!fmod) return;

        lastMusicVolume = fmod->getMusicVolume();
        lastSFXVolume = fmod->getSFXVolume();

        if (muteMusic())
            fmod->setMusicVolume(0.f);

        if (muteSFX())
            fmod->setSFXVolume(0.f);

        mutedByMWoof = true;
    }

    void applicationWillEnterForeground() {
        AppDelegate::applicationWillEnterForeground();

        if (!mutedByMWoof) return;

        auto fmod = FMODAudioEngine::sharedEngine();
        if (!fmod) return;

        if (muteMusic())
            fmod->setMusicVolume(lastMusicVolume);

        if (muteSFX())
            fmod->setSFXVolume(lastSFXVolume);

        mutedByMWoof = false;
    }
};
