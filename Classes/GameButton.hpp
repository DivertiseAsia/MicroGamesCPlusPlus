//
//  GameButton.hpp
//  microgames
//
//  Created by Work on 19/9/16.
//
//

#ifndef GameButton_hpp
#define GameButton_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Ball.hpp"
#include <Box2D/Box2D.h>

#define DEFAULT_BUTTON_RADIUS 150
#define DEFAULT_BUTTON_COLOR Color4F(1,0.5f,0.5f,0.8f)

USING_NS_CC;

class GameButton : public ui::Button{
public:
    GameButton(float radius=DEFAULT_BUTTON_RADIUS, Color4F color=DEFAULT_BUTTON_COLOR, int player=0, std::string format="Hockey");
    
    CREATE_FUNC(GameButton);

    virtual bool init();
    
    void setRadius(float);
    void changeColor(Color4F color);
	void changeFormat(std::string format);
	void updateButton();
    CC_SYNTHESIZE(Ball*, _ball, Ball);
    CC_SYNTHESIZE(Touch*, _touch, Touch); //a property to hold touch
    CC_SYNTHESIZE(b2MouseJoint*, _mouseJoint, b2MouseJoint);

	void setPlayer(int player);
	int getPlayer();
private:
    float _radius;
    Color4F _color;
	int _player;
	std::string _format;
    SpriteFrame* createSpriteFrame() const;
};


#endif /* GameButton_hpp */
