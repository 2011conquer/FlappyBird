//
//  Menu.cpp
//  FlappyBirdSimulator
//
//  Created by 程龙元 on 14-7-2.
//
//

#include "Menu.h"
Scene *GameMenu::scene()
{
    Scene *scene = Scene::create();
    GameMenu *layer = GameMenu::create();
    scene->addChild(layer);
    return scene;
}

bool GameMenu::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    LayerColor *layerColor = LayerColor::create(Color4B(223, 253, 253, 255), size.width, size.height);
    this->addChild(layerColor);
    
//    Label *label = Label::createWithBMFont("fonts.fnt", "Miku Run");
//    label->setScale(0.7);
//    label->setPosition(Point(130, size.height-50));
//    this->addChild(label);
    
    MenuItemImage *startItem = MenuItemImage::create("bird_start_btn.png", "brid_start_btn_pressed.png",CC_CALLBACK_1(GameMenu::startGame, this));
    startItem->setScale(1.5);
    
    auto menu = Menu::create(startItem,NULL);
    menu->alignItemsVerticallyWithPadding(20);
    //menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(Point(160,size.height/2));
    this->addChild(menu, 1);
    
    return true;
}

void GameMenu::startGame(Ref *sender)
{
    TransitionFade *transition = TransitionFade::create(1.5, HelloWorld::createScene(), Color3B(255, 255, 255));
    Director::getInstance()->replaceScene(transition);
}