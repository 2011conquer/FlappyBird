//
//  PreGame.h
//  FlappyBirdSimulator
//
//  Created by 程龙元 on 14-7-2.
//
//

#ifndef __FlappyBirdSimulator__PreGame__
#define __FlappyBirdSimulator__PreGame__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class PreGame:public Layer {
public:
    static Scene* scene();
    bool init();
    void onEnter();
    void loadingComplete(float dt);
    
    CREATE_FUNC(PreGame);
    
    void onExit();
};

#endif /* defined(__FlappyBirdSimulator__PreGame__) */
