#include "MainMenuScene.h"
#include "PlayerMenuScene.h"
#include "GameList.h"
#include "Shared.h"
#include "SoundManager.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
	auto center = visibleSize / 2;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float offsetY = visibleSize.height * MMS_Y_OFFSET_PERCENT;

	// play background sound
	if(!SoundManager::instance()->isMuted())
		SoundManager::instance()->playBackgroundSound();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	Vector<MenuItem*> MenuItems;
    auto soundName = "button/Buttom_Home_SoundOn.png";
    if (SoundManager::instance()->isMuted())
        soundName = "button/Buttom_Home_SoundOff.png";

    _soundMuteItem = MenuItemImage::create(
                                           soundName,
                                           soundName,
                                           CC_CALLBACK_1(MainMenu::muteButtonCallback, this));

    _soundMuteItem->setPosition(Vec2(origin.x + visibleSize.width - _soundMuteItem->getContentSize().width/2 -20,
                                origin.y + _soundMuteItem->getContentSize().height/2+20));

	_soundMuteItem->setScale(0.5);

	MenuItems.pushBack(_soundMuteItem);

	Shared::instance()->setBackground("bg/BG_Home.png", this);

	// define title sprite
	auto title = Sprite::create("item/Item_Home_Logo.png");
	float titleScale = MMS_TITLE_WIDTH_PERCENT * visibleSize.width / title->getContentSize().width;
	title->setScale(titleScale);

	float startY = center.height - offsetY;
	float menuScale = 0;
	MenuItemImage* button;

	// add game menu buttons
	float rad = 3.141 / 2;
	for (int i = 0; i < GameList::instance()->numberOfAvailableGames(); i++) {
		auto fname = "button/Button_Home_" + GameList::getGameName(GameList::instance()->AVAILABLE_GAMES[i]) + ".png";
		auto fname2 = "button/Button_Home_" + GameList::getGameName(GameList::instance()->AVAILABLE_GAMES[i]) + "_Pressed.png";
		log("Create button %s", fname.c_str());
		button = MenuItemImage::create(fname.c_str(), fname2.c_str(), CC_CALLBACK_1(MainMenu::pickPlayers, this, GameList::instance()->AVAILABLE_GAMES[i]));
		auto xPosition = cos(rad) * visibleSize.width * MMS_MENU_RADIUS_CONSTANT;
		auto yPosition = sin(rad) * visibleSize.width * MMS_MENU_RADIUS_CONSTANT;
		log("x = %.4f : y = %.4f", xPosition, yPosition);
		button->setPosition(Vec2(origin.x + visibleSize.width / 2 + xPosition,
			startY + yPosition));
		rad += (3.141 * 2) / 5;
		if (menuScale == 0) {
			menuScale = button->getContentSize().width / visibleSize.width;
		}
		button->setScale(MMS_MENU_WIDTH_PERCENT / menuScale);
		MenuItems.pushBack(button);
	}
	
	// position the title on the center of the screen
	if (visibleSize.width / visibleSize.height < 0.75)
		offsetY *= 2; // 2x offset if height of screen is more than width ~ 2 times
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		center.height + (offsetY + (title->getContentSize().height * titleScale / 2) + (button->getContentSize().height * menuScale))));

	// add the title as a child to this layer
	this->addChild(title, 1);

    this->setKeyboardEnabled(true);
    // create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
    
    return true;
}

void MainMenu::pickPlayers(Ref* pSender, GameType game) {
	SoundManager::instance()->playEffect(SOUND_FILE_MENU_TRANS);
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f,PlayerMenu::createScene(game)));
}

void MainMenu::muteButtonCallback(Ref* pSender)
{
    //Mute or unmute the sound effects of the game
	if (SoundManager::instance()->isMuted()) {
		SoundManager::instance()->unmuteAll();
        _soundMuteItem->setNormalImage(Sprite::create("button/Buttom_Home_SoundOn.png"));
	}
	else {
		SoundManager::instance()->muteAll();
        _soundMuteItem->setNormalImage(Sprite::create("button/Buttom_Home_SoundOff.png"));
	}
}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_HOME)
    {
         log("You pressed home button");
    } else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
        log("You pressed back button");
        Director::getInstance()->end();
        exit(0);
	}
}