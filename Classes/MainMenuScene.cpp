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

	auto title = Sprite::create("item/Item_Home_Logo.png");
	// position the title on the center of the screen
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - title->getContentSize().height / 2 - MMS_TITLE_Y_OFFSET));

	float currentScale = title->getContentSize().width / visibleSize.width;
	title->setScale(MMS_TITLE_WIDTH_PERCENT / (currentScale * 1.5));
	// add the title as a child to this layer
	this->addChild(title, 1);

	currentScale = 0;

	float startY = origin.y + visibleSize.height * MMS_MENU_HEIGHT_PERCENT - title->getContentSize().height / 2;
	float availableArea = visibleSize.height * MMS_MENU_HEIGHT_PERCENT - title->getContentSize().height / 2;

	for (int i = 0; i < GameList::instance()->numberOfAvailableGames(); i++) {
		auto gameListingLabel = Label::createWithBMFont(SHARED_FONT_FILE, GameList::getGameName(GameList::instance ()->AVAILABLE_GAMES[i]));
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