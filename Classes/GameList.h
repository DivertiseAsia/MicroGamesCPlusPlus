#ifndef __GAME_LIST_H__
#define __GAME_LIST_H__

#include "cocos2d.h"


enum GameType { TAPTAP, OVERSHOOT, PINBALL, AIRHOCKEY, SMASHBALL ,WETKITTEN };

class GameList
{
public:
    static cocos2d::Scene* getScene(GameType listing, int numberOfPlayers);
	static std::string getGameName(GameType listing);
	static GameList* instance();
	static int numberOfAvailableGames();
	const std::map<const GameType, std::string> GAME_NAMES = {
		{ TAPTAP, "TapTap" },
		{ OVERSHOOT, "Overshoot"},
		{ PINBALL, "Pinball" },
		{ AIRHOCKEY, "Airhockey" },
		{ SMASHBALL, "SmashBall" },
		{ WETKITTEN, "WetKitten" },
	};

	const GameType AVAILABLE_GAMES[6] = {TAPTAP,OVERSHOOT,PINBALL,AIRHOCKEY,SMASHBALL,WETKITTEN };
private:
	GameList() {};
	static GameList* minstance;
};

#endif // __GAME_LIST_H__
