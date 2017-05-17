#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "GameList.h"

#define MMS_Y_OFFSET_PERCENT	.1f
#define MMS_TITLE_WIDTH_PERCENT .7f
#define MMS_MENU_WIDTH_PERCENT .3f
#define MMS_MENU_RADIUS_CONSTANT .3f

USING_NS_CC;

class MainMenu : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void muteButtonCallback(cocos2d::Ref* pSender);
	void pickPlayers(cocos2d::Ref* pSender, GameType game);
    
    //Override the keyevent
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
private:
    cocos2d::MenuItemImage* _soundMuteItem;
};

#endif // __MAINMENU_SCENE_H__
