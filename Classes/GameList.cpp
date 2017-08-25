#include "GameList.h"
#include "GameScene.h"
#include "Holdrace.h"
#include "Pinball.h"
#include "SmashBall.h"
#include "Taprace.h"
#include "WetKitten.h"
#include "Airhockey.hpp"

USING_NS_CC;

GameList* GameList::minstance = NULL;

Scene* GameList::getScene(GameType listing, int numberOfPlayers) {
	switch (listing) 
	{
	case PINBALL:
		return GameScene::createScene<Pinball>(numberOfPlayers);
	case OVERSHOOT:
		return GameScene::createScene<Holdrace>(numberOfPlayers);
	case TAPTAP:
		return GameScene::createScene<Taprace>(numberOfPlayers);
    case AIRHOCKEY:
        return GameScene::createScene<Airhockey>(numberOfPlayers);
	case SMASHBALL:
		return GameScene::createScene<SmashBall>(numberOfPlayers);
	case WETKITTEN:
		return GameScene::createScene<WetKitten>(numberOfPlayers);
	default:
		return GameScene::createScene<Taprace>(numberOfPlayers);
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
