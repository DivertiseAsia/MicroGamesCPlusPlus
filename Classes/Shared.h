#ifndef __SHARED_H__
#define __SHARED_H__

#include "cocos2d.h"

#define SHARED_FONT_FILE "fonts/font.fnt"

#define SHARED_COLOR_BACKGROUND Color4F(.357f,.259f,.451f,1)

#define SHARED_COLOR_PLAYERS {Color4F(.322f,.580f,.949f,1),Color4F(.949f,.894f,.086f,1),Color4F(.851f,.282f,.314f,1),Color4F(.302f,.949f,.741f,1)}//Blue, Yellow, Red, Teal
#define SHARED_PLAYER1 0
#define SHARED_PLAYER2 1
#define SHARED_PLAYER3 2
#define SHARED_PLAYER4 3

#define SHARED_MAX_PLAYERS 4

#define SHARED_COUNTDOWN_LENGTH 4

#define SHARED_GAME_NAME "MicroGames"

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

USING_NS_CC;

//#define DEBUG_MODE

class Shared
{
public:
	static Shared* instance();
	static std::string intToString(int);
	cocos2d::Color4F getPlayerColor(int);
	cocos2d::Vec2 getPlayerPosition(int);
	cocos2d::Vec2 getPlayerAnchor(int);
	void setBackground(std::string, Layer*);

private:
	Shared() {};
	static Shared* minstance;
	std::vector<cocos2d::Color4F> colors;
	std::vector<cocos2d::Vec2> buttonPositions;
	std::vector<cocos2d::Vec2> buttonAnchor;
	bool debug;
};


#endif // __SHARED_H__
