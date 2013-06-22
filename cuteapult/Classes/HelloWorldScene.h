//
//  HelloWorldScene.h
//  firstbox2d
//
//  Created by andy on 11-9-28.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "MyContactListener.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // adds a new sprite at a given coordinate
   // void addNewSpriteWithCoords(cocos2d::CCPoint p);
    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void tick(float dt);
	
	void createBullets(int count);
	bool attachBullet();
	void resetGame();
	void createTarget(const char* imageName ,CCPoint position ,float rotation ,bool isCircle ,bool isStatic ,bool isEnemy);
	void createTargets();
	void resetBullet();
	void attachBullet_v();

	virtual void menuCloseCallback(CCObject* pSender);
    
private:
	
    b2World* world;
	b2Body* groundBody;
	
	b2Fixture* armFixture;
	b2Body* armBody;
	
	b2RevoluteJoint* armJoint;
	b2MouseJoint* mouseJoint;
	
	std::vector<b2Body*> *bullets;
	int currentBullet;
	
	b2Body *bulletBody;
    b2WeldJoint *bulletJoint;
	
	bool releasingArm;
	bool gameover;

	std::vector<b2Body*> *targets;
	std::vector<b2Body*> *enemies;
	
	MyContactListener *contactListener;
	
};

#endif // __HELLO_WORLD_H__
