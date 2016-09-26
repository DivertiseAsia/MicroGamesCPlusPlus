#include "GameScene.h"
#include "Taprace.h"
#include "Holdrace.h"
#include "MainMenuScene.h"
USING_NS_CC;

template <class T>
Scene* GameScene::createScene(int numberOfPlayers)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create<T>(numberOfPlayers);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::GameScene(int numberOfPlayers)
{
	this->numberOfPlayers = numberOfPlayers;
	this->gameStatus = GAME_START;
}

template <class T>
GameScene* GameScene::create(int numberOfPlayers) {
	GameScene *pRet = new T(numberOfPlayers);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}


void GameScene::startGame(float dt){
    
}

void GameScene::endGame(int[])
{
	if (gameStatus == GAME_INPROGRESS) {
		log("game over");
		gameStatus = GAME_OVER;

		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto visibleSize = Director::getInstance()->getVisibleSize();

		auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "GAMEOVER");

		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);

		this->scheduleOnce(schedule_selector(GameScene::showReturnMenu), 1.0f);
	}
}

void GameScene::showReturnMenu(float dt) {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "Press anywhere to go home");

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);


	auto listener1 = EventListenerTouchOneByOne::create();

	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		Director::getInstance()->replaceScene(MainMenu::createScene());
	};
	
	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

template Scene* GameScene::createScene<Taprace>(int);
template Scene* GameScene::createScene<Holdrace>(int);