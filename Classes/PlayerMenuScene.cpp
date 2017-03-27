#include "PlayerMenuScene.h"
#include "MainMenuScene.h"
#include "SoundManager.h"
#include "Shared.h"


USING_NS_CC;

Scene* PlayerMenu::createScene(GameType game)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PlayerMenu::create();
	layer->setGameToLoad(game);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PlayerMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Shared::instance()->setBackground("bg/BG_Home.png", this);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	Vector<MenuItem*> MenuItems;

	//Title

	auto label = Label::createWithBMFont(SHARED_FONT_FILE, "Choose the number of players");
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height * 2));
	float currentScale = label->getContentSize().width / visibleSize.width;
	label->setScale(PMS_TITLE_WIDTH_PERCENT / currentScale);
	this->addChild(label, 1);

	currentScale = 0;

	float startY = origin.y + visibleSize.height * PMS_MENU_HEIGHT_PERCENT - label->getContentSize().height * 2;
	float availableArea = visibleSize.height * PMS_MENU_HEIGHT_PERCENT - label->getContentSize().height * 2;

	for (int i = 2; i <= 5; i++) {
		auto button = MenuItemImage::create();
		if (i < 5) {
			auto s = Shared::intToString(i);
			button = MenuItemImage::create(
				"button/Button_Choose_" + s + ".png", 
				"button/Button_Choose_Pressed_" + s + ".png", 
				CC_CALLBACK_1(PlayerMenu::startGame, this, i));
		}
		else {
			button = MenuItemImage::create(
				"button/Button_Choose_Back.png",
				"button/Button_Choose_Back.png",
				CC_CALLBACK_1(PlayerMenu::backButtonCallback, this));
		}
		button->setPosition(Vec2(origin.x + visibleSize.width / 2,
			startY - availableArea / 3.5 * (i - 2)));
		if (currentScale == 0) {
			currentScale = button->getContentSize().width / visibleSize.width;
		}
		button->setScale(currentScale / 1.5);
		MenuItems.pushBack(button);
	}
	
    this->setKeyboardEnabled(true);
	// create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void PlayerMenu::setGameToLoad(GameType game) {
	gameToLoad = game;
}

void PlayerMenu::startGame(Ref* pSender, int players) {
	SoundManager::instance()->playEffect(SOUND_FILE_MENU_TRANS);
	log("Want %d players for %s",players,GameList::instance()->getGameName(gameToLoad).c_str());
	auto scene = GameList::instance()->getScene(gameToLoad, players);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}

void PlayerMenu::backButtonCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionSlideInL::create(1, MainMenu::createScene()));
}

void PlayerMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE /*KeyboardEvent::KeyCode::Menu(KEY_BACKSPACE)*/)
    {
         log("You pressed back button");
         Director::getInstance()->end();
         exit(0);
    } 
    else if (keyCode == EventKeyboard::KeyCode::KEY_HOME)
    {
         log("You pressed home button");
         // pause the game
    } else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE){
        backButtonCallback(this);
	}
}