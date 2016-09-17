#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "GameList.h"

class MainMenu : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void muteButtonCallback(cocos2d::Ref* pSender);
	void pickPlayers(cocos2d::Ref* pSender, GameList::GameType game);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};

#endif // __MAINMENU_SCENE_H__
