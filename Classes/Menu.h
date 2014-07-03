//
//  Menu.h
//  FlappyBirdSimulator
//
//  Created by 程龙元 on 14-7-2.
//
//

#ifndef __FlappyBirdSimulator__Menu__
#define __FlappyBirdSimulator__Menu__

#include <iostream>
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;

class GameMenu:public Layer
{
public:

    CREATE_FUNC(GameMenu);
    
    static Scene *scene();
    
    bool init();
    
    void startGame(Ref *sender);
    
};

#endif /* defined(__FlappyBirdSimulator__Menu__) */
