#include "GameList.h"
#include "GameScene.h"
#include "Holdrace.h"
#include "Taprace.h"

USING_NS_CC;

GameList* GameList::minstance = NULL;

Scene* GameList::getScene(GameType listing, int numberOfPlayers) {
	switch (listing) {
		default:
			return GameScene::makeTapTap(numberOfPlayers);
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
