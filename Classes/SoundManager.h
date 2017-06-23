#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "cocos2d.h"

#define SOUND_VOLUME_MUTE 0
#define SOUND_VOLUME_FULL 1

#define SOUND_FILE_FOLDER			"sounds/"
#define SOUND_FILE_MENU_TRANS		SOUND_FILE_FOLDER "Select.mp3"

#define SOUND_FILE_WIN				SOUND_FILE_FOLDER "Score.mp3"
#define SOUND_FILE_INGAME_PRESS		SOUND_FILE_FOLDER "punch.wav" //ButtonTap.mp3
#define SOUND_FILE_BACK				SOUND_FILE_FOLDER "Back.mp3"

#define SOUND_FILE_HOCKEY_PUCK		SOUND_FILE_FOLDER "puck.mp3"
#define SOUND_FILE_TAPRACE_STEP		SOUND_FILE_FOLDER "Step.mp3"
#define SOUND_FILE_HOLDRACE_JUMP	SOUND_FILE_FOLDER "Jump.mp3"

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
