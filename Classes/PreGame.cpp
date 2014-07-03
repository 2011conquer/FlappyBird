//
//  PreGame.cpp
//  FlappyBirdSimulator
//
//  Created by 程龙元 on 14-7-2.
//
//

#include "PreGame.h"
#include "Menu.h"

Scene* PreGame::scene()
{
    Scene *scene = Scene::create();
    PreGame *layer = PreGame::create();
    scene->addChild(layer);
    return scene;
}

bool PreGame::init()
{
    if (!Layer::init()) {
        return false;
    }
    return true;
}

void PreGame::onEnter()
{
    Layer::onEnter();
    Size size = Director::getInstance()->getWinSize();
    
    Sprite *bird = Sprite::create("bird_hero.png");
    bird->setPosition(Point(size.width/2, size.height/2));
    bird->setScale(2);
    this->addChild(bird);
    
    Label *label = Label::createWithSystemFont("MadeByZhangGuorui", "Helvetica", 30);
    label->setPosition(Point(size.width/2, 100));
    label->setColor(Color3B::ORANGE);
    this->addChild(label);
    
    this->scheduleOnce(schedule_selector(PreGame::loadingComplete), 2);
}

void PreGame::loadingComplete(float dt)
{
    TransitionFade *transition = TransitionFade::create(1, GameMenu::scene(), Color3B(0, 0, 0));
    Director::getInstance()->replaceScene(transition);
}

void PreGame::onExit()
{
    this->unscheduleAllSelectors();
    Layer::onExit();
}