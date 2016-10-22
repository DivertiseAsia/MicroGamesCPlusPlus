#ifndef __SHARED_H__
#define __SHARED_H__

#include "cocos2d.h"

#define SHARED_FONT_FILE_TITLE	"fonts/font-title.fnt"
#define SHARED_FONT_FILE_MENU	"fonts/font-menu.fnt"
#define SHARED_FONT_FILE_INGAME	"fonts/font-ingame.fnt"

#define SHARED_COLOR_BACKGROUND Color4F(.357f,.259f,.451f,1)

#define SHARED_COLOR_PLAYERS {Color4F(.322f,.580f,.949f,1),Color4F(.949f,.894f,.086f,1),Color4F(.851f,.282f,.314f,1),Color4F(.302f,.949f,.741f,1)}//Blue, Yellow, Red, Teal
#define SHARED_PLAYER1 0
#define SHARED_PLAYER2 1
#define SHARED_PLAYER3 2
#define SHARED_PLAYER4 3

#define SHARED_MAX_PLAYERS 4

#define SHARED_COUNTDOWN_LENGTH 4

#define SHARED_GAME_NAME "MicroGames"

#define DEBUG_MODE

class Shared
{
public:
	static Shared* instance();
	cocos2d::Color4F getPlayerColor(int);
	cocos2d::Vec2 getPlayerPosition(int);
	bool isDebugMode();
	void setDebugMode(bool);
private:
	Shared() {};
	static Shared* minstance;
	std::vector<cocos2d::Color4F> colors;
	std::vector<cocos2d::Vec2> buttonPositions;
	bool debug;
};


#endif // __SHARED_H__
