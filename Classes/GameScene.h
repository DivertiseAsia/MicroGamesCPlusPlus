#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	enum GameStatus { GAME_START, GAME_INPROGRESS, GAME_OVER };
	GameScene(int numberOfPlayers);
	template <class T> static Scene* createScene(int numberOfPlayers); //Create the scene which holds the root node.
    
    // implement the "static create()" method manually
	template <class T> static GameScene* create(int numberOfPlayers);
    // Our game custom functions
    void startGame(float);
	void endGame(int[]);
    
protected:
	void showReturnMenu(float);
	int numberOfPlayers;
	GameStatus gameStatus;
};

#endif // __GameScene_SCENE_H__
