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