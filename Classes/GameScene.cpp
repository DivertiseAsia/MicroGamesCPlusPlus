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
		log("Set gameType %d", pRet->gameType);
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
		menu->setVisible(FALSE);

		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 centerPosition = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		float panelMarginX = visibleSize.width * GS_PERCENT_PANEL_MARGIN / 3;
		float panelMarginY = visibleSize.height * GS_PERCENT_PANEL_MARGIN;

		// create winner box label
		auto box = Sprite::create("item/Item_Win_Box.png");
		auto label = Sprite::create("item/Item_Win_Text.png");

		auto currentScale = box->getContentSize().width / visibleSize.width;

		box->setScale(GS_WINNER_TEXT_WIDTH_PERCENT / currentScale);
		label->setScale(GS_WINNER_TEXT_WIDTH_PERCENT / currentScale);

		// position the label on the center of the screen
		box->setPosition(centerPosition + Vec2(0, panelMarginY / 2));
		label->setPosition(box->getPosition() + Vec2(0, box->getContentSize().height * box->getScale() / 4));

		// add the label as a child to this layer
		this->addChild(box, 1);
		this->addChild(label, 2);

		// create replay and home buttons
		auto replayBtn = MenuItemImage::create(
			"button/Button_Win_Replay.png",
			"button/Button_Win_Replay.png",
			CC_CALLBACK_1(GameScene::replayButtonCallback, this));
		replayBtn->setScale(GS_PERCENT_WIN_MENU_ITEM * visibleSize.width / replayBtn->getContentSize().width);

		auto homeBtn = MenuItemImage::create(
			"button/Button_Win_Home.png",
			"button/Button_Win_Home.png",
			CC_CALLBACK_1(GameScene::homeButtonCallback, this));
		homeBtn->setScale(GS_PERCENT_WIN_MENU_ITEM * visibleSize.width / homeBtn->getContentSize().width);

		auto panelWidth = replayBtn->getContentSize().width / 2 + homeBtn->getContentSize().width / 2 + panelMarginX;
		replayBtn->setPosition(Vec2::ZERO - Vec2(panelWidth / 2, 0));
		homeBtn->setPosition(Vec2::ZERO + Vec2(panelWidth / 2, 0));
		Vector<MenuItem*> MenuItems;
		MenuItems.pushBack(replayBtn);
		MenuItems.pushBack(homeBtn);

		// create menu, it's an autorelease object
		this->menu = Menu::createWithArray(MenuItems);
		menu->setPosition(centerPosition - Vec2(0, panelMarginY));
		this->addChild(menu);

		float scaleWinner = 1;
		for (int i = 0; i < totalWinners; i++) {
			auto spriteWinner = Sprite::create("item/Item_Win_Winner" + Shared::intToString(winners[i] + 1) + ".png");
			if(scaleWinner == 1)
				scaleWinner = GS_PERCENT_WIN_WINNER_ITEM * visibleSize.width / spriteWinner->getContentSize().width;
			spriteWinner->setScale(scaleWinner);
			auto winnerOffsetSize = spriteWinner->getContentSize().width * scaleWinner;
			if (totalWinners == 1) {
				spriteWinner->setPosition(Vec2(centerPosition.x, box->getPosition().y));
			}
			else {
				spriteWinner->setPosition(Vec2(centerPosition.x - (winnerOffsetSize / totalWinners - winnerOffsetSize * i), box->getPosition().y));
			}

			this->addChild(spriteWinner, 3);
		}

		//draw winner circles
		/*for (int i = 0; i < totalWinners; i++) {
			
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
		}*/

		//this->scheduleOnce(schedule_selector(GameScene::showReturnMenu), 1.0f);
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
	rectOverlay->setContentSize(visibleSize);
	Vec2 rectangle[4];
	rectangle[0] = Vec2::ZERO;
	rectangle[1] = Vec2(0, visibleSize.height);
	rectangle[2] = visibleSize;
	rectangle[3] = Vec2(visibleSize.width, 0);

	Color4F halfblack(0, 0, 0, 0.7f);
	rectOverlay->drawPolygon(rectangle, 4, halfblack, 1, halfblack);

	auto overlaySize = rectOverlay->getContentSize();

	auto pauseIcon = Sprite::create("item/Item_Pause_Icon.png");
	pauseIcon->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 + pauseIcon->getContentSize().height / 2));
	pauseIcon->setScale(3 * pauseIcon->getContentSize().width / visibleSize.width);

	auto label = Label::createWithBMFont(SHARED_FONT_FILE, GS_RESUME_TEXT);
	// position the label on the center of the screen
	label->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 - label->getContentSize().height / 2));

	auto currentScale = label->getContentSize().width / visibleSize.width;

	label->setScale(GS_RETURN_TEXT_WIDTH_PERCENT / (2 * currentScale));

	// add the label as a child to overlay layer
	rectOverlay->addChild(pauseIcon);
	rectOverlay->addChild(label);
	rectOverlay->setPosition(origin);
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
	auto screenOrigin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto screenCenter = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	auto offsetY = GS_MENU_OFFSET_Y * visibleSize.height;
	float scale = 1;

	Vector<MenuItem*> MenuItems;
	// Create pause and back buttons
	auto backBtn = MenuItemImage::create(
		"button/Button_Back.png",
		"button/Button_Back.png",
		CC_CALLBACK_1(GameScene::backButtonCallback, this));
	scale = GS_MENU_WIDTH_PERCENT * visibleSize.width / backBtn->getContentSize().width;
	backBtn->setScale(scale);
	backBtn->setPosition(visibleSize.width - backBtn->getContentSize().width * scale,
		screenCenter.y + backBtn->getContentSize().height / 2 * scale + offsetY);
	MenuItems.pushBack(backBtn);

	auto pauseBtn = MenuItemImage::create(
		"button/Button_Pause.png",
		"button/Button_Pause.png",
		CC_CALLBACK_1(GameScene::pauseButtonCallback, this));
	float diffCenter = (backBtn->getContentSize().width - pauseBtn->getContentSize().width) / 2;
	scale = (GS_MENU_WIDTH_PERCENT - diffCenter / visibleSize.width) * visibleSize.width / pauseBtn->getContentSize().width;
	pauseBtn->setScale(scale);
	pauseBtn->setPosition(visibleSize.width - (pauseBtn->getContentSize().width + diffCenter) * scale,
		screenCenter.y - pauseBtn->getContentSize().height / 2 * scale - offsetY);
	MenuItems.pushBack(pauseBtn);

	// create menu, it's an autorelease object
	this->menu = Menu::createWithArray(MenuItems);
	menu->setPosition(screenOrigin);
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

void GameScene::replayButtonCallback(cocos2d::Ref * pSender)
{
	log("Replay gameType %d", gameType);
	auto scene = GameList::instance()->getScene(GameType(GameList::instance()->AVAILABLE_GAMES[gameType]), numberOfPlayers);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}

void GameScene::homeButtonCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, MainMenu::createScene()));
}

template Scene* GameScene::createScene<Taprace>(int);
template Scene* GameScene::createScene<Holdrace>(int);
template Scene* GameScene::createScene<Airhockey>(int);
template Scene* GameScene::createScene<Pinball>(int);
template Scene* GameScene::createScene<SmashBall>(int);
