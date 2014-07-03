#include "HelloWorldScene.h"

USING_NS_CC;

#pragma mark ---创建及初始化---
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
  
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan =[=](Touch *touch,Event *event)
    {
        this->startGame();
        _eventDispatcher->removeEventListener(listener);
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite *back = Sprite::create("bird_bg.png");
    back->setScaleX(1.2);
    back->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(back,0,0);
    
    this->initPhysics();//初始化物理世界
    
    this->addBird(Point(visibleSize.width/2-50,visibleSize.height/2));//添加小鸟
    
    this->addBarContainer();//添加容器
    
    this->addGround();//添加地面
    
    return true;
}

void HelloWorld::startGame()
{
    this->scheduleUpdate();
    //每隔一秒出现一个管道
    this->schedule(schedule_selector(HelloWorld::addBar), 1);
}
void HelloWorld::endGame()
{
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBar));
    
}

#pragma mark ---初始化物理世界---
void HelloWorld::initPhysics()
{
    Size size = Director::getInstance()->getWinSize();
    
    b2Vec2 gravity;//设置重力
    gravity.Set(0.0f, -15.0f);
    world = new b2World(gravity);
    //刚体不再进行运动模拟时可以将其设置为睡眠
    world->SetContinuousPhysics(false);
    
    //添加box2d世界中的事件监听，当两个物体碰撞时做相应处理
   // contactListener  = new ContactListener();
    world->SetContactListener(this);//设置碰撞检测
    
/*
    //GLESDebugDraw参数表示比例，32：1
    //创建绘制器
    m_debugDraw = new GLESDebugDraw(PTM_RATIO);
    world->SetDebugDraw(m_debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
*/
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
}
void HelloWorld::update(float dt)
{
    int velocityIterations = 8;//速度迭代次数
    int positionIterations = 3;//位置迭代次数
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body *b = world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            Sprite *myActor = (Sprite *)b->GetUserData();
            myActor->setPosition(Point(b->GetPosition().x * PTM_RATIO, b->GetPosition().y *PTM_RATIO));
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            //当刚体移出屏幕，删除
            if (b->GetPosition().x<-3) {
                if (myActor!=NULL) {
                    myActor->removeFromParent();//删除精灵节点
                }
                world->DestroyBody(b);//毁掉刚体
            }
        }
    }
}

#pragma mark  ---添加精灵---
void HelloWorld::addGround()
{
    Texture2D *groundTexture = TextureCache::sharedTextureCache()->addImage("background.png");
    Sprite *ground = Sprite::createWithTexture(groundTexture);
    Size groundSize = ground->getContentSize();
    
    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    bDef.userData = ground;
    bDef.position = b2Vec2(groundSize.width/2/PTM_RATIO,groundSize.height/2/PTM_RATIO);
    b2Body *groundBody = world->CreateBody(&bDef);
    
    //创建形状
    b2PolygonShape groundShape;
    groundShape.SetAsBox(groundSize.width/2/PTM_RATIO, groundSize.height/2/PTM_RATIO);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody->CreateFixture(&groundFixtureDef);//用刚体创建夹具
    
    this->addChild(ground);
}

//创建上下的管道
void HelloWorld::addBar(float dt)
{
    float offset = -rand()%5;//偏移的随机数
    
    //downBar下方管道
    Texture2D *barTexture = TextureCache::sharedTextureCache()->addImage("obstacle_down.png");
    Sprite *downBar = Sprite::createWithTexture(barTexture);
    Size downBarSize = downBar->getContentSize();
    b2BodyDef down_bar_body_def;
    down_bar_body_def.userData = downBar;//刚体和精灵绑定
    down_bar_body_def.type=b2_kinematicBody;//不受重力影响又可以移动的类型
    down_bar_body_def.position = b2Vec2(visibleSize.width/PTM_RATIO +2,downBarSize.height/PTM_RATIO/2+offset);
    down_bar_body_def.linearVelocity = b2Vec2(-5,0);
    b2Body  *downBarBody = world->CreateBody(&down_bar_body_def);
    
    b2PolygonShape down_bar_shape;
    down_bar_shape.SetAsBox(downBarSize.width/2/PTM_RATIO, downBarSize.height/2/PTM_RATIO);
    b2FixtureDef down_bar_fixture_def;
    down_bar_fixture_def.shape = &down_bar_shape;
    downBarBody->CreateFixture(&down_bar_fixture_def);
    barContainer->addChild(downBar);
    
    // upBar上方管道
    Texture2D *upbarTexture = TextureCache::sharedTextureCache()->addImage("obstacle_up.png");
    Sprite *upBar = Sprite::createWithTexture(upbarTexture);
    Size upBarSize = upBar->getContentSize();
    b2BodyDef upBar_body_def;
    upBar_body_def.userData = upBar;//刚体和精灵绑定
    upBar_body_def.type=b2_kinematicBody;//不受重力影响又可以移动的类型
    upBar_body_def.position = b2Vec2(visibleSize.width/PTM_RATIO +2,downBarSize.height/PTM_RATIO+offset+3+upBarSize.height/2/PTM_RATIO);
    upBar_body_def.linearVelocity = b2Vec2(-5,0);
    b2Body  *upBarBody = world->CreateBody(&upBar_body_def);
    b2PolygonShape up_bar_shape;
    up_bar_shape.SetAsBox(upBarSize.width/2/PTM_RATIO, upBarSize.height/2/PTM_RATIO);
    b2FixtureDef up_bar_fixture_def;
    up_bar_fixture_def.shape = &up_bar_shape;
    upBarBody->CreateFixture(&up_bar_fixture_def);
    barContainer->addChild(upBar);
    
}
//添加容器，放入管道，便于放置在地面之后
void HelloWorld::addBarContainer()
{
    barContainer = Sprite::create();
    this->addChild(barContainer);
}

void HelloWorld::addBird(cocos2d::Point p)
{
    SpriteBatchNode *parent = SpriteBatchNode::create("bird_hero.png");
    m_pSpriteTexture = parent->getTexture();//获取纹理贴图
    //将精灵批处理节点添加到层
    this->addChild(parent,0,kTagParentNode);
    
//    Sprite *sprite = Sprite::createWithTexture(m_pSpriteTexture,Rect(32*idx, 32*idy, 32, 32));
    
    bird = Sprite::createWithTexture(m_pSpriteTexture);
    parent->addChild(bird);
    
    Size birdSize = m_pSpriteTexture->getContentSize();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    bodyDef.userData = bird;
    b2Body *body = world->CreateBody(&bodyDef);
    
    //定义盒子形状
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(birdSize.width/2/PTM_RATIO, birdSize.height/2/PTM_RATIO);
    
    b2FixtureDef fixtureDef;
//   fixtureDef.filter.groupIndex = 0;
//    fixtureDef.filter.maskBits = 0x0002;//掩码
//    fixtureDef.filter.categoryBits = 0x0003;//设置分类码
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.f;//密度
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.f;//弹性系数
    body->CreateFixture(&fixtureDef);//用刚体创建夹具
    
    //事件监听
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->setSwallowTouches(true);
    listener1->onTouchBegan =[=](Touch *touch,Event *event)
    {
        //设置线性速度
        body->SetLinearVelocity(b2Vec2(0, 7));
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
}



#pragma mark ---碰撞检测处理---

void HelloWorld::BeginContact(b2Contact *contact)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
    b2Body *bodyB = contact->GetFixtureB()->GetBody();
    
    Sprite *spriteA = (Sprite *)bodyA->GetUserData();
    Sprite *spriteB = (Sprite *)bodyB->GetUserData();
    
    //接触的两个刚体任意一个为bird则游戏结束
    if (spriteA ==bird || spriteB == bird) {
        this->endGame();
        MessageBox("游戏失败", "游戏失败");
    }
}

void HelloWorld::EndContact(b2Contact *contact)
{
                    
}

#pragma mark ---Cocos2dx和Box2d中的单位转换---
b2Vec2 HelloWorld::toMeters(Point point)
{
    return b2Vec2(point.x/PTM_RATIO,point.y/PTM_RATIO);
}

Point HelloWorld::toPixels(b2Vec2 vec)
{
    return Point(vec.x * PTM_RATIO, vec.y *PTM_RATIO);
}

#pragma mark ---绘制器方法---
void HelloWorld::draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    CustomCommand *_customCmd = new CustomCommand;
    //_globalZOrder默认为0.000000；
    _customCmd->init(_globalZOrder);
    
    _customCmd->func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, transformUpdated);
    renderer->addCommand(_customCmd);
}

void HelloWorld::onDraw(const kmMat4 &transform, bool transformUpdated)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    kmGLPopMatrix();
}

HelloWorld::~HelloWorld()
{
    delete world;
    world = nullptr;
    
    delete m_debugDraw;
    m_debugDraw = nullptr;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
