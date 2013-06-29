#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // adds a new sprite at a given coordinate
    void addNewSpriteWithCoords(cocos2d::CCPoint p);
    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void tick(float dt);
    
private:
    b2World* world;
};

#endif // __HELLOWORLD_SCENE_H__
