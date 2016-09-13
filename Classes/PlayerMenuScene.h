#ifndef __PLAYERMENU_SCENE_H__
#define __PLAYERMENU_SCENE_H__

#include "cocos2d.h"

class PlayerMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void backButtonCallback(cocos2d::Ref* pSender);
	void startGame(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(PlayerMenu);
};

#endif // __PLAYERMENU_SCENE_H__
