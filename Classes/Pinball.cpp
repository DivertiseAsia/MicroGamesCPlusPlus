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
#include "Box2D/Box2D.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool Pinball::init()
{
	//////////////////////////////
	// 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
	{
		return false;
	}

	b2Vec2 gravity = b2Vec2(0.0f, -GRAVITY);
	world = new b2World(gravity);
	b2Draw *debugDraw = new GLESDebugDraw(SCALE_RATIO);

	debugDraw->SetFlags(GLESDebugDraw::e_shapeBit);

	world->SetDebugDraw(debugDraw);

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
	createWall(Vec2(0, 0),screenSize.height);
	createWall(Vec2(winSize.width - 5, 0), screenSize.height);

	//boxes of fun
	createBox(Vec2(screenCenter.x, screenCenter.y - BOX_YOFFSET));
	createBox(Vec2(screenCenter.x, screenCenter.y + BOX_YOFFSET));

	//create the ball
	float offset = BALL_RESET_OFFSET_Y;
	if (cocos2d::rand_0_1() > 0.5) {
		offset = -BALL_RESET_OFFSET_Y;
	}
	auto p = Vec2(screenCenter.x+cocos2d::random(-BALL_RESET_OFFSET_X, BALL_RESET_OFFSET_X), screenCenter.y+offset);
	
	_ball = DrawNode::create();
	_ball->setContentSize(cocos2d::Size(DEFAULT_BALL_RADIUS*2, DEFAULT_BALL_RADIUS*2));
	_ball->drawSolidCircle(Vec2(DEFAULT_BALL_RADIUS, DEFAULT_BALL_RADIUS), DEFAULT_BALL_RADIUS, 360, 100, Color4F::MAGENTA);
	_ball->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_ball->setPosition(p);
	this->addChild(_ball);

	b2FixtureDef ballFixture;
	ballFixture.density = 10;
	ballFixture.friction = 0.8;
	ballFixture.restitution = 0.6;
	b2CircleShape ballShape;
	ballShape.m_radius = DEFAULT_BALL_RADIUS / SCALE_RATIO;
	ballFixture.shape = &ballShape;
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2BodyType::b2_dynamicBody;
	ballBodyDef.userData = _ball;
	ballBodyDef.position.Set(_ball->getPosition().x / SCALE_RATIO, _ball->getPosition().y / SCALE_RATIO);
	
	ballBody = world->CreateBody(&ballBodyDef);
	ballBody->CreateFixture(&ballFixture);
	ballBody->SetGravityScale(1);

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


		//auto body = PhysicsBody::createCircle(DEFAULT_BUTTON_RADIUS, PhysicsMaterial());
		//body->setCategoryBitmask(CAT_MASK_STAT);
		//body->setCollisionBitmask(CAT_MASK_STAT);
		//body->setDynamic(false);
		//_button[i]->addComponent(body);

		//paddle
		_paddle[i] = DrawNode::create();
		_paddle[i]->setContentSize(cocos2d::Size(PADDLE_WIDTH_PX, screenSize.width* PADDLE_LENGTH_PERCENT));
		_paddle[i]->drawSolidRect(Vec2(0, 0), Vec2(PADDLE_WIDTH_PX, screenSize.width * PADDLE_LENGTH_PERCENT), Color4F::GRAY);
		//auto paddlebody = PhysicsBody::createBox(Size(PADDLE_WIDTH_PX, screenSize.width * PADDLE_LENGTH_PERCENT), PhysicsMaterial(1, .99f, .5f));
		//paddlebody->setMass(10);
		//paddlebody->setAngularVelocityLimit(10);
		//paddlebody->setVelocityLimit(0);
		//paddlebody->setGravityEnable(false);
		//paddlebody->setCategoryBitmask(CAT_MASK_PADDLE);
		//paddlebody->setCategoryBitmask(CAT_MASK_BALL);
		//_paddle[i]->addComponent(paddlebody);
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
		_paddle[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		_positions[i] = Vec2(Shared::instance()->getPlayerPosition(i).x + (DEFAULT_BUTTON_RADIUS)*xfix, Shared::instance()->getPlayerPosition(i).y + (DEFAULT_BUTTON_RADIUS)*yfix);
		_paddle[i]->setPosition(_positions[i]);
		_paddle[i]->setRotation(startAngle);
		this->addChild(_paddle[i]);


		/*_paddleButt[i] = DrawNode::create();
		_paddleButt[i]->setContentSize(cocos2d::Size(90, 90));
		_paddleButt[i]->drawSolidCircle(Vec2(45,45), 45, 360, 100, Color4F::MAGENTA);
		//body = PhysicsBody::createCircle(45, PhysicsMaterial(1, .95f, 1));
		//body->setDynamic(false);
		//_paddleButt[i]->addComponent(body);
		_paddleButt[i]->setPosition(Vec2(_positions[i].x+xfix * -10,_positions[i].y));
		if (i == SHARED_PLAYER2 || i == SHARED_PLAYER4) {
			_paddleButt[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		}
		else {
			_paddleButt[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		}
		
		this->addChild(_paddleButt[i]);
		*/

		this->addChild(_button[i]);
	}

	//Debug Layer
	this->addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 1000);

	this->setName("PinballSceneRoot");
	this->initTouchHandling();
	this->scheduleUpdate();

	return true;
}

void Pinball::createWall(Vec2 position, float height) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(5, height));
	drawNode->drawSolidRect(position, Vec2(position.x+5, height), Color4F::RED);
	drawNode->setPosition(position);

	b2FixtureDef boxFixture;
	boxFixture.density = 10;
	boxFixture.friction = 0.8;
	boxFixture.restitution = 0.6;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(5.0f / SCALE_RATIO, height / SCALE_RATIO);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_staticBody;
	boxBodyDef.userData = drawNode;
	boxBodyDef.position.Set(drawNode->getPosition().x / SCALE_RATIO, drawNode->getPosition().y / SCALE_RATIO);
	
	this->addChild(drawNode);
}

void Pinball::createBox(Vec2 position) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(BOX_SIZE, BOX_SIZE));
	drawNode->drawSolidRect(Vec2(0, 0), Vec2(BOX_SIZE, BOX_SIZE), Color4F::GRAY);;
	drawNode->setPosition(position);
	drawNode->setRotation(45);
	drawNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	b2FixtureDef boxFixture;
	boxFixture.density = 10;
	boxFixture.friction = 0.8;
	boxFixture.restitution = 0.6;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(BOX_SIZE / SCALE_RATIO, BOX_SIZE / SCALE_RATIO);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_staticBody;
	boxBodyDef.userData = drawNode;
	boxBodyDef.angle = CC_DEGREES_TO_RADIANS(drawNode->getRotation());
	boxBodyDef.position.Set(drawNode->getPosition().x / SCALE_RATIO, drawNode->getPosition().y / SCALE_RATIO);

	auto boxBody1 = world->CreateBody(&boxBodyDef);
	boxBody1->CreateFixture(&boxFixture);

	this->addChild(drawNode);
}

void Pinball::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated) {
	Layer::draw();
	world->DrawDebugData();
}

void Pinball::update(float dt) {
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}

	int positionIterations = 10;
	int velocityIterations = 10;

	deltaTime = dt;
	world->Step(dt, velocityIterations, positionIterations);

	for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData())
		{
			DrawNode *sprite = (DrawNode *)body->GetUserData();
			sprite->setPosition(ccp(body->GetPosition().x * SCALE_RATIO, body->GetPosition().y * SCALE_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}
	world->ClearForces();
	world->DrawDebugData();

	float gameHeight = Director::getInstance()->getVisibleSize().height;
	float ballY = _ball->getPositionY();
	if (ballY < gameHeight/2) {
		world->SetGravity(b2Vec2(0.0f, -GRAVITY));
	}
	else {
		world->SetGravity(b2Vec2(0.0f, GRAVITY));
	}

	if (ballY > gameHeight || ballY < 0) {
		//give score
		int offsetYModifier = 1;
		if (ballY > gameHeight) {
			_score[TEAM_BOT]++;
			offsetYModifier = -1;
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
			offsetYModifier = 1;
			
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
		float offset_x = cocos2d::random(-BALL_RESET_OFFSET_X, BALL_RESET_OFFSET_X);
		if (offset_x == 0) {
			offset_x = 0.5f;
		}
		_ball->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + offset_x,
			gameHeight / 2 + offsetYModifier*BALL_RESET_OFFSET_Y));
		//reset ball transform & velocity
		ballBody->SetTransform(b2Vec2(_ball->getPosition().x / SCALE_RATIO, _ball->getPosition().y / SCALE_RATIO),0);
		ballBody->SetLinearVelocity(b2Vec2_zero);
		ballBody->SetAngularVelocity(0);
		updateScore();
	}
	/*lockPaddleAngle(SHARED_PLAYER1);
	lockPaddleAngle(SHARED_PLAYER2);
	lockPaddleAngle(SHARED_PLAYER3);
	lockPaddleAngle(SHARED_PLAYER4);*/
	
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
	Size winSize = Director::getInstance()->getWinSize();
	auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);

	/*for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
		int xfix = 1;
		int yfix = 1;
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
		//Vec2 pos = Vec2(Shared::instance()->getPlayerPosition(i).x + (DEFAULT_BUTTON_RADIUS+5)*xfix, Shared::instance()->getPlayerPosition(i).y + (DEFAULT_BUTTON_RADIUS+5)*yfix);
		//auto joint = PhysicsJointDistance::construct(_paddleButt[i]->getPhysicsBody(), _paddle[i]->getPhysicsBody(), Vec2(0, 0), Vec2(0, 0));
		//auto joint = PhysicsJointLimit::construct(_paddleButt[i]->getPhysicsBody(), _paddle[i]->getPhysicsBody(), Vec2(0,0),Vec2(0,0));
		//joint->createConstraints();
		//joint->setDistance(30);
		//joint->setMax(30);
		//joint->setMaxForce(10);
		//joint->setMin(100);
		//joint->setCollisionEnable(false);	

		//this->getScene()->getPhysicsWorld()->addJoint(joint);
	}
	this->getScene()->getPhysicsWorld()->setGravity(Vec2::ZERO);
	*/
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
			//_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(-PADDLE_ANG_VEL);
		}
		else {
			//_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(PADDLE_ANG_VEL);
		}
		
		break;
	case ui::Widget::TouchEventType::ENDED: {
		SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
		//lockPaddleAngle(button->getPlayer());//fix paddle angle if necessary
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			//_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(PADDLE_ANG_VEL);
		}
		else {
			//_paddle[button->getPlayer()]->getPhysicsBody()->setAngularVelocity(-PADDLE_ANG_VEL);
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