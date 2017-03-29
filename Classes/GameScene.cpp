#include "GameScene.h"
#include "GameList.h"
#include "Taprace.h"
#include "Holdrace.h"
#include "Pinball.h"
#include "SmashBall.h"
#include "MainMenuScene.h"
#include "Shared.h"
#include "Airhockey.hpp"
#include "PlayerMenuScene.h"

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
	this->gameType = 0;
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
        showText(Shared::intToString(_counter), 1.0f, 10.0f);
    }
}

//Start game with the given time dt(seconds)
void GameScene::startGame(float dt){
    if (gameStatus == GAME_START){
        _counter = dt;
        this->schedule(schedule_selector(GameScene::updateCounter),1.0f);
		createBtnPanel();
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

		auto label = Label::createWithBMFont(SHARED_FONT_FILE, GS_WINNER_TEXT);

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

	auto label = Label::createWithBMFont(SHARED_FONT_FILE, GS_RETURN_TEXT);

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

void GameScene::showText(std::string s, float dt, float scale){
    log("showText %s", s.c_str());
    auto label = Label::createWithBMFont(SHARED_FONT_FILE, s);
    label->setBMFontSize(128);
    
    // position the label on the center of the screen
    Size winSize = Director::getInstance()->getWinSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    label->setPosition(screenCenter);
	auto currentScale = 1/scale;
	if (scale == 0) {
		currentScale = label->getContentSize().width / visibleSize.width;
	}
	log("scale %", currentScale);
	if (s.length() == 1) {
		label->setScale(GS_COUNTDOWN_TEXT_WIDTH_PERCENT / currentScale);
		log("showText length %d", s.length());
	}
	else {
		label->setScale(GS_GO_TEXT_WIDTH_PERCENT / currentScale);
	}
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    this->scheduleOnce([this, label, dt](float tp){ this->removeChild(label);}, dt, "remove"+s);
}

void GameScene::createListenerTabOverlay()
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	this->rectOverlay = DrawNode::create();
	Vec2 rectangle[4];
	rectangle[0] = origin;
	rectangle[1] = Vec2(origin.x, visibleSize.height);
	rectangle[2] = visibleSize;
	rectangle[3] = Vec2(visibleSize.width, origin.y);

	Color4F halfblack(0, 0, 0, 0.7f);
	rectOverlay->drawPolygon(rectangle, 4, halfblack, 1, halfblack);

	auto pauseIcon = Sprite::create("item/Item_Pause_Icon.png");
	pauseIcon->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 2) + (pauseIcon->getContentSize().height / 2)));
	pauseIcon->setScale(3 * pauseIcon->getContentSize().width / visibleSize.width);

	auto label = Label::createWithBMFont(SHARED_FONT_FILE, GS_RESUME_TEXT);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + (visibleSize.height / 2) - (label->getContentSize().height / 2)));

	auto currentScale = label->getContentSize().width / visibleSize.width;

	label->setScale(GS_RETURN_TEXT_WIDTH_PERCENT / (2 * currentScale));

	// add the label as a child to overlay layer
	rectOverlay->addChild(pauseIcon);
	rectOverlay->addChild(label);
	this->addChild(rectOverlay);

	auto tabScreenLtn = EventListenerTouchOneByOne::create();

	tabScreenLtn->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};

	tabScreenLtn->onTouchEnded = [=](Touch* touch, Event* event) {
		resumeGame();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(tabScreenLtn, this);
}

void GameScene::pauseGame()
{
	if (gameStatus == GAME_INPROGRESS) {
		gameStatus = GAME_PAUSE;
		menu->setVisible(FALSE);
		createListenerTabOverlay();
	}
}

void GameScene::resumeGame()
{
	if (gameStatus == GAME_PAUSE) {
		_counter = SHARED_COUNTDOWN_LENGTH;
		this->schedule(schedule_selector(GameScene::updateCounter), 1.0f);
		menu->setVisible(TRUE);
		rectOverlay->removeAllChildren();
		rectOverlay->clear();
	}
}

void GameScene::onGameStart(){}

void GameScene::createBtnPanel() {
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto winSize = Director::getInstance()->getWinSize();
	auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);

	Vector<MenuItem*> MenuItems;
	// Create pause and back buttons
	auto backBtn = MenuItemImage::create(
		"button/Button_Back.png",
		"button/Button_Back.png",
		CC_CALLBACK_1(GameScene::backButtonCallback, this));
	backBtn->setPosition(screenSize.width - backBtn->getContentSize().width / 2,
		screenCenter.y + backBtn->getContentSize().height);
	backBtn->setScale(backBtn->getContentSize().width / screenSize.width * 3);
	MenuItems.pushBack(backBtn);

	auto pauseBtn = MenuItemImage::create(
		"button/Button_Pause.png",
		"button/Button_Pause.png",
		CC_CALLBACK_1(GameScene::pauseButtonCallback, this));
	auto diffSize = backBtn->getContentSize().width - pauseBtn->getContentSize().width;
	pauseBtn->setPosition((screenSize.width - pauseBtn->getContentSize().width / 2) - abs(diffSize) / 2,
		screenCenter.y - pauseBtn->getContentSize().height);
	float scaleFromBackBtn = pauseBtn->getContentSize().width / backBtn->getContentSize().width;
	pauseBtn->setScale(scaleFromBackBtn);
	MenuItems.pushBack(pauseBtn);

	// create menu, it's an autorelease object
	this->menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void GameScene::backButtonCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->replaceScene(TransitionSlideInL::create(1, PlayerMenu::createScene(GameType(GameList::instance()->AVAILABLE_GAMES[gameType]))));
}

void GameScene::pauseButtonCallback(cocos2d::Ref * pSender)
{
	pauseGame();
}

template Scene* GameScene::createScene<Taprace>(int);
template Scene* GameScene::createScene<Holdrace>(int);
template Scene* GameScene::createScene<Airhockey>(int);
template Scene* GameScene::createScene<Pinball>(int);
template Scene* GameScene::createScene<SmashBall>(int);
