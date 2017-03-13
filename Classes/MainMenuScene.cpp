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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	Vector<MenuItem*> MenuItems;
    auto soundName = "SoundOn.png";
    if (SoundManager::instance()->isMuted())
        soundName = "SoundOff.png";

        
    _soundMuteItem = MenuItemImage::create(
                                           soundName,
                                           soundName,
                                           CC_CALLBACK_1(MainMenu::muteButtonCallback, this));

    
    _soundMuteItem->setPosition(Vec2(origin.x + visibleSize.width - _soundMuteItem->getContentSize().width/2 -20,
                                origin.y + _soundMuteItem->getContentSize().height/2+20));

	MenuItems.pushBack(_soundMuteItem);

	Shared::instance()->setBackground("bg/BG_Home.png", this);

	auto label = Label::createWithBMFont(SHARED_FONT_FILE_TITLE, SHARED_GAME_NAME);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height - MMS_TITLE_Y_OFFSET));
	float currentScale = label->getContentSize().width / visibleSize.width;
	label->setScale(MMS_TITLE_WIDTH_PERCENT / currentScale);
	// add the label as a child to this layer
	this->addChild(label, 1);

	currentScale = 0;

	float startY = origin.y + visibleSize.height * MMS_MENU_HEIGHT_PERCENT - label->getContentSize().height*2;
	float availableArea = visibleSize.height * MMS_MENU_HEIGHT_PERCENT - label->getContentSize().height * 2;

	for (int i = 0; i < GameList::instance()->numberOfAvailableGames(); i++) {
		auto gameListingLabel = Label::createWithBMFont(SHARED_FONT_FILE_MENU, GameList::getGameName(GameList::instance ()->AVAILABLE_GAMES[i]));
		auto gameListing = MenuItemLabel::create(gameListingLabel, CC_CALLBACK_1(MainMenu::pickPlayers, this, GameList::instance()->AVAILABLE_GAMES[i]));
		gameListing->setPosition(Vec2(origin.x + visibleSize.width / 2,
			startY - availableArea/GameList::instance()->numberOfAvailableGames() * i));

		if (currentScale == 0) {
			currentScale = gameListing->getContentSize().width / visibleSize.width;
		}
		gameListing->setScale(MMS_MENU_WIDTH_PERCENT / currentScale);
		MenuItems.pushBack(gameListing);
	}

    this->setKeyboardEnabled(true);
    // create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}

void MainMenu::pickPlayers(Ref* pSender, GameList::GameType game) {
	SoundManager::instance()->playEffect(SOUND_FILE_MENU_TRANS);
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f,PlayerMenu::createScene(game)));
}

void MainMenu::muteButtonCallback(Ref* pSender)
{
    //Mute or unmute the sound effects of the game
	if (SoundManager::instance()->isMuted()) {
		SoundManager::instance()->unmuteAll();
        _soundMuteItem->setNormalImage(Sprite::create("SoundOn.png"));
	}
	else {
		SoundManager::instance()->muteAll();
        _soundMuteItem->setNormalImage(Sprite::create("SoundOff.png"));
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