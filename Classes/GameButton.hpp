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

#define DEFAULT_BUTTON_RADIUS 100
#define DEFAULT_BUTTON_COLOR Color4F(1,0.5f,0.5f,0.8f)

USING_NS_CC;

class GameButton : public ui::Button{
public:
    GameButton(float radius=DEFAULT_BUTTON_RADIUS, Color4F color=DEFAULT_BUTTON_COLOR);
    
    CREATE_FUNC(GameButton);

    virtual bool init();
    
    void changeColor(Color4F color);
    
private:
    float _radius;
    Color4F _color;
    
    SpriteFrame* createSpriteFrame() const;
};


#endif /* GameButton_hpp */
