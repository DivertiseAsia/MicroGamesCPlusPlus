#ifndef __GAME_LIST_H__
#define __GAME_LIST_H__

#include "cocos2d.h"

class GameList
{
public:
	enum GameType { TAPTAP, OVERSHOOT, PINBALL, AIRHOCKEY, FLAPFLAP};

    static cocos2d::Scene* getScene(GameType listing);
	static std::string getGameName(GameType listing);
	static GameList* instance();
	static int numberOfAvailableGames();
	const std::map<const GameType, std::string> GAME_NAMES = {
		{ TAPTAP, "TapTap" },
		{ OVERSHOOT, "Overshoot"},
		{ PINBALL, "Pinball" },
		{ AIRHOCKEY, "Airhockey" },
		{ FLAPFLAP, "Flapflap" }
	};

	const GameType AVAILABLE_GAMES[5] = {TAPTAP,OVERSHOOT,PINBALL,AIRHOCKEY,FLAPFLAP};
private:
	GameList() {};
	static GameList* minstance;
};

#endif // __GAME_LIST_H__
