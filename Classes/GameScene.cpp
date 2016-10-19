#include "GameScene.h"
#include "Taprace.h"
#include "Holdrace.h"
#include "Pinball.h"
#include "MainMenuScene.h"
#include "Shared.h"
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

template <class T>
Scene* GameScene::createSceneWithPhysics(int numberOfPlayers)
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

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
        showText("Go!", 1.0f);
    } else{
        std::ostringstream ss;
        ss << _counter;
        std::string s(ss.str());
        showText(s, 1.0f);
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

		auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "WINNER");

		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

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

void GameScene::showText(std::string s, float dt){
    log("showText %s", s.c_str());
    auto label = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, s);
    label->setBMFontSize(64);
    
    // position the label on the center of the screen
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    label->setPosition(screenCenter);
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    this->scheduleOnce([this, label, dt](float tp){ this->removeChild(label);}, dt, "remove"+s);

}

template Scene* GameScene::createScene<Taprace>(int);
template Scene* GameScene::createScene<Holdrace>(int);
template Scene* GameScene::createSceneWithPhysics<Pinball>(int);