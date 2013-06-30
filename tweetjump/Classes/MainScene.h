#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

//#define RESET_DEFAULTS

#define kFPS 60

#define kNumClouds			12

// Fist platform is 30 pixels from the bottom of the screen
#define kStartingCurrentPlatformY	30

#define kMinPlatformStep	50
#define kMaxPlatformStep	300		// This is the top of the screen (SHOULD BE RESOLUTION INDEPENDANT)
#define kNumPlatforms		10		// Number of platforms
#define kPlatformTopPadding 10

#define kMinBonusStep		20
#define kMaxBonusStep		40

enum 
{
	kSpriteManager = 0,
	kBird,
	kScoreLabel,
	kCloudsStartTag = 100,
	kPlatformsStartTag = 200,
	kBonusStartTag = 300
};

enum 
{
	kBonus5 = 0,
	kBonus10,
	kBonus50,
	kBonus100,
	kNumBonuses
};

class MainScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainScene);

	cocos2d::CCSpriteBatchNode* spriteSheet ;

    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);

	void initClouds(void);
	void initCloud(void);
	void resetClouds(void);
	void resetCloud(void);
	void step(float dt);

protected:
	CCSprite * background;

	float currentCloudY;
	int currentCloudTag;
};

#endif  // __MAIN_SCENE_H__