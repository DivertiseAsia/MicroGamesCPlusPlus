#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include <sstream>
#include <string>

USING_NS_CC;
using namespace CocosDenshion;

SoundManager* SoundManager::minstance = NULL;

SoundManager * SoundManager::instance() {
	if (!minstance) {
		minstance = new SoundManager();
	}
	return minstance;
}

void SoundManager::muteAll() {
	SoundManager::instance()->muted = true;

	SimpleAudioEngine::getInstance()->setEffectsVolume(SOUND_VOLUME_MUTE);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(SOUND_VOLUME_MUTE);

	stopBackgroundSound();
}

void SoundManager::unmuteAll() {
	SoundManager::instance()->muted = false;

	SimpleAudioEngine::getInstance()->setEffectsVolume(SOUND_VOLUME_FULL);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(SOUND_VOLUME_FULL);
}


void SoundManager::playEffect(const char* filePath) {
	if (!SoundManager::instance()->isMuted()) {
		SimpleAudioEngine::getInstance()->playEffect(filePath, false, 1, 0, 1);
	}
}

bool SoundManager::isMuted() {
	return SoundManager::instance()->muted;
}

void SoundManager::playBackgroundSound()
{
	SoundManager::instance()->backgroundMuted = false;
	SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_FILE_MENU_BACKGROUND);
}

void SoundManager::stopBackgroundSound()
{
	SoundManager::instance()->backgroundMuted = true;
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

bool SoundManager::isBackgroundMuted()
{
	return SoundManager::instance()->backgroundMuted;
}