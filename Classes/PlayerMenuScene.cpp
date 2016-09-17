#include "PlayerMenuScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

Scene* PlayerMenu::createScene(GameList::GameType game)
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
	if (!Layer::init())
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
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(PlayerMenu::backButtonCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	MenuItems.pushBack(closeItem);

	for (int i = 2; i <= 4; i++) {
		auto item1 = Label::createWithBMFont("fonts/font2.fnt", std::to_string(i));
		auto game1 = MenuItemLabel::create(item1, CC_CALLBACK_1(PlayerMenu::startGame, this, i));
		game1->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height /2 + visibleSize.height / 10 * (4-i)));
		MenuItems.pushBack(game1);
	}
	

	// create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	auto label = Label::createWithBMFont("fonts/font2.fnt", "Choose the number of players");

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	return true;
}

void PlayerMenu::setGameToLoad(GameList::GameType game) {
	gameToLoad = game;
}

void PlayerMenu::startGame(Ref* pSender, int players) {
	log("Want %d players for %s",players,GameList::instance()->getGameName(gameToLoad).c_str());
	auto scene = GameList::instance()->getScene(gameToLoad);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}

void PlayerMenu::backButtonCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionSlideInL::create(1, MainMenu::createScene()));
}