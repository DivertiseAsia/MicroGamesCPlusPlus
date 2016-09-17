#ifndef __PLAYERMENU_SCENE_H__
#define __PLAYERMENU_SCENE_H__

#include "cocos2d.h"
#include "GameList.h"

class PlayerMenu : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(GameList::GameType game);

	virtual bool init();

	// a selector callback
	void backButtonCallback(cocos2d::Ref* pSender);
	void startGame(cocos2d::Ref* pSender, int players);
	void setGameToLoad(GameList::GameType game);

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerMenu);
private:
	GameList::GameType gameToLoad;
};

#endif // __PLAYERMENU_SCENE_H__
