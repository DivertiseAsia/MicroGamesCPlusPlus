#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#define GS_WINNER_TEXT_OFFSET_Y 20
#define GS_GO_TEXT_WIDTH_PERCENT .5f
#define GS_COUNTDOWN_TEXT_WIDTH_PERCENT .08f
#define GS_WINNER_TEXT_WIDTH_PERCENT .85f
#define GS_RETURN_TEXT_WIDTH_PERCENT .95f
#define GS_WINNER_TEXT	"Winner"
#define GS_RETURN_TEXT	"Press anywhere to go home"
#define GS_GO_TEXT		"Go!"

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::LayerColor
{
public:
	enum GameStatus { GAME_START, GAME_INPROGRESS, GAME_OVER };
	GameScene(int numberOfPlayers);
	template <class T> static Scene* createScene(int numberOfPlayers); //Create the scene which holds the root node.
    
    // implement the "static create()" method manually
	template <class T> static GameScene* create(int numberOfPlayers);
    // Our game custom functions
    void startGame(float);
	void endGame(int);
	void endGame(int[],int);
    void updateCounter(float);
    void showText(std::string, float);
    
protected:
	void showReturnMenu(float);
	int numberOfPlayers;
	GameStatus gameStatus;
    int _counter = 0;
};

#endif // __GameScene_SCENE_H__
