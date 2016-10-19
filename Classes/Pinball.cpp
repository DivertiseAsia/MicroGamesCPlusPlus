//
//  Pinball.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Pinball.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GLES-Render/B2DebugDrawLayer.h"
#include "ui/CocosGUI.h"
#include "SoundManager.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool Pinball::init()
{
	//////////////////////////////
	// 1. super init first
	if (!GameScene::init())
	{
		return false;
	}

	//http://www.cocos2d-x.org/wiki/Multi_resolution_support
	auto screenSize = Director::getInstance()->getVisibleSize();

	Size winSize = Director::getInstance()->getWinSize();
	auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);

	//Create drawNode and draw the center line
	_drawNode = DrawNode::create(10);    //Default line width
	_drawNode->drawLine(Vec2(0, screenCenter.y), Vec2(winSize.width, screenCenter.y), Color4F::GRAY);
	this->addChild(_drawNode);

	_score[TEAM_TOP] = 0;
	_score[TEAM_BOT] = 0;

	scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
	scoreBottom->setBMFontSize(64);
	scoreBottom->setPosition(Vec2(50, screenCenter.y - 50));
	scoreBottom->setRotation(90);
	this->addChild(scoreBottom, 1);

	scoreTop = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
	scoreTop->setBMFontSize(64);
	scoreTop->setPosition(Vec2(50, screenCenter.y + 50));
	scoreTop->setRotation(90);
	this->addChild(scoreTop, 1);
	updateScore();

	//create the walls
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
	drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
	auto body = PhysicsBody::createBox(Size(3, screenSize.height), PhysicsMaterial(1,.95f,1));
	body->setDynamic(false);
	drawNode->addComponent(body);
	drawNode->setPosition(Vec2(0, 0));
	this->addChild(drawNode);

	drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
	drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
	body = PhysicsBody::createBox(Size(3, screenSize.height), PhysicsMaterial(1, .95f, 1));
	body->setDynamic(false);
	drawNode->addComponent(body);
	drawNode->setPosition(Vec2(screenSize.width - 3, 0));
	this->addChild(drawNode);

	//create the ball
	float offset = BALL_RESET_OFFSET_Y;
	if (cocos2d::rand_0_1() > 0.5) {
		offset = -BALL_RESET_OFFSET_Y;
	}
	auto p = Vec2(screenCenter.x, screenCenter.y+offset);
	_ball = Ball::create(Shared::instance()->getPlayerColor(0));
	_ball->setPosition(p);

	body = PhysicsBody::createCircle(DEFAULT_BALL_RADIUS, PhysicsMaterial());
	body->setGravityEnable(true);
	//body->setCategoryBitmask(CAT_MASK_BALL);
	//body->setCollisionBitmask(CAT_MASK_PADDLE);
	_ball->addComponent(body);
	this->addChild(_ball);

	//create the controls and paddles
	for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
		_button[i] = GameButton::create();
		if (numberOfPlayers < 3 && i == SHARED_PLAYER3) {
			_button[i]->changeColor(Shared::instance()->getPlayerColor(SHARED_PLAYER2));
		}
		else if (numberOfPlayers < 4 && i == SHARED_PLAYER4) {
			_button[i]->changeColor(Shared::instance()->getPlayerColor(SHARED_PLAYER1));
		}
		else {
			_button[i]->changeColor(Shared::instance()->getPlayerColor(i));
		}
		_button[i]->setPlayer(i);
		auto currentPosition = Shared::instance()->getPlayerPosition(i);
		_button[i]->setPosition(currentPosition);
		_button[i]->setTag(i);  //Set the number to indicate button order.
		_button[i]->addTouchEventListener(CC_CALLBACK_2(Pinball::onPress, this));


		auto body = PhysicsBody::createCircle(DEFAULT_BUTTON_RADIUS, PhysicsMaterial());
		//body->setCategoryBitmask(CAT_MASK_STAT);
		//body->setCollisionBitmask(CAT_MASK_STAT);
		body->setDynamic(false);
		_button[i]->addComponent(body);

		//paddle
		_paddle[i] = DrawNode::create();
		_paddle[i]->setContentSize(cocos2d::Size(PADDLE_WIDTH_PX, screenSize.width* PADDLE_LENGTH_PERCENT));
		_paddle[i]->drawSolidRect(Vec2(0, 0), Vec2(PADDLE_WIDTH_PX, screenSize.width * PADDLE_LENGTH_PERCENT), Color4F::GRAY);
		auto paddlebody = PhysicsBody::createBox(Size(PADDLE_WIDTH_PX, screenSize.width * PADDLE_LENGTH_PERCENT), PhysicsMaterial(1, .99f, .5f));
		paddlebody->setMass(10);
		paddlebody->setAngularVelocityLimit(10);
		paddlebody->setVelocityLimit(0);
		paddlebody->setGravityEnable(false);
		//paddlebody->setCategoryBitmask(CAT_MASK_PADDLE);
		//paddlebody->setCategoryBitmask(CAT_MASK_BALL);
		_paddle[i]->addComponent(paddlebody);
		int yfix = 1;
		int xfix = 1;

		if (Shared::instance()->getPlayerPosition(i).x > screenCenter.x) {
			xfix = -1;
		}
		if (Shared::instance()->getPlayerPosition(i).y > screenCenter.y) {
			yfix = -1;
		}
		float startAngle = getMinPaddleAngle(i);
		if (i == SHARED_PLAYER3 || i == SHARED_PLAYER4) {
			startAngle = getMaxPaddleAngle(startAngle);
		}
		Vec2 pos = Vec2(Shared::instance()->getPlayerPosition(i).x + DEFAULT_BUTTON_RADIUS*xfix, Shared::instance()->getPlayerPosition(i).y + DEFAULT_BUTTON_RADIUS*yfix);
		_paddle[i]->setPosition(pos.x, pos.y);
		_paddle[i]->setRotation(startAngle);
		this->addChild(_paddle[i]);



		this->addChild(_button[i]);
	}

	//Debug Layer
	//this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);

	this->setName("PinballSceneRoot");
	this->initTouchHandling();
	this->scheduleUpdate();

	return true;
}

void Pinball::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated) {

}

void Pinball::update(float dt) {
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
	float gameHeight = Director::getInstance()->getVisibleSize().height;
	float ballY = _ball->getPositionY();
	if (ballY < gameHeight/2) {
		this->getScene()->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f));
	}
	else {
		this->getScene()->getPhysicsWorld()->setGravity(Vec2(0.0f, 98.0f));
	}

	if (ballY > gameHeight || ballY < 0) {
		//give score
		if (ballY > gameHeight) {
			_score[TEAM_BOT]++;
			_ball->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, gameHeight / 2 - BALL_RESET_OFFSET_Y));
			if (_score[TEAM_BOT] >= POINTS_TO_WIN) {
				if (numberOfPlayers > 2)
				{
					int winners[] = TEAM_BOT_PLAYERS;
					endGame(winners, 2);
				}
				else {
					endGame(SHARED_PLAYER2);
				}
			}
		}
		else {
			_score[TEAM_TOP]++;
			_ball->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, gameHeight / 2 + BALL_RESET_OFFSET_Y));
			if (_score[TEAM_TOP] >= POINTS_TO_WIN) {
				if (numberOfPlayers > 3)
				{
					int winners[] = TEAM_TOP_PLAYERS;
					endGame(winners,2);
				}
				else {
					endGame(SHARED_PLAYER1);
				}
			}
		}
		//reset ball velocity
		_ball->getPhysicsBody()->setVelocity(Vec2::ZERO);
		updateScore();
	}

	lockPaddleAngle(SHARED_PLAYER1);
	lockPaddleAngle(SHARED_PLAYER2);
	lockPaddleAngle(SHARED_PLAYER3);
	lockPaddleAngle(SHARED_PLAYER4);
	
}

float Pinball::getMinPaddleAngle(int paddle) {
	auto angles = MIN_ANGLE;
	return angles.begin()[paddle];
}

float Pinball::getMaxPaddleAngle(float minAngle) {
	return minAngle + MAX_ANGLE_DIFF;
}

void Pinball::lockPaddleAngle(int paddle) {
	float minAngle = getMinPaddleAngle(paddle);
	float maxAngle = getMaxPaddleAngle(minAngle);
	if (_paddle[paddle]->getPhysicsBody()->getRotation() > maxAngle) {
		_paddle[paddle]->getPhysicsBody()->setAngularVelocity(0);
		_paddle[paddle]->setRotation(maxAngle);
	}
	else if (_paddle[paddle]->getPhysicsBody()->getRotation() < minAngle) {
		_paddle[paddle]->getPhysicsBody()->setAngularVelocity(0);
		_paddle[paddle]->setRotation(minAngle);
	}
}

//This method will be called on the Node entered.
void Pinball::onEnter() {
	Node::onEnter();
	startGame();
}

void Pinball::startGame() {
	for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
		auto joint = PhysicsJointPin::construct(_button[i]->getPhysicsBody(), _paddle[i]->getPhysicsBody(), _paddle[i]->getPosition());
		joint->setCollisionEnable(false);


		this->getScene()->getPhysicsWorld()->addJoint(joint);
	}
	this->getScene()->getPhysicsWorld()->setGravity(Vec2::ZERO);
	GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void Pinball::initTouchHandling() {
	auto listener1 = EventListenerTouchOneByOne::create();

	// trigger when you push down
	listener1->onTouchBegan = [](Touch* touch, Event* event) {


		auto parentNode = static_cast<Sprite*>(event->getCurrentTarget());

		Vector<Node *> children = parentNode->getChildren();
		Point touchPosition = parentNode->convertTouchToNodeSpace(touch);
		for (auto iter = children.rbegin(); iter != children.rend(); ++iter) {
			Node *childNode = *iter;
			if (childNode->getBoundingBox().containsPoint(touchPosition)) {
				//childNode is the touched Node
				//do the stuff here
				log(">>%s", childNode->getName().c_str());
				return true;
			}
		}
		return false;
		//return true; // if you are consuming it
		log("TOUCH!!");

	};

	// trigger when moving touch
	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		log("MOVE");
	};

	// trigger when you let up
	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		// your code
		log("TOUCH ENDED");
	};

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}

void Pinball::onPress(Ref* sender, GameButton::Widget::TouchEventType type) {
	auto button = static_cast<GameButton*>(sender);
	int player = button->getPlayer();
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		//lockPaddleAngle(button->getPlayer());//fix paddle angle if necessary
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			_paddle[button->getPlayer()]->getPhysicsBody()->applyTorque(100);
			//_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(-PADDLE_ANG_VEL);
		}
		else {
			_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(PADDLE_ANG_VEL);
		}
		
		break;
	case ui::Widget::TouchEventType::ENDED: {
		SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
		//lockPaddleAngle(button->getPlayer());//fix paddle angle if necessary
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			_paddle[button->getPlayer()]->getPhysicsBody()->applyTorque(PADDLE_ANG_VEL);
			_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(PADDLE_ANG_VEL);
		}
		else {
			_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(-PADDLE_ANG_VEL);
		}
		break;
	}
	default:
		break;
	}
}

void Pinball::updateScore() {
	
	std::ostringstream st;
	st << _score[TEAM_TOP];
	scoreTop->setString(st.str());

	std::ostringstream sb;
	sb << _score[TEAM_BOT];
	scoreBottom->setString(sb.str());
}