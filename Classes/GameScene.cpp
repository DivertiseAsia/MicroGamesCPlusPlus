#include "GameScene.h"
#include "Taprace.h"
#include "Holdrace.h"
USING_NS_CC;

Scene* GameScene::makeTapTap(int numberOfPlayers) {
	return GameScene::createScene<Taprace>(numberOfPlayers);
}

Scene* GameScene::makeHoldrace(int numberOfPlayers) {
	return GameScene::createScene<Holdrace>(numberOfPlayers);
}

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