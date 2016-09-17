#include "MainMenuScene.h"
#include "PlayerMenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameList.h"

USING_NS_CC;
using namespace CocosDenshion;

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
    if ( !Layer::init() )
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
                                           CC_CALLBACK_1(MainMenu::muteButtonCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	MenuItems.pushBack(closeItem);

	for (int i = 0; i < GameList::instance()->numberOfAvailableGames(); i++) {
		auto gameListingLabel = Label::createWithBMFont("fonts/font2.fnt", GameList::getGameName(GameList::instance ()->AVAILABLE_GAMES[i]));
		auto gameListing = MenuItemLabel::create(gameListingLabel, CC_CALLBACK_1(MainMenu::pickPlayers, this, GameList::instance()->AVAILABLE_GAMES[i]));
		gameListing->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 8 * (GameList::instance()->numberOfAvailableGames() - i)));
		MenuItems.pushBack(gameListing);
	}

    // create menu, it's an autorelease object
	auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
    
	auto label = Label::createWithBMFont("fonts/font2.fnt", "MicroGames");
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    return true;
}

void MainMenu::pickPlayers(Ref* pSender, GameList::GameType game) {
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f,PlayerMenu::createScene(game)));
}

void MainMenu::muteButtonCallback(Ref* pSender)
{
    //Mute or unmute the sound effects of the game
	if (SimpleAudioEngine::getInstance()->getEffectsVolume() > 0) {
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	}
	else {
		SimpleAudioEngine::getInstance()->setEffectsVolume(1);
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
	}
}