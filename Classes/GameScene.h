#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	GameScene(int numberOfPlayers);
	static Scene* createScene(int numberOfPlayers); //Create the scene which holds the root node.
    
    // implement the "static create()" method manually
	template <class T> static GameScene* create(int numberOfPlayers);
    // Our game custom functions
    void startGame(float);
    
    
protected:
	int numberOfPlayers;
};

#endif // __GameScene_SCENE_H__
