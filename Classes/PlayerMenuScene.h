#ifndef __PLAYERMENU_SCENE_H__
#define __PLAYERMENU_SCENE_H__

#define PMS_TITLE_WIDTH_PERCENT .9f
#define PMS_MENU_WIDTH_PERCENT .08f
#define PMS_MENU_HEIGHT_PERCENT .8f

#include "cocos2d.h"
#include "GameList.h"
#include "Shared.h"

USING_NS_CC;

class PlayerMenu : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(GameType game);

	virtual bool init();

	// a selector callback
	void backButtonCallback(cocos2d::Ref* pSender) ;
	void startGame(cocos2d::Ref* pSender, int players);
	void setGameToLoad(GameType game);

	//Override the keyevent
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerMenu);
private:
	GameType gameToLoad;
};

#endif // __PLAYERMENU_SCENE_H__
