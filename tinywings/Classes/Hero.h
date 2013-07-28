#ifndef twxes10_Hero_h
#define twxes10_Hero_h

#include "cocos2d.h"
#include "Box2D.h"

using namespace cocos2d;

class Hero : public CCNode {
    
public:
    b2World *world;
	b2Body *body;
	float radius;
    
    static Hero * heroWithWorld(b2World * w);
    bool initWithWorld(b2World * w);
    
    void sleep();
    void wake();
    void dive();
    void limitVelocity();
    void updateNodePosition();
    
    CC_SYNTHESIZE_READONLY(bool, awake, Awake);
    CC_SYNTHESIZE_RETAIN(CCSprite *, sprite, Sprite);
    
    void createBox2DBody();
    
    ~Hero();
};

#endif
