#include "PlayerMenuScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

Scene* PlayerMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PlayerMenu::create();

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

	auto item1 = Label::createWithBMFont("fonts/font2.fnt", "1");
	item1->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	auto game1 = MenuItemLabel::create(item1, CC_CALLBACK_1(PlayerMenu::startGame, this));


	MenuItems.pushBack(game1);

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

void PlayerMenu::startGame(Ref* pSender) {

}

void PlayerMenu::backButtonCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionSlideInL::create(1, MainMenu::createScene()));
}
