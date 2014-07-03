#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define PTM_RATIO 32
#define TILESET_COLUMNS 9
#define TILESE_ROWS 19
#define TILESIZE 32

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"
USING_NS_CC;

typedef enum {
    kTagParentNode = 1,
}tagNode;

class HelloWorld : public Layer,b2ContactListener
{
public:
    
    static Scene* createScene();
    ~HelloWorld();
    void initPhysics();
    
    virtual bool init();  
    
    Size visibleSize;
    
    Sprite *barContainer;//容器
    
    b2World *world;
    
    Sprite *bird;
    
    GLESDebugDraw *m_debugDraw;
    
    Texture2D *m_pSpriteTexture;
    
    Point toPixels(b2Vec2 vec);
    
    b2Vec2 toMeters(Point point);
    
    void update(float dt);
    
    void addBird(cocos2d::Point p);
    
    void addGround();
    
    void addBar(float dt);
    
    void addBarContainer();
    
    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);
    
    void startGame();
    
    void endGame();
    
    void menuCloseCallback(Ref* pSender);
    
    void draw(cocos2d::Renderer *renderer, const kmMat4 &transform, bool transformUpdated);
    void onDraw(const kmMat4 &transform, bool transformUpdated);
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
