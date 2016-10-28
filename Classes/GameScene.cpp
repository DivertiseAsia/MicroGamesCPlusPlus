#include "GameScene.h"
#include "Taprace.h"
#include "Holdrace.h"
#include "Pinball.h"
#include "SmashBall.h"
#include "MainMenuScene.h"
#include "Shared.h"
#include "Airhockey.hpp"

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

//Reduce countdown Counter
void GameScene::updateCounter(float dt){
    _counter -= dt;
    if (_counter<=0){
        this->unschedule(schedule_selector(GameScene::updateCounter));
        _counter = 0;
        gameStatus = GAME_INPROGRESS;
        log("Game started!");
        showText(GS_GO_TEXT, 1.0f);
        onGameStart();
    } else{
        showText(Shared::intToString(_counter), 1.0f);
    }
}

//Start game with the given time dt(seconds)
void GameScene::startGame(float dt){
    if (gameStatus == GAME_START){
        _counter = dt;
        this->schedule(schedule_selector(GameScene::updateCounter),1.0f);
    }
}
void GameScene::endGame(int winner) {
	int winners[] = { winner };
	endGame(winners, 1);
}
void GameScene::endGame(int winners[], int totalWinners)
{
	if (gameStatus == GAME_INPROGRESS) {
		gameStatus = GAME_OVER;

		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto visibleSize = Director::getInstance()->getVisibleSize();

		auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, GS_WINNER_TEXT);

		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height - GS_WINNER_TEXT_OFFSET_Y));

		auto currentScale = label->getContentSize().width / visibleSize.width;

		label->setScale(GS_WINNER_TEXT_WIDTH_PERCENT / currentScale);

		// add the label as a child to this layer
		this->addChild(label, 1);

		//draw winner circles
		for (int i = 0; i < totalWinners; i++) {
			
			auto dNode = DrawNode::create();
			int radius = visibleSize.width / 6;
			dNode->setContentSize(Size(radius * 2, radius * 2));
			dNode->drawSolidCircle(Vec2(radius, radius), radius, 3.1415968f, 360, Shared::instance()->getPlayerColor(winners[i]));
			if (totalWinners == 1) {
				dNode->setPosition(Vec2(origin.x + visibleSize.width / 2 - dNode->getContentSize().width / 2,
					origin.y + visibleSize.height / 2 - dNode->getContentSize().height / 2));
			}
			else {
				dNode->setPosition(Vec2(origin.x + visibleSize.width / 2 - dNode->getContentSize().width / 2 - radius*2/totalWinners + radius*2*i,
					origin.y + visibleSize.height / 2 - dNode->getContentSize().height / 2));
			}
			
			this->addChild(dNode);
		}

		this->scheduleOnce(schedule_selector(GameScene::showReturnMenu), 1.0f);
	}
}

void GameScene::showReturnMenu(float dt) {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, GS_RETURN_TEXT);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + label->getContentSize().height));

	auto currentScale = label->getContentSize().width / visibleSize.width;

	label->setScale(GS_RETURN_TEXT_WIDTH_PERCENT / currentScale);

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

void GameScene::showText(std::string s, float dt){
    log("showText %s", s.c_str());
    auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, s);
    label->setBMFontSize(128);
    
    // position the label on the center of the screen
    Size winSize = Director::getInstance()->getWinSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    label->setPosition(screenCenter);

	auto currentScale = label->getContentSize().width / visibleSize.width;

	if (s.length() == 1) {
		label->setScale(GS_COUNTDOWN_TEXT_WIDTH_PERCENT / currentScale);
	}
	else {
		label->setScale(GS_GO_TEXT_WIDTH_PERCENT / currentScale);
	}
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    this->scheduleOnce([this, label, dt](float tp){ this->removeChild(label);}, dt, "remove"+s);
}

void GameScene::onGameStart(){}

template Scene* GameScene::createScene<Taprace>(int);
template Scene* GameScene::createScene<Holdrace>(int);
template Scene* GameScene::createScene<Airhockey>(int);
template Scene* GameScene::createScene<Pinball>(int);
template Scene* GameScene::createScene<SmashBall>(int);
