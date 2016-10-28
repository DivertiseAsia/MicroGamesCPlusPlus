//
//  Pinball.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Pinball.h"
#include "GLES-Render/B2DebugDrawLayer.h"

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
	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(Pinball::onShake, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	b2Vec2 gravity = b2Vec2(0.0f, -PB_GRAVITY);
	world = new b2World(gravity);
	

	//http://www.cocos2d-x.org/wiki/Multi_resolution_support;

	Size screenSize = Director::getInstance()->getVisibleSize();
	Size fullSize = Director::getInstance()->getWinSize();
	auto screenOrigin = Director::getInstance()->getVisibleOrigin();
	auto screenCenter = Vec2(fullSize.width / 2, fullSize.height / 2);

	//Create drawNode and draw the center line
	_drawNode = DrawNode::create(10);    //Default line width
	_drawNode->drawLine(Vec2(screenOrigin.x, screenCenter.y), Vec2(screenOrigin.x + screenSize.width, screenCenter.y), Color4F::GRAY);
	this->addChild(_drawNode);

	_score[PB_TEAM_TOP] = 0;
	_score[PB_TEAM_BOT] = 0;

	scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
	scoreBottom->setBMFontSize(64);
	scoreBottom->setPosition(Vec2(screenOrigin.x+PB_SCORE_OFFSET_X, screenCenter.y - PB_SCORE_OFFSET_Y));
	scoreBottom->setRotation(90);
	this->addChild(scoreBottom, 1);

	scoreTop = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
	scoreTop->setBMFontSize(64);
	scoreTop->setPosition(Vec2(screenOrigin.x + PB_SCORE_OFFSET_X, screenCenter.y + PB_SCORE_OFFSET_Y));
	scoreTop->setRotation(90);
	this->addChild(scoreTop, 1);
	updateScore();

	//create the walls
	createWall(screenOrigin,screenSize.height);
	createWall(Vec2(screenOrigin.x + screenSize.width - PB_WALL_WIDTH, screenOrigin.y), screenSize.height);

	//boxes of fun
	createBox(Vec2(screenCenter.x, screenCenter.y - PB_BOX_YOFFSET));
	createBox(Vec2(screenCenter.x, screenCenter.y + PB_BOX_YOFFSET));

	//create the ball
	float offset = PB_BALL_RESET_OFFSET_Y;
	if (cocos2d::rand_0_1() > 0.5) {
		offset = -PB_BALL_RESET_OFFSET_Y;
	}
	auto p = Vec2(screenCenter.x+cocos2d::random(-PB_BALL_RESET_OFFSET_X, PB_BALL_RESET_OFFSET_X), screenCenter.y+offset);
	
	_ball = DrawNode::create();
	_ball->setContentSize(cocos2d::Size(DEFAULT_BALL_RADIUS*2, DEFAULT_BALL_RADIUS*2));
	_ball->drawSolidCircle(Vec2(DEFAULT_BALL_RADIUS, DEFAULT_BALL_RADIUS), DEFAULT_BALL_RADIUS, 360, 100, Color4F::MAGENTA);
	_ball->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_ball->setPosition(p);
	this->addChild(_ball);

	b2FixtureDef ballFixture;
	ballFixture.density = PB_DEFAULT_DENSITY;
	ballFixture.friction = PB_DEFAULT_FRICTION;
	ballFixture.restitution = PB_DEFAULT_RESTITUTION;
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
		_button[i] = addButtonForPlayer(i);
		_paddle[i] = addPaddleForPlayer(i,screenSize,screenCenter);
	}

	
#ifdef DEBUG_MODE
	//Debug Layer
	b2Draw *debugDraw = new GLESDebugDraw(SCALE_RATIO);
	debugDraw->SetFlags(GLESDebugDraw::e_shapeBit || GLESDebugDraw::e_jointBit);
	world->SetDebugDraw(debugDraw);
	this->addChild(B2DebugDrawLayer::create(world, SCALE_RATIO), 1000);
#endif // DEBUG_MODE

    this->world->SetContactListener(this);
	this->setName("PinballSceneRoot");

	return true;
}

void Pinball::onGameStart() {
	this->scheduleUpdate();
}

DrawNode* Pinball::addPaddleForPlayer(int player, Size screenSize, Vec2 screenCenter) {
	auto paddle = DrawNode::create();
	auto paddleLength = screenSize.width* PB_PADDLE_LENGTH_PERCENT;
	paddle->setContentSize(cocos2d::Size(PB_PADDLE_WIDTH_PX, paddleLength));
	paddle->drawSolidRect(Vec2(0, 0), Vec2(PB_PADDLE_WIDTH_PX, paddleLength), Color4F::GRAY);

	int yfix = 1;
	int xfix = 1;

	if (Shared::instance()->getPlayerPosition(player).x > screenCenter.x) {
		xfix = -1;
	}
	if (Shared::instance()->getPlayerPosition(player).y > screenCenter.y) {
		yfix = -1;
	}
	paddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_positions[player] = Vec2(Shared::instance()->getPlayerPosition(player).x + (PB_PADDLE_OFFSET_X_PERCENT*screenSize.width)*xfix, Shared::instance()->getPlayerPosition(player).y + (DEFAULT_BUTTON_RADIUS)*yfix);
	paddle->setPosition(_positions[player]);
	this->addChild(paddle);
	paddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	b2FixtureDef boxFixture;
	boxFixture.density = PB_DEFAULT_DENSITY;
	boxFixture.friction = PB_DEFAULT_FRICTION;
	boxFixture.restitution = PB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(PB_PADDLE_WIDTH_PX / 2 / SCALE_RATIO, paddleLength / 2 / SCALE_RATIO);
	boxShape.m_centroid = b2Vec2(0, 0);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_dynamicBody;
	boxBodyDef.userData = paddle;
	boxBodyDef.position.Set(paddle->getPosition().x / SCALE_RATIO, paddle->getPosition().y / SCALE_RATIO);

	auto pB = world->CreateBody(&boxBodyDef);
	pB->CreateFixture(&boxFixture);
	
	

	auto padControl = DrawNode::create();
	padControl->setContentSize(cocos2d::Size(PB_PADDLE_CONTROL_RAD * 2, PB_PADDLE_CONTROL_RAD * 2));
	padControl->drawSolidCircle(Vec2(PB_PADDLE_CONTROL_RAD, PB_PADDLE_CONTROL_RAD), PB_PADDLE_CONTROL_RAD, 360, 100, Shared::instance()->getPlayerColor(player));
	padControl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	padControl->setPosition(_positions[player]);
	this->addChild(padControl);
	b2FixtureDef circleFixture;
	circleFixture.density = PB_DEFAULT_DENSITY;
	circleFixture.friction = PB_DEFAULT_FRICTION;
	circleFixture.restitution = PB_DEFAULT_RESTITUTION;
	b2CircleShape circleShape;
	circleShape.m_radius = PB_PADDLE_CONTROL_RAD / SCALE_RATIO;
	circleFixture.shape = &circleShape;
	b2BodyDef buttonBodyDef;
	buttonBodyDef.type = b2BodyType::b2_kinematicBody;
	buttonBodyDef.userData = NULL;
	buttonBodyDef.position.Set(_positions[player].x / SCALE_RATIO, _positions[player].y / SCALE_RATIO);
	auto bb = world->CreateBody(&buttonBodyDef);
	bb->CreateFixture(&circleFixture);

	paddleControlBody[player] = bb;

	b2WeldJointDef revoluteJointDef;
	revoluteJointDef.bodyA = bb;
	revoluteJointDef.bodyB = pB;

	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(0, 0);//the center
	revoluteJointDef.localAnchorB.Set(-PB_PADDLE_WIDTH_PX / 2 / SCALE_RATIO, 0);//center of the circle

	world->CreateJoint(&revoluteJointDef);

	return paddle;
}

GameButton* Pinball::addButtonForPlayer(int player) {
	auto button = GameButton::create();
	if (numberOfPlayers < 3 && player == SHARED_PLAYER3) {
		button->changeColor(Shared::instance()->getPlayerColor(SHARED_PLAYER2));
	}
	else if (numberOfPlayers < 4 && player == SHARED_PLAYER4) {
		button->changeColor(Shared::instance()->getPlayerColor(SHARED_PLAYER1));
	}
	else {
		button->changeColor(Shared::instance()->getPlayerColor(player));
	}
	button->setPlayer(player);
	auto currentPosition = Shared::instance()->getPlayerPosition(player);
	button->setPosition(currentPosition);
	button->setTag(player);  //Set the number to indicate button order.
	button->addTouchEventListener(CC_CALLBACK_2(Pinball::onPress, this));
	this->addChild(button);

	b2FixtureDef circleFixture;
	circleFixture.density = PB_DEFAULT_DENSITY;
	circleFixture.friction = PB_DEFAULT_FRICTION;
	circleFixture.restitution = PB_DEFAULT_RESTITUTION;
	b2CircleShape circleShape;
	circleShape.m_radius = DEFAULT_BUTTON_RADIUS / SCALE_RATIO;
	circleFixture.shape = &circleShape;
	b2BodyDef buttonBodyDef;
	buttonBodyDef.type = b2BodyType::b2_staticBody;
	buttonBodyDef.userData = NULL;
	buttonBodyDef.position.Set(button->getPosition().x / SCALE_RATIO, button->getPosition().y / SCALE_RATIO);

	auto bb = world->CreateBody(&buttonBodyDef);
	bb->CreateFixture(&circleFixture);

	return button;
}

void Pinball::onShake(cocos2d::Acceleration *acc, cocos2d::Event *event) {
	if (ballBody->GetLinearVelocity().Length() == 0) {
		ballBody->ApplyLinearImpulse(b2Vec2(acc->x*100, acc->y*100),ballBody->GetWorldCenter(),true);
	}
}

void Pinball::createWall(Vec2 position, float height) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(PB_WALL_WIDTH, height));
	drawNode->drawSolidRect(Vec2::ZERO, Vec2(PB_WALL_WIDTH, height), Color4F::GRAY);
	drawNode->setPosition(position);

	b2FixtureDef boxFixture;
	boxFixture.density = PB_DEFAULT_DENSITY;
	boxFixture.friction = PB_DEFAULT_FRICTION;
	boxFixture.restitution = PB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(PB_WALL_WIDTH / 2 / SCALE_RATIO, height / SCALE_RATIO);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_staticBody;
	boxBodyDef.userData = drawNode;
	boxBodyDef.position.Set(drawNode->getPosition().x / SCALE_RATIO, drawNode->getPosition().y / SCALE_RATIO);

	auto boxBody1 = world->CreateBody(&boxBodyDef);
	boxBody1->CreateFixture(&boxFixture);
	
	this->addChild(drawNode);
}

void Pinball::createBox(Vec2 position) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(PB_BOX_SIZE, PB_BOX_SIZE));
	drawNode->drawSolidRect(Vec2::ZERO, Vec2(PB_BOX_SIZE, PB_BOX_SIZE), Color4F::GRAY);;
	drawNode->setPosition(position);
	drawNode->setRotation(45);
	drawNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	b2FixtureDef boxFixture;
	boxFixture.density = PB_DEFAULT_DENSITY;
	boxFixture.friction = PB_DEFAULT_FRICTION;
	boxFixture.restitution = PB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(PB_BOX_SIZE / 2 / SCALE_RATIO, PB_BOX_SIZE / 2 / SCALE_RATIO);
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
			sprite->setPosition(Vec2(body->GetPosition().x * SCALE_RATIO, body->GetPosition().y * SCALE_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}
	world->ClearForces();
	world->DrawDebugData();

	float gameHeight = Director::getInstance()->getVisibleSize().height;
	float ballY = _ball->getPositionY();
	if (ballY < gameHeight/2) {
		world->SetGravity(b2Vec2(0.0f, -PB_GRAVITY));
	}
	else {
		world->SetGravity(b2Vec2(0.0f, PB_GRAVITY));
	}

	if (ballY > gameHeight || ballY < 0) {
		//give score
		int offsetYModifier = 1;
		if (ballY > gameHeight) {
            SoundManager::instance()->playEffect(SOUND_FILE_WIN);
			_score[PB_TEAM_BOT]++;
			offsetYModifier = -1;
			if (_score[PB_TEAM_BOT] >= PB_POINTS_TO_WIN) {
				if (numberOfPlayers > 2)
				{
					int winners[] = PB_TEAM_BOT_PLAYERS;
					endGame(winners, 2);
				}
				else {
					endGame(SHARED_PLAYER2);
				}
			}
		}
		else {
            SoundManager::instance()->playEffect(SOUND_FILE_WIN);
			_score[PB_TEAM_TOP]++;
			offsetYModifier = 1;
			
			if (_score[PB_TEAM_TOP] >= PB_POINTS_TO_WIN) {
				if (numberOfPlayers > 3)
				{
					int winners[] = PB_TEAM_TOP_PLAYERS;
					endGame(winners,2);
				}
				else {
					endGame(SHARED_PLAYER1);
				}
			}
		}
		float offset_x = cocos2d::random(-PB_BALL_RESET_OFFSET_X, PB_BALL_RESET_OFFSET_X);
		if (offset_x == 0) {
			offset_x = 0.5f;
		}
		_ball->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + offset_x,
			gameHeight / 2 + offsetYModifier*PB_BALL_RESET_OFFSET_Y));
		//reset ball transform & velocity
		ballBody->SetTransform(b2Vec2(_ball->getPosition().x / SCALE_RATIO, _ball->getPosition().y / SCALE_RATIO),0);
		ballBody->SetLinearVelocity(b2Vec2_zero);
		ballBody->SetAngularVelocity(0);
		updateScore();
	}
	
}

//This method will be called on the Node entered.
void Pinball::onEnter() {
	Node::onEnter();
	startGame();
}

void Pinball::startGame() {
	GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
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
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			paddleControlBody[player]->SetAngularVelocity(-PB_PADDLE_ANG_VEL);
		}
		else {
			paddleControlBody[player]->SetAngularVelocity(PB_PADDLE_ANG_VEL);
		}
		
		break;
	case ui::Widget::TouchEventType::ENDED: {
		SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			paddleControlBody[player]->SetAngularVelocity(PB_PADDLE_ANG_VEL);
		}
		else {
			paddleControlBody[player]->SetAngularVelocity(-PB_PADDLE_ANG_VEL);
		}
		break;
	}
	default:
		break;
	}
}

void Pinball::updateScore() {
	scoreTop->setString(Shared::intToString(_score[PB_TEAM_TOP]));
	scoreBottom->setString(Shared::intToString(_score[PB_TEAM_BOT]));
}

void Pinball::BeginContact(b2Contact* contact) {
    SoundManager::instance()->playEffect(SOUND_FILE_HOCKEY_PUCK);
}

void Pinball::EndContact(b2Contact* contact) {
    
}
