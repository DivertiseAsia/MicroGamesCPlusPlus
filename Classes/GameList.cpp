#include "GameList.h"
#include "Holdrace.h"
#include "Taprace.h"

USING_NS_CC;

GameList* GameList::minstance = NULL;

cocos2d::Scene* GameList::getScene(GameType listing) {
	switch (listing) {
        case TAPTAP:
            return Taprace::createScene();
            break;
		case OVERSHOOT:
			return Holdrace::createScene();
			break;
		default:
			return Holdrace::createScene();
			break;
	}
}

std::string GameList::getGameName(GameType listing) {
	return GameList::instance()->GAME_NAMES.at(listing);
}

GameList * GameList::instance() {
	if (!minstance) {
		minstance = new GameList();
	}
	return minstance;
}

int GameList::numberOfAvailableGames()
{
	return sizeof(GameList::instance()->AVAILABLE_GAMES)/sizeof(*GameList::instance()->AVAILABLE_GAMES);
}
