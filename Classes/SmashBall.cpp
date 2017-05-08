#include "SmashBall.h"
#include "GameList.h"
#include "GLES-Render/B2DebugDrawLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool SmashBall::init()
{
	//////////////////////////////
	// 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
	{
		return false;
	}
	GameScene::gameType = GameType::SMASHBALL;

	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(SmashBall::onShake, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	b2Vec2 gravity = b2Vec2(0.0f, -SB_GRAVITY);
	world = new b2World(gravity);
	

	//http://www.cocos2d-x.org/wiki/Multi_resolution_support;

	Size screenSize = Director::getInstance()->getVisibleSize();
	Size fullSize = Director::getInstance()->getWinSize();
	auto screenOrigin = Director::getInstance()->getVisibleOrigin();
	auto screenCenter = Vec2(fullSize.width / 2, fullSize.height / 2);

	// Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_Smashball.png", this);

	//Create drawNode and draw the center line
	//_drawNode = DrawNode::create(10);    //Default line width
	//_drawNode->drawLine(Vec2(screenOrigin.x, screenCenter.y), Vec2(screenOrigin.x + screenSize.width, screenCenter.y), Color4F::GRAY);
	//this->addChild(_drawNode);

	setupScore(0, Vec2(screenOrigin.x + SB_SCORE_OFFSET_X, screenCenter.y - SB_SCORE_OFFSET_Y), Vec2(screenOrigin.x + SB_SCORE_OFFSET_X, screenCenter.y + SB_SCORE_OFFSET_Y));

	//create the walls
	createWall(screenOrigin,screenSize.height);
	createWall(Vec2(screenOrigin.x + screenSize.width - SB_WALL_WIDTH, screenOrigin.y), screenSize.height);

	//boxes of fun
	createBox(Vec2(screenCenter.x, screenCenter.y - SB_BOX_YOFFSET), "item/Item_BlockUpper.png");
	createBox(Vec2(screenCenter.x, screenCenter.y + SB_BOX_YOFFSET), "item/Item_BlockLower.png");

	//create the ball
	float offset = SB_BALL_RESET_OFFSET_Y;
	if (cocos2d::rand_0_1() > 0.5) {
		offset = -SB_BALL_RESET_OFFSET_Y;
	}
	auto p = Vec2(screenCenter.x+cocos2d::random(-SB_BALL_RESET_OFFSET_X, SB_BALL_RESET_OFFSET_X), screenCenter.y+offset);
	createBall(p, DEFAULT_BALL_RADIUS, "item/Item_Ball.png");

	//create the controls and paddles
	for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
		_button[i] = addButtonForPlayer(i, gameType);
		_paddle[i] = addPaddleForPlayer(i,screenSize,screenCenter);
	}

	
#ifdef DEBUG_MODE
	//Debug Layer
	b2Draw *debugDraw = new GLESDebugDraw(SB_SCALE_RATIO);
	debugDraw->SetFlags(GLESDebugDraw::e_shapeBit || GLESDebugDraw::e_jointBit);
	world->SetDebugDraw(debugDraw);
	this->addChild(B2DebugDrawLayer::create(world, SB_SCALE_RATIO), 1000);
#endif // DEBUG_MODE

    this->world->SetContactListener(this);
	this->setName("SmashBallSceneRoot");

	return true;
}

DrawNode* SmashBall::addPaddleForPlayer(int player, Size screenSize, Vec2 screenCenter) {
	float paddlePicOffset = 0.05;

	std::string fname = "item/Item_Smashball_Spinner" + std::to_string(player + 1) + ".png";
	auto spritePaddle = Sprite::create(fname);
	spritePaddle->setScale(screenSize.width * SMB_PADDLE_LENGTH_PERCENT / spritePaddle->getContentSize().height - paddlePicOffset);

	auto paddle = DrawNode::create();
	auto paddleLength = screenSize.width * SMB_PADDLE_LENGTH_PERCENT;

	spritePaddle->setPositionY(paddleLength / 2);

	paddle->setContentSize(cocos2d::Size(SB_PADDLE_WIDTH_PX, paddleLength));
	paddle->addChild(spritePaddle);
	//paddle->drawSolidRect(Vec2(0, 0), Vec2(SB_PADDLE_WIDTH_PX, paddleLength), Color4F::GRAY);

	int yfix = 1;
	int xfix = 1;

	if (Shared::instance()->getPlayerPosition(player).x > screenCenter.x) {
		xfix = -1;
	}
	if (Shared::instance()->getPlayerPosition(player).y > screenCenter.y) {
		yfix = -1;
	}
	paddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_positions[player] = Vec2(Shared::instance()->getPlayerPosition(player).x + (SMB_PADDLE_OFFSET_X_PERCENT*screenSize.width)*xfix, 
		Shared::instance()->getPlayerPosition(player).y + (DEFAULT_BUTTON_RADIUS)*yfix);
	paddle->setPosition(_positions[player]);
	this->addChild(paddle);
	paddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	b2FixtureDef boxFixture;
	boxFixture.density = SB_DEFAULT_DENSITY;
	boxFixture.friction = SB_DEFAULT_FRICTION;
	boxFixture.restitution = SB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(SB_PADDLE_WIDTH_PX / 2 / SB_SCALE_RATIO, paddleLength / 2 / SB_SCALE_RATIO);
	boxShape.m_centroid = b2Vec2(0, 0);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_dynamicBody;
	boxBodyDef.userData = paddle;
	boxBodyDef.position.Set(paddle->getPosition().x / SB_SCALE_RATIO, paddle->getPosition().y / SB_SCALE_RATIO);

	auto pB = world->CreateBody(&boxBodyDef);
	pB->CreateFixture(&boxFixture);	

	auto padControl = DrawNode::create();
	padControl->setContentSize(cocos2d::Size(SB_PADDLE_CONTROL_RAD * 2, SB_PADDLE_CONTROL_RAD * 2));
	//padControl->drawSolidCircle(Vec2(SB_PADDLE_CONTROL_RAD, SB_PADDLE_CONTROL_RAD), SB_PADDLE_CONTROL_RAD, 360, 100, Shared::instance()->getPlayerColor(player));
	padControl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	padControl->setPosition(_positions[player]);
	this->addChild(padControl);
	b2FixtureDef circleFixture;
	circleFixture.density = SB_DEFAULT_DENSITY;
	circleFixture.friction = SB_DEFAULT_FRICTION;
	circleFixture.restitution = SB_DEFAULT_RESTITUTION;
	b2CircleShape circleShape;
	circleShape.m_radius = SB_PADDLE_CONTROL_RAD / SB_SCALE_RATIO;
	circleFixture.shape = &circleShape;
	b2BodyDef buttonBodyDef;
	buttonBodyDef.type = b2BodyType::b2_kinematicBody;
	buttonBodyDef.userData = NULL;
	buttonBodyDef.position.Set(_positions[player].x / SB_SCALE_RATIO, _positions[player].y / SB_SCALE_RATIO);
	auto bb = world->CreateBody(&buttonBodyDef);
	bb->CreateFixture(&circleFixture);

	paddleControlBody[player] = bb;

	b2WeldJointDef revoluteJointDef;
	revoluteJointDef.bodyA = bb;
	revoluteJointDef.bodyB = pB;

	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(0, 0);//the center
	revoluteJointDef.localAnchorB.Set(-SB_PADDLE_WIDTH_PX / 2 / SB_SCALE_RATIO, 0);//center of the circle

	world->CreateJoint(&revoluteJointDef);

	return paddle;
}

void SmashBall::update(float dt) {
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}

	updatePhysics(dt);

	float gameHeight = Director::getInstance()->getVisibleSize().height;
	float ballY = _ball->getPositionY();
	if (ballY < gameHeight/2) {
		world->SetGravity(b2Vec2(0.0f, -SB_GRAVITY));
	}
	else {
		world->SetGravity(b2Vec2(0.0f, SB_GRAVITY));
	}

	if (ballY > gameHeight || ballY < 0) {
		//give score
		int offsetYModifier = 1;
		if (ballY > gameHeight) {
            SoundManager::instance()->playEffect(SOUND_FILE_WIN);
			addScore(SB_TEAM_BOT, 1);
			offsetYModifier = -1;
		}
		else {
            SoundManager::instance()->playEffect(SOUND_FILE_WIN);
			addScore(SB_TEAM_TOP, 1);
			offsetYModifier = 1;
		}
		float offset_x = cocos2d::random(-SB_BALL_RESET_OFFSET_X, SB_BALL_RESET_OFFSET_X);
		if (offset_x == 0) {
			offset_x = 0.5f;
		}
		_ball->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + offset_x,
			gameHeight / 2 + offsetYModifier*SB_BALL_RESET_OFFSET_Y));
		//reset ball transform & velocity
		ballBody->SetTransform(b2Vec2(_ball->getPosition().x / SB_SCALE_RATIO, _ball->getPosition().y / SB_SCALE_RATIO),0);
		ballBody->SetLinearVelocity(b2Vec2_zero);
		ballBody->SetAngularVelocity(0);
		updateScore();
	}
	
}

void SmashBall::onPress(Ref* sender, GameButton::Widget::TouchEventType type) {
	auto button = static_cast<GameButton*>(sender);
	int player = button->getPlayer();
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			paddleControlBody[player]->SetAngularVelocity(-SMB_PADDLE_ANG_VEL);
		}
		else {
			paddleControlBody[player]->SetAngularVelocity(SMB_PADDLE_ANG_VEL);
		}
		
		break;
	case ui::Widget::TouchEventType::ENDED: {
		SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
		if (player == SHARED_PLAYER1 || player == SHARED_PLAYER2) {
			paddleControlBody[player]->SetAngularVelocity(SMB_PADDLE_ANG_VEL);
		}
		else {
			paddleControlBody[player]->SetAngularVelocity(-SMB_PADDLE_ANG_VEL);
		}
		break;
	}
	default:
		break;
	}
}