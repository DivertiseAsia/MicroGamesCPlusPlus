#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "cocos2d.h"

#define SOUND_VOLUME_MUTE 0
#define SOUND_VOLUME_FULL 1

#define SOUND_FILE_FOLDER		"sounds/"
#define SOUND_FILE_MENU_TRANS	SOUND_FILE_FOLDER "metal-impact.wav"

#define SOUND_FILE_WIN			SOUND_FILE_FOLDER "crowd-hey.mp3"
#define SOUND_FILE_INGAME_PRESS SOUND_FILE_FOLDER "punch.wav"

#define SOUND_FILE_HOLD_ENGINE	SOUND_FILE_FOLDER "car-engine.mp3"
#define SOUND_FILE_HOLD_CRASH	SOUND_FILE_FOLDER "car-accident.mp3"
#define SOUND_FILE_HOCKEY_PUCK	SOUND_FILE_FOLDER "puck.mp3"


class SoundManager
{
public:
    static void unmuteAll();
	static void muteAll();
	static bool isMuted();
	static SoundManager* instance();

	static void playEffect(const char* filePath);
private:
	SoundManager() {};
	static SoundManager* minstance;
	bool muted = false;
};

#endif // __SOUND_MANAGER_H__
